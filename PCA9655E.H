#ifndef __PCA9655E_H__
#define __PCA9655E_H__

void PCA9655E_Init(void);
void PCA9655E_OUT(unsigned char portn,unsigned char index,unsigned char f);
uint8_t PCA9655E_Input(uint8_t portn,uint8_t index);

#endif
