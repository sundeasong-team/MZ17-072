#ifndef __TOYOCONTROL_H
#define __TOYOCONTROL_H
#define MAX_ADRESS 2	   							//最多3个轴	 1，2，3（不含0）
//#include "static.h" 
extern unsigned char Rx_Flag;
extern unsigned char Rx_String[100];	//接受到的字符串 去除lrc校验及回车换行	extern

// unsigned char IAI_Set_OrderEnable(unsigned char AdressNum);
unsigned char Toyo_Set_ClearArm(unsigned char AdressNum);
unsigned char Toyo_Set_ServoON(unsigned char AdressNum);
unsigned char Toyo_Set_ServoOFF(unsigned char AdressNum);
unsigned char Toyo_Go_Origin(unsigned char AdressNum);
unsigned char Toyo_Set_Speed(unsigned char AdressNum,unsigned char CSpeed );
// unsigned char IAI_Set_Pause(unsigned char AdressNum);
// unsigned char IAI_Go_Zero(unsigned char AdressNum);
unsigned char Toyo_Go_AssignPoint(unsigned char AdressNum,unsigned int Point);

unsigned int Toyo_Read_CurrentStatus(unsigned char AdressNum);
//unsigned int Read_CurrentPoint(unsigned char AdressNum);
         int Toyo_Read_CurrentPoint(unsigned char AdressNum);
unsigned int Toyo_Read_ErrorCode(unsigned char AdressNum);

	
unsigned char Get_Send_Command(unsigned char AdressNum,unsigned char *Command);	
unsigned char Get_Send_Flag(unsigned char AdressNum,unsigned char *Command);
void Clear_Rx_String(void);
unsigned char StringCompare(unsigned char *Command);
unsigned char ChrToInt(char chr);
#endif /* __END_*/
