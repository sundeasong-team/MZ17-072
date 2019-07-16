#ifndef __UART2_H_ 
#define __UART2_H_

#define UART2_BUFSIZE 0x40

void UART2_Init(uint32_t baudrate );
void UART2_SendByte(uint8_t k);
void UART2_SendDat( uint8_t *BufferPtr, uint32_t Length );
void Uart2_Print_String(char *prt);
#endif

