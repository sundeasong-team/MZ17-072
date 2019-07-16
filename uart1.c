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

void Uart1_putch(uint8_t k) 	//发送
{
	if(k == '\n')
	{
		LPC_UART1->THR = '\r';	//THR 发送保持寄存器，在此写入下一个要发送的字符
		while((LPC_UART1->LSR & LSR_THRE) == 0); //LSR：线状态寄存器。包含发送和接收的状态标志  LSR_THRE = 0x20;“&”为有0为0
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
void UART1_IRQHandler(void)	 //接收
{
	uint8_t IIRValue,RBRValue;
	IIRValue = LPC_UART1->IIR;	//中断标识寄存器，只读
	IIRValue >>= 1 ;	//可写成 IIRValue = IRRValue >>	1;
	IIRValue &= 0x07;	//检查中断标识
	if(IIRValue == IIR_RDA)		//接受数据有效中断即THRE中断IIR_RDA=0x02；
	{
		RBRValue = LPC_UART1->RBR;//接受缓冲寄存器，内含下一个要读取的已接收字符；
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
			 if(uart1_index>BUFSIZE){//表的波特率不对,主动收情况下清0处理
				 uart1_index=0;
			 }
		}
	}
}
