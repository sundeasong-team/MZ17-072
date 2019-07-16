#include <lpc17xx.h>
#include "stdarg.h"
#include "stdio.h"	
#include "uart.h"
#include "uart2.h"
#include "delay.h"
#include "toyocontrol.h"

//============================================================================
extern uint32_t SystemCoreClock;
extern unsigned char Ascill_16[];
//============================================================================
volatile uint8_t  UART2Buffer[UART2_BUFSIZE];   //0x40
volatile uint32_t UART2Count = 0;
volatile uint8_t  Uart2Flag=0;
//============================================================================
void UART2_Init( uint32_t baudrate ){
    uint32_t Fdiv;
	  LPC_PINCON->PINSEL0 |=(LPC_PINCON->PINSEL0 & 0xff0fffff) | 0X00500000;  // Enable RxD2 P0.10, TxD2 P0.11    //Take Apart
    LPC_UART2->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	  /* By default, the PCLKSELx value is zero, thus, the PCLK for
	  all the peripherals is 1/4 of the SystemFrequency. */ 
    Fdiv = ( SystemCoreClock/4/16 ) / baudrate ;	/*baud rate */
    LPC_UART2->DLM = Fdiv / 256;							
    LPC_UART2->DLL = Fdiv % 256;
	  LPC_UART2->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART2->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

    LPC_UART2->IER = IER_RBR | IER_THRE ;//| IER_RLS;	/* Enable UART2 interrupt */
	  NVIC_EnableIRQ(UART2_IRQn);
}

//========================================
void UART2_SendByte(uint8_t k){
	  LPC_UART2->THR =k;
	  while((LPC_UART2->LSR & LSR_THRE)==0) ;
}
//===============================================================================
void Uart2_Print_String(char *str)
{
 unsigned char Lrc=0;
 unsigned char Temp=0,Temp_h=0,Temp_l=0;
 unsigned char i=0;
 unsigned char StrCmp[UART2_BUFSIZE];
	
	while( (*str) != '\0')       //ֱ���ַ�������
	{		
		StrCmp[i]= (*str);			 	 //��ָ������ݸ�ֵ��StrCmp����
		str++;	                   //str�ʼ=1                       
		if(StrCmp[0] == 0x3a)		 	 //���ַ�������":"
	  {							 
			if(i%2==0){	             //�����ż��λ(��ʼ���һ�����ݵĸ�λ)
        Temp_h=ChrToInt(*str); //��λ�ַ�ת��Ϊ���� ʵ�������i+1��ֵ	
				Temp=0;                //����Tempÿȡ��ߵ�λ����0,Ϊ����һ����׼�� (�������0 Lrc�Ͳ���ȷ ��ʱ������ϸ�����else����е�Tempֵ)
      }
			else{	                   //����ǻ���λ(����λ)
        Temp_l=ChrToInt(*str); //��λ�ַ�ת��Ϊ����	                                      
        Temp=Temp_h*16+Temp_l; //��λ���ϵ�λ
      }	
			Lrc += Temp;             //����λ�ۼ�
			i++;                     //i�ӣ���ʼ��
		}
	}
	Lrc = (~Lrc)+1;              //ȡ����	
	StrCmp[i]=Ascill_16[Lrc/16]; //ת��Ϊ�ַ� 	Uart_Printf("StrCmp[%d]=%c\n",i,Ascill_16[Lrc/16]);
	StrCmp[++i]=Ascill_16[Lrc%16];//ת��Ϊ�ַ�	Uart_Printf("StrCmp[%d]=%c\n",i,Ascill_16[Lrc%16]);
	StrCmp[++i]='\0';

	LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<9));//485 en  
	//NVIC_DisableIRQ(UART2_IRQn);
	i=0;
	while(StrCmp[i] != '\0')
	{
		 UART2_SendByte(StrCmp[i]); i++;//0X39
	}
	UART2_SendByte(0xD);
	UART2_SendByte(0xA);
	delay_us(560);//530---660 600msʱ��ż�����ֽ��մ���560
	LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<9)); 
  //NVIC_EnableIRQ(UART2_IRQn);

}
//=========================================================================
void UART2_SendDat( uint8_t *BufferPtr, uint32_t Length ){
	  LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<9));//485 en  
	  while ( Length != 0 ){
	  UART2_SendByte(*BufferPtr);
	  BufferPtr++;
	  Length--;
    }
		Delay_US(100);//60-800us 100  
	  LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<9));
}
//====================================================================
void Uart2_Printf(const char *fmt,...)//
{
	  va_list ap;	   //This is support the format output 
    char string[60];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap); // Store the string in the String buffer
    Uart2_Print_String(string);
    va_end(ap);	 
}
//====================================================================
void UART2_Get_String(void){
	unsigned char i;
	unsigned char Lrc=0;
	unsigned char Temp=0,Temp_h=0,Temp_l=0;
	unsigned char Lrc_buf[2];
	if(Uart2Flag==1){
		 if(UART2Buffer[0]==':'){
				for(i=1;i<UART2Count-2;i++)//:01060D001000		
				{
					if(i%2!= 0){//����λΪ��λ
	          Temp_h=ChrToInt(UART2Buffer[i]);	//0    0    0     0    1         0      D
						Temp=0;
          }					
					else{ 	    //ż��λΪ��λ
						    Temp_l=ChrToInt(UART2Buffer[i]);//1     6    D     0    0          0      C
                Temp=Temp_h*16+Temp_l;          //1     6    D     0    0X10       0      0XDC 
          }	        
					Lrc += Temp;
				}
				Lrc = (~Lrc)+1;             //ȡ���� 0X24
				Lrc_buf[0]=Lrc/16;
				Lrc_buf[1]=Lrc%16;		
				if( (ChrToInt(UART2Buffer[UART2Count-2])==Lrc_buf[0])&&(ChrToInt(UART2Buffer[UART2Count-1])==Lrc_buf[1]) )
				{
					 Rx_Flag=1;
					 for(i=0;i<UART2Count-2;i++)		  
				   Rx_String[i]=UART2Buffer[i];
					 Rx_String[i]='\0';
				}
				else{
					 UART2Count=0;
				}
				
		}else{
			  UART2Count=0;
		}
				 
	}
	UART2Count=0;
	Uart2Flag=0;

}
//====================================================================
void UART2_IRQHandler (void){
	uint8_t IIRValue,RBRValue;	
	IIRValue = LPC_UART2->IIR;  //Interrupt ID Register  
	IIRValue >>= 1;			    /* skip pending bit in IIR */
	IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
	if( IIRValue == IIR_RDA )//�����ж�
	{	
		RBRValue=LPC_UART2->RBR;//���ܻ���Ĵ������ں���һ��Ҫ��ȡ���ѽ����ַ�  0xe8  0x82 0x8a 0x82 0xb2 0x92 0x82 0x8a 0x2a 0x05 0x82
		if(RBRValue == '\r')                                                  //0x82  0x9a 0x12 0x85 0x6a
		{
		}
		else if(RBRValue=='\n')	{	//'\n'���� 0x0a
			//UART2Buffer[UART2Count]='\0';
			Uart2Flag=1;
		  UART2_Get_String();
    }
    else{		
			if( UART2Count < (sizeof(UART2Buffer)-1)){//{':','0','0','0','3','1','0','0','A','0','0','0','2','\0'};
				UART2Buffer[UART2Count++]=RBRValue;
			}
			//UART2_SendByte(RBRValue);
	  }
		
	}
	
}


