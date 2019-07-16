#ifndef __24C128_H__
#define __24C128_H__

void init_config(void);
void AT24C128_W(unsigned short addr,unsigned char dat);
unsigned char AT24C128_R(unsigned short addr);
void AT24C128_W_P(unsigned short addr,unsigned char *dat,unsigned char len);
void eepromtest(void);
void AT24C128_R_P(unsigned short addr,unsigned char *dat,unsigned char len);
void eepromwrite(void);
void eepromread(void);

void SaveZS(uint32_t time1);
uint32_t GetZSValue(void);
void Save_Test_LC(uint32_t ZPlus);
void Save_Ready_LC(uint32_t ZPlus);
uint32_t Read_Test_LC(void);
uint32_t Read_Ready_LC(void);
uint32_t GetZSValue(void);

void Set_Z_Initial_Position(uint32_t Z_Plus);
uint32_t Get_Z_Initial_Position(void);
uint32_t Get_Motor_Initial_Position(void);
#endif
