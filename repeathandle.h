#ifndef ___REPEATHANDLE_H
#define ___REPEATHANDLE_H

#include "ToyoControl.h" 
#define MAX_ADRESS 2	

void Time_Order_Handle(void); 									  					                  //定时命令处理函数
void read_home_status(void);
unsigned char ServoInitialize(unsigned char AdressNum);			  					      //伺服初始话函数
unsigned char ServoGoOrigin(unsigned char AdressNum);			  					        //伺服回原点函数
unsigned char ServoGoAssignPoint(unsigned char AdressNum,int Point);	        //伺服去指定位置
unsigned char ServoSetSpeed(unsigned char AdressNum,unsigned int CSpeed);			//伺服全局速度设置
unsigned char Get_Send_WData(unsigned char AdressNum,unsigned char *WData);
void Toyo_Read_Motor_LC_Status(unsigned char i);
int Iabs(int val);

extern unsigned char ServoReadyBit[MAX_ADRESS];				//伺服控制器就绪
extern unsigned char ServoPauseBit[MAX_ADRESS];				//伺服暂停
extern unsigned char ServoOriginBit[MAX_ADRESS];			//伺服原点
extern unsigned char ServoErrorBit[MAX_ADRESS];				//伺服报警	  
extern unsigned char ServoOrigin_reset[MAX_ADRESS];
extern unsigned char ServoOrigin_R[MAX_ADRESS];       //伺服到达自己定义的原点位置
extern unsigned char ServoOrigin_I[MAX_ADRESS];       //伺服到达初始位置
extern unsigned char ServoMeddle_M[MAX_ADRESS];       //伺服到达中间调试位置(提供给UP_N电机是否走完用)
extern unsigned char Servoendpoint_E[MAX_ADRESS];     //伺服到达端点位置
extern unsigned char Servoendpoint_HOME[MAX_ADRESS];  //伺服电机本身的原点位置
extern int ServoCurrPosition[MAX_ADRESS];
extern int ServoTarPosition[MAX_ADRESS];
extern int ServoOriginPosition[MAX_ADRESS];

#endif 
