//#include <LPC17xx.h>
//#include <System_LPC17xx.h>
//volatile uint32_t  SystemFrequency=0;
//void PPL0_100M(void){
//  LPC_SC->CCLKCFG  = 0x03;			        /*CPU Clock Configuration register 4 PLL出来后四分频*/
//  LPC_SC->CLKSRCSEL = 0x01;                 /* Select Main Oscillator Clock 12M Source for PLL0       */
//  LPC_SC->PLL0CFG   = 0x00050063;			//M>N  M 14:0  N 16:23
//  LPC_SC->PLL0CON   = 0x01;                 /* PLL0 Enable                        */
//  LPC_SC->PLL0FEED  = 0xAA;
//  LPC_SC->PLL0FEED  = 0x55;
//  while (!(LPC_SC->PLL0STAT & (1 << 26)));  /* Wait for PLOCK0                    */
//
//  LPC_SC->PLL0CON   = 0x03;                 /* PLL0 Enable & Connect              */
//  LPC_SC->PLL0FEED  = 0xAA;
//  LPC_SC->PLL0FEED  = 0x55;
//	
////  SystemFrequency  = 100000000;
//}

