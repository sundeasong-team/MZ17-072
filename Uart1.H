#ifndef __UART1_H_ 
#define __UART1_H_

void UART1_Init(unsigned int baudrate );
void UART1_SendByte(unsigned char k);
void UART1_SendDat( uint8_t *BufferPtr, uint32_t Length );
void Uart1_Print_String(char *prt);
void Uart1_Printf(const char *fmt,...);
#endif 
