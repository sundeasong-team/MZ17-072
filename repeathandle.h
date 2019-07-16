#ifndef ___REPEATHANDLE_H
#define ___REPEATHANDLE_H

#include "ToyoControl.h" 
#define MAX_ADRESS 2	

void Time_Order_Handle(void); 									  					                  //��ʱ�������
void read_home_status(void);
unsigned char ServoInitialize(unsigned char AdressNum);			  					      //�ŷ���ʼ������
unsigned char ServoGoOrigin(unsigned char AdressNum);			  					        //�ŷ���ԭ�㺯��
unsigned char ServoGoAssignPoint(unsigned char AdressNum,int Point);	        //�ŷ�ȥָ��λ��
unsigned char ServoSetSpeed(unsigned char AdressNum,unsigned int CSpeed);			//�ŷ�ȫ���ٶ�����
unsigned char Get_Send_WData(unsigned char AdressNum,unsigned char *WData);
void Toyo_Read_Motor_LC_Status(unsigned char i);
int Iabs(int val);

extern unsigned char ServoReadyBit[MAX_ADRESS];				//�ŷ�����������
extern unsigned char ServoPauseBit[MAX_ADRESS];				//�ŷ���ͣ
extern unsigned char ServoOriginBit[MAX_ADRESS];			//�ŷ�ԭ��
extern unsigned char ServoErrorBit[MAX_ADRESS];				//�ŷ�����	  
extern unsigned char ServoOrigin_reset[MAX_ADRESS];
extern unsigned char ServoOrigin_R[MAX_ADRESS];       //�ŷ������Լ������ԭ��λ��
extern unsigned char ServoOrigin_I[MAX_ADRESS];       //�ŷ������ʼλ��
extern unsigned char ServoMeddle_M[MAX_ADRESS];       //�ŷ������м����λ��(�ṩ��UP_N����Ƿ�������)
extern unsigned char Servoendpoint_E[MAX_ADRESS];     //�ŷ�����˵�λ��
extern unsigned char Servoendpoint_HOME[MAX_ADRESS];  //�ŷ���������ԭ��λ��
extern int ServoCurrPosition[MAX_ADRESS];
extern int ServoTarPosition[MAX_ADRESS];
extern int ServoOriginPosition[MAX_ADRESS];

#endif 
