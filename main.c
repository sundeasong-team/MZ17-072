#include "lpc17xx.h"
#include "Uart.H"
#include "Command.H"
#include "IIC.H"
#include "IIC1.h"
#include "IIC2.h"
#include "24c128.h"
#include "Uart1.H"
#include "Uart2.H"
#include "Uart3.H"
#include "control.h"
#include "io.h"
#include "timer.h"
#include "delay.h"
#include "repeathandle.h"
#include "PCA9655E.H"

extern volatile uint8_t  Uart0Flag;
extern volatile uint32_t UART0Count;
extern volatile uint8_t  UART0Buffer[BUFSIZE];
extern uint8_t uart1_flag;
extern uint8_t uart1_index;

extern volatile uint32_t UART2Count;

//===========================================
 int  main(){ 	
	 SystemInit();
	 delay_init(100);
	 IO_Init();
	 I2C_Init();
	 I2C1_Init();
	 I2C2_Init();
	 PCA9655E_Init();
	 UART0_Init(19200);//电脑串口 
	 UART2_Init(19200);//485接口
	 init_config();
	 Motor_init(); 	 
	 fixture_init();
	 Uart_Printf("ok\r\n");
   while(1)
	 {		 		
			if(Uart0Flag==1)
				{		             /*If receive the command*/
			  Uart0Flag=0;
			  UART0Count=0;
			  Check((char *)UART0Buffer);  /*Run the action*/
				}
			key_scan();
//		if((SENSOR_PUSH_front==0)&&(SENSOR_Y_front==0))//1009
//		{
//				ION_ON();
//		}
//		else ION_OFF();
	 }	
}





