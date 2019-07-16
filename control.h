#ifndef __Control_H_
#define __Control_H_
//=======================================
void key_scan(void);
void Help(void);
void fixture_init(void);
void sensor_check(void);
//=======================================
void  VACON(void);
void  VACOFF(void);
void  LEDON(void);
void  LEDOFF(void);
void  PUSHON(void);
void  PUSHOFF(void);
void  HOLDERUP(void);
void  HOLDERDOWN(void);
void  HOLDERON(void);
void  HOLDEROFF(void);
void  GO_HOME(void);
void  CM_PW_ON(void);
void  CM_PW_OFF(void);
void  ION_ON(void);
void  ION_OFF(void);
void  BLOW_MIRRORON(void);
void  BLOW_MIRROROFF(void);
void reset(uint8_t f);
void ver(void);
unsigned char Writeid(char *str);
void UnitID(uint8_t f);//uint32_t 
void Help(void);
void PadUP(void);
void PadDown(void);
int  GetStringNum( char *cmd);
unsigned char SetZS(char *str);
uint32_t GetZS(void);

void Set_Motor_Initial_Position(uint32_t Z_Plus);
void  Cylinder1_On(void);
void  Cylinder1_Off(void);
void  Cylinder2_On(void);
void  Cylinder2_Off(void);
void  Cylinder3_On(void);
void  Cylinder3_Off(void);
void  Cylinder4_On(void);
void  Cylinder4_Off(void);
void  Cylinder5_On(void);
void  Cylinder5_Off(void);
void  Cylinder6_On(void);
void  Cylinder6_Off(void);
void  Cylinder7_On(void);
void  Cylinder7_Off(void);
void  Cylinder8_On(void);
void  Cylinder8_Off(void);
void  Cylinder9_On(void);
void  Cylinder9_Off(void);
void  Cylinder10_On(void);
void  Cylinder10_Off(void);
void  Cylinder11_On(void);
void  Cylinder11_Off(void);
void  Cylinder12_On(void);
void  Cylinder12_Off(void);
void  Cylinder13_On(void);
void  Cylinder13_Off(void);
void  Cylinder14_On(void);
void  Cylinder14_Off(void);
void  Cylinder15_On(void);
void  Cylinder15_Off(void);
void  Cylinder16_On(void);
void  Cylinder16_Off(void);
unsigned char set_fw_number(char *str);
unsigned char set_hw_number(char *str);
unsigned char set_vtorch_fw_number(char *str);
void  Up_N(uint32_t tempValue);
void  Down_N(uint32_t tempValue);
void  Motor_init(void);
void  Motor_Test(void);
void  Move_Test_LC(void);
void  Move_Ready_LC(void);
#endif
