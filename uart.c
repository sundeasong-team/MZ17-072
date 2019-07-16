#include <lpc17xx.h>
#include "type.h"
#include "uart.h"
#include "stdarg.h"
#include "stdio.h"	
#include "math.h"	

volatile uint32_t UART0Status;
volatile uint8_t  UART0TxEmpty = 1;
volatile uint8_t  UART0Buffer[BUFSIZE];   //0x40
volatile uint32_t UART0Count = 0;
volatile uint8_t  Uart0Flag=0;
extern uint32_t SystemCoreClock;
//==================================================================================
void UART0_Init( uint32_t baudrate ){
    uint32_t Fdiv;
	  LPC_PINCON->PINSEL0 |= 0x00000050;       /* RxD0 is P0.3 and TxD0 is P0.2 */
    LPC_UART0->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	  /* By default, the PCLKSELx value is zero, thus, the PCLK for
	  all the peripherals is 1/4 of the SystemFrequency. */ 
    Fdiv = ( SystemCoreClock/4/16 ) / baudrate ;	/*baud rate, Fpclk: 18MHz */
    LPC_UART0->DLM = Fdiv / 256;							
    LPC_UART0->DLL = Fdiv % 256;
	  LPC_UART0->LCR = 0x03;		/* DLAB = 0 Disable access to Divisor Latches*/
    LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO one char interrupt. */
    LPC_UART0->IER = IER_RBR;	/* Enable UART0 interrupt */
	  NVIC_EnableIRQ(UART0_IRQn); 
}
//============================================================================
void putch(uint8_t k){
	if(k=='\n'){ //Enter
		//LPC_UART0->THR ='\r';//一定要屏蔽 Print_String()函数调用了putch();
		while((LPC_UART0->LSR & LSR_THRE)==0);
		LPC_UART0->THR =k;
		while((LPC_UART0->LSR & LSR_THRE)==0);
	}else{
		LPC_UART0->THR =k;
		while((LPC_UART0->LSR & LSR_THRE)==0);
	}
}
//============================================================================
void u0putch(uint8_t k){
	  LPC_UART0->THR =k;
	  while((LPC_UART0->LSR & LSR_THRE)==0);
}
//=============================================
void Print_String(char *ptr){
  	while(*ptr!='\0')putch(*ptr++);
}
//============================================									
//variable-length 
void Uart_Printf(const char *fmt,...){  
	  va_list ap;	   //This is support the format output 
    char string[50];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap); // Store the string in the String buffer
    Print_String(string);
    va_end(ap);	 

}
//=============================================
void printU32(unsigned long g){
  unsigned char seg[]={'0','1','2','3','4','5','6','7','8','9'};
  unsigned char a[10];
  unsigned char l=0;
  unsigned char pp=9;
  a[0]=(g%10000000000)/1000000000;
  a[1]=(g%1000000000)/100000000;
  a[2]=(g%100000000)/10000000;
  a[3]=(g%10000000)/1000000;
  a[4]=(g%1000000)/100000;
  a[5]=(g%100000)/10000;
  a[6]=(g%10000)/1000;
  a[7]=(g%1000)/100;
  a[8]=(g%100)/10;
  a[9]=g%10;
  
  for(l=0;l<10;l++){																
    if(a[l]!=0){pp=l;l=10;break;}
  }
  for(l=pp;l<10;l++){
    u0putch(seg[a[l]]);
  }    
}
//=======================================================
void print_float(float f,unsigned char len)
{
	unsigned int a;
	unsigned char i;

	printU32((unsigned long)f);		//打印整数
	Uart_Printf(".");
	f=f*pow(10,len);
	for(i=0;i<len;i++)
	{
		a=(unsigned int)f/(unsigned int)pow(10,len-i-1)%10;
		u0putch(a+'0');
	}
}
//===========================================================================
void UART0_IRQHandler (void){
  uint8_t IIRValue,RBRValue;
  uint8_t Dummy = Dummy;	
  IIRValue = LPC_UART0->IIR; //Interrupt ID Register  
  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  {	   //接收数据有效中断
		/* Receive Data Available */
		RBRValue=LPC_UART0->RBR;
		if(RBRValue=='\b'){
		  if(UART0Count>0){
			putch('\b');
			putch(' ');
			putch('\b');
			UART0Count=UART0Count-1;
		   }
		}
		else if(RBRValue=='\r')	{
			  UART0Buffer[UART0Count] ='\0';
			  putch('\r');//回显命令后面的\r\n
			  putch('\n');//"\r\n"
		   	Uart0Flag=1;
		}
		else if(RBRValue=='\n')	{}
		else {
			if( UART0Count < (sizeof(UART0Buffer)-1) ) UART0Buffer[UART0Count++]=RBRValue;
		    putch(RBRValue);
		}
  }

    
}
/******************************************************************************
**                            End Of File
******************************************************************************/

