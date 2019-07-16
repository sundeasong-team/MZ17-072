/*****************************************************************************
 *   timer.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TIMER_H 
#define __TIMER_H

void delayMs(uint8_t timer_num,uint32_t delayInMs);
uint32_t init_timer(uint8_t timer_num,uint32_t timerInterval);
void enable_timer(uint8_t timer_num);
void disable_timer(uint8_t timer_num);
void reset_timer(uint8_t timer_num);
void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER2_IRQHandler(void);

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
