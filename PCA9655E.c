#include "lpc17xx.h"
#include "stdio.h"
#include "IIC1.h"
#include "IIC2.h"
#include "uart.h"
//===================================================
uint8_t buf[];
uint8_t record0=0x00,record1=0x00;
uint8_t ADH4;
//===================================================
void PCA9655E_Init(){
	 //===============output===========================
	 //----config port0 as out-----  
	 I2C1_Start();
	 I2C1_Puts(0x40);//address
   I2C1_Puts(0x06);//config port0 as out
	 I2C1_Puts(0x00);//config port0 as out
   I2C1_Stop();
   //----config port1 as out----- 
   I2C1_Start();
	 I2C1_Puts(0x40);//address
   I2C1_Puts(0x07);//config port1 as out
	 I2C1_Puts(0x00);//config port1 as out
   I2C1_Stop();	
	 //----config port0 as low-----
	 I2C1_Start();
	 I2C1_Puts(0x40);//address
	 I2C1_Puts(0x02);//output port0
	 I2C1_Puts(0x00);//output low
	 I2C1_Stop();	
	 //----config port1 as low----- 
	 I2C1_Start();
	 I2C1_Puts(0x40);//address
	 I2C1_Puts(0x03);//output port1
	 I2C1_Puts(0x00);//output low
	 I2C1_Stop();
	 
	 //===============input===========================
	 //LPC_GPIO0->FIODIR=LPC_GPIO0->FIODIR&(~(1<<16));//SSEL as input  (used for interrupted input)
	 //----config port0 as inout-----  
	 I2C2_Start();
	 I2C2_Puts(0x42);//address
   I2C2_Puts(0x06);//config port0 as input
	 I2C2_Puts(0xff);//config port0 as input
   I2C2_Stop();
   //----config port1 as inout----- 
   I2C2_Start();
	 I2C2_Puts(0x42);//address
   I2C2_Puts(0x07);//config port1 as input
	 I2C2_Puts(0xff);//config port1 as input
   I2C2_Stop();	
}
//===================================================
//portn -----port0 or port1
//index -----select io port
//f------set io high or low
//===================================================
void PCA9655E_OUT(uint8_t portn,uint8_t index,uint8_t f){//port0 index(0-7) 
	 //-----------
   I2C1_Start();
   I2C1_Puts(0x40);//address
	
	 if(portn==0){
		 I2C1_Puts(0x02);//select port0 
		 if(f==1)record0=record0 | (1<<index);   //单独cy8on record0=0x80    再cy9on record0=0x81  单独的cy9on record0=0x01
		 else    record0=record0 & (~(1<<index));
		 I2C1_Puts(record0);//单独cy8on record0=0x80	再cy9on record0=0x81   单独的cy9on record0=0x01
		 I2C1_Stop();
		 }
	 if(portn==1){
		 I2C1_Puts(0x03);//select port1
		 if(f==1)record1=record1 | (1<<index);   //单独cy8on record0=0x80    再cy9on record0=0x81  单独的cy9on record0=0x01
		 else    record1=record1 & (~(1<<index));
		 I2C1_Puts(record1);//单独cy8on record0=0x80	再cy9on record0=0x81   单独的cy9on record0=0x01
		 I2C1_Stop();
	 }                                                                 //再cy1on record0=0x81

}
//===================================================
//portn -----port0 or port1
//index -----select io port
//===================================================
uint8_t PCA9655E_Input(uint8_t portn,uint8_t index)
{
	uint8_t result=0;
  I2C2_Start();
  I2C2_Puts(0x42);//address写操作 
	if(portn==0) I2C2_Puts(0x00);//read port0
	if(portn==1) I2C2_Puts(0x01);//read port1
			
	I2C2_Start();	
	I2C2_Puts(0x43);//address读操作
	result=I2C2_Read();
	//Uart_Printf("%x\r\n",result);//0xfe
	I2C2_Stop();
	return ((result>>index)&0x01);	
}

