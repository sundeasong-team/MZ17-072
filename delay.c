#include <lpc17xx.h>
#include "delay.h"     
static uint8_t  fac_us=0;//usÑÓÊ±±¶³ËÊý
static uint32_t  fac_ms=0;//msÑÓÊ±±¶³ËÊý
//³õÊ¼»¯ÑÓ³Ùº¯Êý
//SY ST I CKµÄÊ±ÖÓ¹Ì¶¨ÎªHCLKÊ±ÖÓµÄ1/8
//SY SCLK:ÏµÍ³Ê±ÖÓ
void delay_init (uint8_t  SYSCLK)
{
	//SysTick->CTRL&=0xfffffffd;//bit 2Çå¿Õ,Ñ¡ÔñÍâ²¿Ê±ÖÓ  HCLK/8
	fac_us=SYSCLK;           
	fac_ms=(uint16_t)fac_us*1000;
}                                                                   
//ÑÓÊ±nms
//×¢ÒânmsµÄ·¶Î§
//SysTick->LOADÎª24Î»¼Ä´æÆ÷,ËùÒÔ,×î´óÑÓÊ±Îª:
//nms<=0xffffff*8*1000/SY SCLK
//SYSCLKµ¥Î»ÎªHz,nmsµ¥Î»Îªms
//¶Ô72MÌõ¼þÏÂ,nms<=1864
void delay_ms(uint16_t  nms)
{                            
	uint32_t  temp;
	SysTick->LOAD = (uint32_t )fac_ms;//Ê±¼ä¼ÓÔØ(SysTick->LOADÎª24bi t )
	SysTick->VAL = 0x00;           //Çå¿Õ¼ÆÊýÆ÷
	SysTick->CTRL |= 0x05 ;          //¿ªÊ¼µ¹Êý
	while (nms--)
	{
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));//µÈ´ýÊ±¼äµ½´ï
		SysTick->LOAD = (uint32_t )fac_ms;//Ê±¼ä¼ÓÔØ(SysT i ck->LOA DÎª24bi t )
		SysTick->VAL = 0x00;           //Çå¿Õ¼ÆÊý÷
	}
	SysTick->CTRL = 0x00;       //¹Ø±Õ¼ÆÊýÆ÷
	SysTick->VAL = 0X00;       //Çå¿Õ¼ÆÊýÆ÷                     
}  

//ÑÓÊ±nus
//nusÎªÒªÑÓÊ±µÄusÊý.                                                                                       
void delay_us(uint16_t  nus)
{       
	uint32_t  temp;            
	SysTick->LOAD = nus*fac_us; //Ê±¼ä¼ÓÔØ                  
	SysTick->VAL = 0x00;        //Çå¿Õ¼ÆÊýÆ÷
	SysTick->CTRL = 0x05;      //¿ªÊ¼µ¹Êý 
	do
	{
		temp = SysTick->CTRL;
	}
	while (temp&0x01&&!(temp&(1<<16)));//µÈ´ýÊ±¼äµ½´ï
	SysTick->CTRL=0x00;       //¹Ø±Õ¼ÆÊýÆ÷
	SysTick->VAL=0X00;       //Çå¿Õ¼ÆÊýÆ÷        
}
//===========================================================================
void Delay_US(uint32_t nCount_temp)
{
	uint32_t nCount=nCount_temp*20;//Èí¼þ·ÂÕæ0.00073464  0.00173477¼ä¸ô1ms
	while(nCount--);
}
/****************************************************************************
* Ãû	³Æ£ºvoid Delay_MS(u32 nCount_temp)
* ¹¦	ÄÜ£ººÁÃë¼¶
* Èë¿Ú²ÎÊý£ºu32 nCount_temp	ÑÓÊ±Á¿
* ³ö¿Ú²ÎÊý£ºÎÞ
* Ëµ	Ã÷£ºÎÞ
****************************************************************************/
void Delay_MS(uint32_t nCount_temp)
{
	uint32_t nCount=nCount_temp*20000;//Èí¼þ·ÂÕæ0.00073464 1.000734777¼ä¸ô1s
	while(nCount--);
}
