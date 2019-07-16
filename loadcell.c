#include <LPC17xx.h>
#include "Uart.h"
#include "uart1.h"
//#include "uart2.h"  
//===========================================================  
uint8_t cmd[4]={'#','0','1',0x0d};
//===========================================================
void Cell_SendCmd(uint8_t index){
	if(index==2)UART1_SendDat( cmd,4);
}
