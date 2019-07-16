/*****************************************************************************
 *   timer.c:  Timer C file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <lpc17xx.h>
#include "timer.h"
#include "uart.h"
#include "control.h"
#include "io.h"
#include "delay.h"
#include "repeathandle.h"

volatile uint32_t timer0_counter = 0;
volatile uint32_t timer1_counter = 0;
volatile uint32_t timer2_counter = 0;
volatile uint32_t timer3_counter = 0;


extern uint8_t  ZUp_Dir;
extern uint8_t  ZDown_Dir;

/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if ( timer_num == 0 )
  {
		/*
		* setup timer #0 for delay
		*/
		LPC_TIM0 -> TCR = 0x02;		/* reset timer */
		LPC_TIM0 -> PR  = 0x00;		/* set prescaler to zero */
		LPC_TIM0 -> MR0 = delayInMs * (9000000 / 1000-1);
		LPC_TIM0 -> IR  = 0xff;		/* reset all interrrupts */
		LPC_TIM0 -> MCR = 0x04;		/* stop timer on match */
		LPC_TIM0 -> TCR = 0x01;		/* start timer */
	  
		/* wait until delay time has elapsed */
		while (LPC_TIM0 -> TCR & 0x01);
  }
  else if ( timer_num == 1 )
  {
		/*
		* setup timer #1 for delay
		*/
		LPC_TIM1 -> TCR = 0x02;		/* reset timer */
		LPC_TIM1 -> PR  = 0x00;		/* set prescaler to zero */
		LPC_TIM1 -> MR0 = delayInMs * (9000000 / 1000-1);
		LPC_TIM1 -> IR  = 0xff;		/* reset all interrrupts */
		LPC_TIM1 -> MCR = 0x04;		/* stop timer on match */
		LPC_TIM1 -> TCR = 0x01;		/* start timer */
	  
		/* wait until delay time has elapsed */
		while (LPC_TIM1 -> TCR & 0x01);
  }
	else if ( timer_num == 2 )
  {
		/*
		* setup timer #1 for delay
		*/
		LPC_TIM2 -> TCR = 0x02;		/* reset timer */
		LPC_TIM2 -> PR  = 0x00;		/* set prescaler to zero */
		LPC_TIM2 -> MR0 = delayInMs * (9000000 / 1000-1);
		LPC_TIM2 -> IR  = 0xff;		/* reset all interrrupts */
		LPC_TIM2 -> MCR = 0x04;		/* stop timer on match */
		LPC_TIM2 -> TCR = 0x01;		/* start timer */
	  
		/* wait until delay time has elapsed */
		while (LPC_TIM2 -> TCR & 0x01);
  }
	else if ( timer_num == 3 )
  {
		/*
		* setup timer #1 for delay
		*/
		LPC_TIM3 -> TCR = 0x02;		/* reset timer */
		LPC_TIM3 -> PR  = 0x00;		/* set prescaler to zero */
		LPC_TIM3 -> MR0 = delayInMs * (9000000 / 1000-1);
		LPC_TIM3 -> IR  = 0xff;		/* reset all interrrupts */
		LPC_TIM3 -> MCR = 0x04;		/* stop timer on match */
		LPC_TIM3 -> TCR = 0x01;		/* start timer */
	  
		/* wait until delay time has elapsed */
		while (LPC_TIM3 -> TCR & 0x01);
  }
  return;
}


/******************************************************************************
** Function name:		Timer0Handler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER0_IRQHandler (void) 
{ 
	uint8_t IRValue;
	IRValue = LPC_TIM0->IR;
	if ( (IRValue & 0x01) == 0x01 )
	{
		//motor0_control(0);
		LPC_TIM0->IR = LPC_TIM0->IR | 0x01;
	}
	else if ( (IRValue & 0x02) == 0x02 )
	{
		//motor0_control(1);
		LPC_TIM0->IR = LPC_TIM0->IR | 0x02;
	}
	timer0_counter++;
  LPC_TIM0 -> IR = 1;			/* clear interrupt flag */
}
/******************************************************************************
** Function name:		Timer1Handler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER1_IRQHandler (void)  
{ 
  LPC_TIM1 -> IR = 1;			/* clear interrupt flag */
	timer1_counter++;
	//Uart_Printf("timer1_counter=%d\r\n",timer1_counter);	
  if(timer1_counter==200)//20ms
	{
	   timer1_counter=0;
		 //LoadcellCheck();
	}
}

