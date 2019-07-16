#ifndef __UART3_H_ 
#define __UART3_H_

void UART3_Init(unsigned int baudrate );
void UART3_SendByte(unsigned char k);
void Uart3_putch(uint8_t k);
void UART3_SendDat( uint8_t *BufferPtr, uint32_t Length );
void Uart3_Print_String(char *prt);
void Uart3_Printf(const char *fmt,...);
#endif 
