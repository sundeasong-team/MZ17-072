#ifndef __delay_h
#define __delay_h
#include "stdint.h"

//uint32_t SysTickConfig(uint32_t ticks);
void delay_init (uint8_t  SYSCLK);
//void Delay_Ms(uint16_t nms);	  //实验毫秒的延时
//void delayMs(uint32_t delayInMs);
void delay_us(uint16_t i);
void delay_ms(uint16_t  nms);
void Delay_US(uint32_t nCount_temp);
void Delay_MS(uint32_t nCount_temp);
#endif

