#include <lpc17xx.h>
#include "delay.h"     
static uint8_t  fac_us=0;//us��ʱ������
static uint32_t  fac_ms=0;//ms��ʱ������
//��ʼ���ӳٺ���
//SY ST I CK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SY SCLK:ϵͳʱ��
void delay_init (uint8_t  SYSCLK)
{
	//SysTick->CTRL&=0xfffffffd;//bit 2���,ѡ���ⲿʱ��  HCLK/8
	fac_us=SYSCLK;           
	fac_ms=(uint16_t)fac_us*1000;
}                                                                   
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SY SCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(uint16_t  nms)
{                            
	uint32_t  temp;
	SysTick->LOAD = (uint32_t )fac_ms;//ʱ�����(SysTick->LOADΪ24bi t )
	SysTick->VAL = 0x00;           //��ռ�����
	SysTick->CTRL |= 0x05 ;          //��ʼ����
	while (nms--)
	{
		do
		{
			temp=SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
		SysTick->LOAD = (uint32_t )fac_ms;//ʱ�����(SysT i ck->LOA DΪ24bi t )
		SysTick->VAL = 0x00;           //��ռ����
	}
	SysTick->CTRL = 0x00;       //�رռ�����
	SysTick->VAL = 0X00;       //��ռ�����                     
}  

//��ʱnus
//nusΪҪ��ʱ��us��.                                                                                       
void delay_us(uint16_t  nus)
{       
	uint32_t  temp;            
	SysTick->LOAD = nus*fac_us; //ʱ�����                  
	SysTick->VAL = 0x00;        //��ռ�����
	SysTick->CTRL = 0x05;      //��ʼ���� 
	do
	{
		temp = SysTick->CTRL;
	}
	while (temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL=0X00;       //��ռ�����        
}
//===========================================================================
void Delay_US(uint32_t nCount_temp)
{
	uint32_t nCount=nCount_temp*20;//��������0.00073464  0.00173477���1ms
	while(nCount--);
}
/****************************************************************************
* ��	�ƣ�void Delay_MS(u32 nCount_temp)
* ��	�ܣ����뼶
* ��ڲ�����u32 nCount_temp	��ʱ��
* ���ڲ�������
* ˵	������
****************************************************************************/
void Delay_MS(uint32_t nCount_temp)
{
	uint32_t nCount=nCount_temp*20000;//��������0.00073464 1.000734777���1s
	while(nCount--);
}