/******************************************************************************
** Function name:		Timer1Handler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER2_IRQHandler (void)  
{ 
	uint8_t IRValue;
	IRValue = LPC_TIM2->IR;
	
	if ( (IRValue & 0x01) == 0x01 )
	{
		//motor2_control(0);
		LPC_TIM2->IR = LPC_TIM2->IR | 0x01;
	}
	else if ( (IRValue & 0x02) == 0x02 )
	{
		//motor2_control(1);
		LPC_TIM2->IR = LPC_TIM2->IR | 0x02;
	}
	timer2_counter++;
  LPC_TIM2 -> IR = 1;			/* clear interrupt flag */
}



/******************************************************************************
** Function name:		Timer1Handler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER3_IRQHandler (void)  
{ 

	timer3_counter++;
  LPC_TIM3 -> IR = 1;			/* clear interrupt flag */
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
		LPC_TIM0 -> TCR = 1;
  }
  else if ( timer_num == 1 )
  {
		LPC_TIM1 -> TCR = 1;
  }
	else if ( timer_num == 2 )
  {
		LPC_TIM2 -> TCR = 1;
  }
	else if ( timer_num == 3 )
  {
		LPC_TIM3 -> TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
		LPC_TIM0 -> TCR = 0;
  }
  else if ( timer_num == 1 )
  {
		LPC_TIM1 -> TCR = 0;
  }
	else if ( timer_num == 2 )
  {
		LPC_TIM2 -> TCR = 0;
  }
	else if ( timer_num == 3 )
  {
		LPC_TIM3 -> TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void reset_timer( uint8_t timer_num )
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
		regVal = LPC_TIM0 -> TCR;
		regVal |= 0x02;
		LPC_TIM0 -> TCR = regVal;
  }
  else if ( timer_num == 1 )
  {
		regVal = LPC_TIM1 -> TCR;
		regVal |= 0x02;
		LPC_TIM1 -> TCR = regVal;
  }
	else if ( timer_num == 2 )
  {
		regVal = LPC_TIM2 -> TCR;
		regVal |= 0x02;
		LPC_TIM2 -> TCR = regVal;
  }
	else if ( timer_num == 3 )
  {
		regVal = LPC_TIM3 -> TCR;
		regVal |= 0x02;
		LPC_TIM3 -> TCR = regVal;
  }
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
** 
******************************************************************************/
uint32_t init_timer ( uint8_t timer_num, uint32_t TimerInterval ) 
{

  if ( timer_num == 0 )
  {
		timer0_counter = 0;
		LPC_TIM0 -> MR0 = TimerInterval;
		LPC_TIM0 -> MR1 = TimerInterval/2;
		LPC_TIM0 -> MCR = 11;				/* Interrupt and Reset on MR0 */
	
		NVIC_EnableIRQ(TIMER0_IRQn);
	
		return (1);
  }
  else if ( timer_num == 1 )//使用计时功能
  {
		LPC_TIM1 -> TCR = 0x02;
		LPC_TIM1 -> TC  = 0x00;
		LPC_TIM1 -> PR  = 0x00;
		LPC_TIM1 -> MR0 = TimerInterval;
		LPC_TIM1 -> MCR = 0x03;	
		LPC_TIM1 -> IR  = 0xff;
		LPC_TIM1 -> TCR = 0x01; 
		NVIC_EnableIRQ(TIMER1_IRQn);
		return (1);
  }
	else if ( timer_num == 2 )
  {
		timer2_counter = 0;
		LPC_TIM2 -> MR0 = TimerInterval;
		LPC_TIM2 -> MR1 = TimerInterval/2;
		LPC_TIM2 -> MCR = 11;				/* Interrupt and Reset on MR1 */
	
		NVIC_EnableIRQ(TIMER2_IRQn);
	
		return (1);
  }
	else if ( timer_num == 3 )
  {
		timer3_counter = 0;
		LPC_TIM3 -> MR0 = TimerInterval;
		LPC_TIM3 -> MCR = 3;				/* Interrupt and Reset on MR1 */
	
		NVIC_EnableIRQ(TIMER3_IRQn);
	
		return (1);
  }
  return (0);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
