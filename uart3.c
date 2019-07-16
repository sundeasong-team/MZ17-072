#include <lpc17xx.h>
#include "stdarg.h"
#include "stdio.h"	
#include "uart.h"

//============================================================================
extern uint32_t SystemCoreClock;
volatile uint8_t  *UART3TxBuffer;//0x40
volatile uint32_t UART3TxCount = 0;
volatile uint32_t UART3TxCurrentCount = 0;
//======================
uint8_t uart3_buf[BUFSIZE]={0,0,0,0,'0','0',0,'0','0'};
uint8_t uart3_index=0;
uint8_t uart3_flag=0;
//============================================================================
void UART3_Init( uint32_t baudrate ){
  uint32_t Fdiv;
	LPC_PINCON->PINSEL9 |= 0x0F000000;	/* Enable RxD3 P4.29, TxD3 P4.28 */
	
  LPC_UART3->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */ 
    Fdiv = ( SystemCoreClock/4/16 ) / baudrate ;	/*baud rate */
    LPC_UART3->DLM = Fdiv / 256;							
    LPC_UART3->DLL = Fdiv % 256;
	  LPC_UART3->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART3->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART3_IRQn);

    LPC_UART3->IER = IER_RBR | IER_THRE ;// | IER_RLS;	/* Enable UART3 interrupt */
}

void Uart3_putch(uint8_t k) 	//����
{
	if(k == '\n')
	{
		LPC_UART3->THR = '\r';	//THR ���ͱ��ּĴ������ڴ�д����һ��Ҫ���͵��ַ�
		while((LPC_UART3->LSR & LSR_THRE) == 0); //LSR����״̬�Ĵ������������ͺͽ��յ�״̬��־  LSR_THRE = 0x20;��&��Ϊ��0Ϊ0
		LPC_UART3->THR = k;
		while((LPC_UART3->LSR & LSR_THRE) == 0);
	}
	else
	{
		LPC_UART3->THR = k;
		while((LPC_UART3->LSR & LSR_THRE) == 0);
	}	
}
//===============================================================================
void Uart3_Print_String(char *prt)
{
	while(*prt != '\0')
	Uart3_putch(*prt++);
}
//===============================================================================
void Uart3_Printf(const char *fmt,...)//
{
	  va_list ap;	   //This is support the format output 
    char string[60];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap); // Store the string in the String buffer
    Uart3_Print_String(string);
    va_end(ap);	 
}
//===============================================================================
void UART3_IRQHandler(void)	 //����
{
	uint8_t IIRValue,RBRValue;
	IIRValue = LPC_UART3->IIR;	//�жϱ�ʶ�Ĵ�����ֻ��
	IIRValue >>= 1 ;	//��д�� IIRValue = IRRValue >>	1;
	IIRValue &= 0x07;	//����жϱ�ʶ
	if(IIRValue == IIR_RDA)		//����������Ч�жϼ�THRE�ж�IIR_RDA=0x02��
	{
		RBRValue = LPC_UART3->RBR;//���ܻ���Ĵ������ں���һ��Ҫ��ȡ���ѽ����ַ���
    //Uart_Printf("%c",RBRValue);
		if(RBRValue == '\r')
		{
		  uart3_buf[uart3_index]='\0';
		  uart3_flag=1;
		  uart3_index=0;
		}
		else
		{
		   uart3_buf[uart3_index]=RBRValue;
		   uart3_index=uart3_index+1;
			 if(uart3_index>BUFSIZE){//��Ĳ����ʲ���,�������������0����
				 uart3_index=0;
			 }
		}
	}
}
