#include <lpc17xx.h>
#include "stdarg.h"
#include "stdio.h"	
#include "uart.h"

//============================================================================
extern uint32_t SystemCoreClock;
volatile uint8_t  *UART1TxBuffer;//0x40
volatile uint32_t UART1TxCount = 0;
volatile uint32_t UART1TxCurrentCount = 0;
//======================
uint8_t uart1_buf[BUFSIZE]={0,0,0,0,'0','0',0,'0','0'};
uint8_t uart1_index=0;
uint8_t uart1_flag=0;
//============================================================================
void UART1_Init( uint32_t baudrate ){
  uint32_t Fdiv;
	LPC_PINCON->PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 */
	
  LPC_UART1->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */ 
    Fdiv = ( SystemCoreClock/4/16 ) / baudrate ;	/*baud rate */
    LPC_UART1->DLM = Fdiv / 256;							
    LPC_UART1->DLL = Fdiv % 256;
	  LPC_UART1->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART1_IRQn);

    LPC_UART1->IER = IER_RBR | IER_THRE ;// | IER_RLS;	/* Enable UART1 interrupt */
}

void Uart1_putch(uint8_t k) 	//����
{
	if(k == '\n')
	{
		LPC_UART1->THR = '\r';	//THR ���ͱ��ּĴ������ڴ�д����һ��Ҫ���͵��ַ�
		while((LPC_UART1->LSR & LSR_THRE) == 0); //LSR����״̬�Ĵ������������ͺͽ��յ�״̬��־  LSR_THRE = 0x20;��&��Ϊ��0Ϊ0
		LPC_UART1->THR = k;
		while((LPC_UART1->LSR & LSR_THRE) == 0);
	}
	else
	{
		LPC_UART1->THR = k;
		while((LPC_UART1->LSR & LSR_THRE) == 0);
	}	
}
//===============================================================================
void Uart1_Print_String(char *prt)
{
	while(*prt != '\0')
	Uart1_putch(*prt++);
}
//===============================================================================
void Uart1_Printf(const char *fmt,...)//
{
	  va_list ap;	   //This is support the format output 
    char string[60];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap); // Store the string in the String buffer
    Uart1_Print_String(string);
    va_end(ap);	 
}
//===============================================================================
void UART1_IRQHandler(void)	 //����
{
	uint8_t IIRValue,RBRValue;
	IIRValue = LPC_UART1->IIR;	//�жϱ�ʶ�Ĵ�����ֻ��
	IIRValue >>= 1 ;	//��д�� IIRValue = IRRValue >>	1;
	IIRValue &= 0x07;	//����жϱ�ʶ
	if(IIRValue == IIR_RDA)		//����������Ч�жϼ�THRE�ж�IIR_RDA=0x02��
	{
		RBRValue = LPC_UART1->RBR;//���ܻ���Ĵ������ں���һ��Ҫ��ȡ���ѽ����ַ���
    //Uart_Printf("%c",RBRValue);
		if(RBRValue == '\r')
		{
		  uart1_buf[uart1_index]='\0';
		  uart1_flag=1;
		  uart1_index=0;
		}
		else
		{
		   uart1_buf[uart1_index]=RBRValue;
		   uart1_index=uart1_index+1;
			 if(uart1_index>BUFSIZE){//��Ĳ����ʲ���,�������������0����
				 uart1_index=0;
			 }
		}
	}
}
