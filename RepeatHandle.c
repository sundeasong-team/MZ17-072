#include "RepeatHandle.h"
#include "math.h"
#include "stdio.h"
#include "delay.h"
#include "Uart.H"
#define SON 1
#define SOFF 0

#define REPEAT_TIMES  1							 	        //�����ط�����
unsigned char ServoBusyBit[MAX_ADRESS];				//�ŷ�æµ
unsigned char ServoPauseBit[MAX_ADRESS];			//�ŷ���ͣ
unsigned char ServoReadyBit[MAX_ADRESS];			//�ŷ�����������
unsigned char ServoOriginBit[MAX_ADRESS];			//�ŷ�ԭ��  ��������������ԭ����ɵ�״̬���ж�
unsigned char ServoOnBit[MAX_ADRESS];			    //�ŷ�ON    ServoPwrOnBit[MAX_ADRESS];
unsigned char ServoErrorBit[MAX_ADRESS];			//�ŷ�����
unsigned char ServoOrigin_reset[MAX_ADRESS];
unsigned char ServoOrigin_R[MAX_ADRESS];      //�ŷ������Լ������ԭ��λ��
unsigned char ServoOrigin_I[MAX_ADRESS];      //�ŷ������ʼλ��
unsigned char ServoMeddle_M[MAX_ADRESS];      //�ŷ������м����λ��(�ṩ��UP_N����Ƿ�������)
unsigned char Servoendpoint_E[MAX_ADRESS];    //�ŷ�����˵�λ��
unsigned char Servoendpoint_HOME[MAX_ADRESS]; //�ŷ���������ԭ��λ��
//=================main.c�ж���ı���=======================
extern uint32_t Z_Test_LC;                        //���ѹ��2KG�ĸ߶�
extern char print;                            //������ӡ�����
extern uint32_t Initial_Zaxis_Plus;           //PT�ξ߳�ʼλ��
extern int  STEP;                             //UP_N ��Ĳ���ֵ
//==========================================================
int ServoCurrPosition[MAX_ADRESS];            //��ǰλ��
int ServoTarPosition[MAX_ADRESS];             //Ŀ��λ��
int ServoOriginPosition[MAX_ADRESS];          //ԭ��λ��
/*******************************************************************************
* Function Name  : 
* Description    : �ŷ���ʼ��
* Parameter		 : 1 or 2 or 3
* Return		 :  
********************************************************************************/
unsigned char ServoInitialize(unsigned char AdressNum)//AdressNum=1
{
	 unsigned char i=0;
   for(i=0;i<MAX_ADRESS;i++)	  //��ʼ��All��־λ
   {
		ServoBusyBit[i]=SOFF;			  //�ŷ�æµSOFF=0
		ServoPauseBit[i]=SOFF;			//�ŷ���ͣSOFF=0
		ServoOriginBit[i]=SOFF;			//�ŷ�ԭ��SOFF=0
		ServoErrorBit[i]=SOFF;			//�ŷ�����SOFF=0
		ServoOnBit[i]=SOFF;			    //�ŷ�ON  SOFF=0
		ServoReadyBit[i]=SOFF;      //�ŷ�����������
		ServoMeddle_M[i]=SOFF;      //�ŷ������м����λ��(�ṩ��UP_N����Ƿ�������)
   }
//    i=0;
//    while(i<REPEAT_TIMES)			  			              //����ģʽ���� Tx_String[]={':','0','1','0','5','0','4','2','7','F','F','0','0','\0'};
//    {
//    		if(IAI_Set_OrderEnable(AdressNum) == 0)		  	//ʧ���ط� �յ�:01050427FF00D0
// 		{	
// 			break;										                    //�ɹ��������һ��ѭ��
// 		}
// 		Delay_MS(10);
// 		i++;
//    }
//    if(i >= REPEAT_TIMES)	   return 1;

   i=0;
   while(i<REPEAT_TIMES)			  			             //�������Tx_String1[]={':','0','1','0','5','0','4','0','7','F','F','0','0','\0'};Tx_String1[]={':','0','1','0','5','0','4','0','7','0','0','0','0','\0'};
   {
   		if(Toyo_Set_ClearArm(AdressNum) == 0)		  	 //ʧ���ط�:01050407FF00F0 :010504070000EF
		{	
			break;										                   //�ɹ�����
		}
			Delay_MS(10);
		i++;
   }
   if( i >= REPEAT_TIMES )	   return 2;
     	
   i=0;
   while(i<REPEAT_TIMES)			  			             //�ŷ�ON Tx_String[]={':','0','1','0','5','0','4','0','3','F','F','0','0','\0'};
   {
   		if(Toyo_Set_ServoON(AdressNum) == 0)		  		 //ʧ���ط�	:01050403FF00F4
		{			
			break;									                     //�ɹ�����
		}
			Delay_MS(10);
		i++;												
   }

   if( i >= REPEAT_TIMES )	   return 3;
   return 0;	
}

/*******************************************************************************
* Function Name  : 
* Description    : �ŷ���ԭ��
* Parameter		   : 1 or 2 or 3
* Return		     : 
********************************************************************************/
unsigned char ServoGoOrigin(unsigned char AdressNum)
{
	unsigned char i=0;

   while( i<REPEAT_TIMES )						          //��ԭ������
   {
   	if(Toyo_Go_Origin(AdressNum) == 0)		  		  //ʧ���ط�	Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','3','\0'}
		{			                                    
			break;									                  //�ɹ�����
		}
	  Delay_MS(50);                               //���Ͷ����м��50ms��ʱ
		i++;														
   }
   if( i >= REPEAT_TIMES )	   return 1;        //����i=0
   return 0;
}
/*******************************************************************************
* Function Name  : 
* Description    : �ŷ��ٶ�����
* Parameter		 : 1 or 2 or 3
* Return		 : 
********************************************************************************/
unsigned char ServoSetSpeed(unsigned char AdressNum,unsigned int CSpeed)
{
   unsigned char i=0;
   while( i<REPEAT_TIMES )						            //�����ٶ�����
   {
   	if(Toyo_Set_Speed(AdressNum,CSpeed) == 0)		  //ʧ���ط�			
		{			
			break;                                      //�ɹ�����   		
		}
	  Delay_MS(10);
		i++;											
   }
   if( i >= REPEAT_TIMES )	   return 1; 
   return 0;
}
/*******************************************************************************
* Function Name  : 
* Description    : �ŷ��ƶ�ָ��λ��
* Parameter		   :  ��ַ λ��
* Return		 : 
********************************************************************************/
unsigned char ServoGoAssignPoint(unsigned char AdressNum, int Point)
{
	 unsigned char i=0;
	 unsigned char ret =0;
   while( i<REPEAT_TIMES )		  						     //ȥָ��λ��
   {
		 	Delay_MS(20);
		  ServoTarPosition[AdressNum] = Point;
   		ret=Toyo_Go_AssignPoint(AdressNum, Point); 
   		if(ret== 0)										             //ʧ���ط�
			{			
				break;								    	             //�ɹ�����
			}
			i++;																		
   }
   if( i >= REPEAT_TIMES )	   return ret; 
   return 0;
}

void read_home_status(void)
{
	unsigned int RET;
	while(1)
	{
	     RET=Toyo_Read_CurrentStatus(1);                //��������·���RET=0x0f
		   if(RET != 0xffff)                              //0-1023
			 {       
				 if(RET & 0x01) 	ServoOriginBit[1]=1;		    //�ŷ�ԭ��			 
			 	 else	
					          ServoOriginBit[1]=0;              //��ԭ�㶯��  			
				 
				 if(RET & 0x04) 	ServoReadyBit[1]=1;		      //�ŷ����� 			 
			 	 else				ServoReadyBit[1]=0;	
				 
				 if(RET & 0x08) 	ServoOnBit[1]=1;			      //�ŷ�ON 	 	 
			 	 else				ServoOnBit[1]=0;	
				 
			 }
			 Delay_MS(10);                                  //��������һ������֮�����ӱ�Ҫ����ʱ
			 RET=Toyo_Read_ErrorCode(1);		                //��ȡ�������
			 if(RET == 0)
				 ServoErrorBit[1] = 0;                        //����Ϊ0
			 else
				 ServoErrorBit[1] = 1;                        //����Ϊ1
			 
			 if(print==1){
				Uart_Printf("ServoOriginBit=%d\r\n,ServoReadyBit=%d\r\n",ServoOriginBit[1],ServoReadyBit[1]);
				Uart_Printf("ServoOnBit=%d\r\n,ServoErrorBit=%d\r\n",ServoOnBit[1],ServoErrorBit[1]);
			 }
			 if( (ServoOriginBit[1]==1)&&(ServoReadyBit[1]==1)&&(ServoOnBit[1]==1)&&(ServoErrorBit[1]==0) )
			 {
			  break;//��������
			 }
   }	 
}
/*******************************************************************************
* Function Name  : 
* Description    : ��ʱ�������� ��ȡ��ǰλ��,��������,PCIO״̬ 
********************************************************************************/
void Time_Order_Handle(void) //ʵ��û�õ�(��ѭ����ѯ��)
{
    static unsigned char i=0;
    unsigned int RET;
		//================================================================
		for(i=1;i<MAX_ADRESS;i++)						    //��һ �� ��
		{	
			Delay_MS(10);			
			RET=Toyo_Read_CurrentStatus(i);//��ȡ�����ǰ״̬ ����Ϊ0x3098
	    if(RET != 0)
			 {
				 if(RET & 0x0008) 	ServoBusyBit[1]=1;			//�ŷ�æµ  BIT(3)	����ServoBusyBit[1]=1	 
			 	 else				ServoBusyBit[1]=0;	

				 if(RET & 0x0010) 	ServoOriginBit[1]=1;		//�ŷ�ԭ��	BIT(4)	����ServoOriginBit[1]=1	UP 5000ҲΪ1 ����ع�ԭ��		 
			 	 else				ServoOriginBit[1]=0;	
				 
				 if(RET & 0x0020) 	ServoPauseBit[1]=1;			//�ŷ���ͣ	BIT(5)	����ServoPauseBit[1]=0		 
			 	 else				ServoPauseBit[1]=0;	
				 
				 if(RET & 0x0400) 	ServoErrorBit[1]=1;			//�ŷ�����	BIT(10)	����ServoErrorBit[1]=0		 
			 	 else				ServoErrorBit[1]=0;	

				 if(RET & 0x1000) 	ServoOnBit[1]=1;			  //�ŷ�ON 	  BIT(12) ����ServoPwrOnBit[1]=1		 
			 	 else				ServoOnBit[1]=0;	
				 
				 if(RET & 0x2000) 	ServoReadyBit[1]=1;		  //�ŷ����� 	BIT(13)���������ServoReadyBit[1]=1 		 
			 	 else				ServoReadyBit[1]=0;	
			 }
			 Delay_MS(10);
			 ServoCurrPosition[i] = Toyo_Read_CurrentPoint(i);//��ԭ���ʱ���ȡ��ǰλ��Ϊ15,UP��Ϊ5000
			 if(print==1)
			 {
				 Uart_Printf("Servo1_CurrentPoint=%d \n",ServoCurrPosition[i]);//Servo1_CurrentPoint
				 Uart_Printf("ServoOriginBit[1]=%d \n",ServoOriginBit[1]);
			 }
			 if( (ServoCurrPosition[i]>=0)&&(ServoCurrPosition[i]<=2)){       //HOME��λ�� 
				 Servoendpoint_HOME[i]=1;
			 }
			 else
			 {
				 Servoendpoint_HOME[i]=0;                                           
			 }
			 if( (ServoCurrPosition[i]>=13)&&(ServoCurrPosition[i]<=17))      //ԭ��ʱ���ߵ�15��λ�� 
			 {
				 ServoOrigin_R[i]=1;                                            //ԭ����,�����ԭ��15λ��  
			 }
			 else
			 {
				 ServoOrigin_R[i]=0;                                            //���δ��ԭ��λ�� UP��5000λ��
			 }
			 if(print==1)
			 {
			 	Uart_Printf("ServoOrigin_R[1]=%d\n",ServoOrigin_R[1]);
			 }
			 if( (ServoCurrPosition[i]>=(Z_Test_LC-2))&&(ServoCurrPosition[i]<=Z_Test_LC))//���ִ��"up"����������ñ�ʾλΪ1 ServoCurrPosition[1]=15,STEPP=5000
			 {
				   Servoendpoint_E[i]=1;                                        //�������Ԥ�趨λ�� 
			 }
			 else
			 {
				   Servoendpoint_E[i]=0;                                        //���δ����Ԥ�趨λ�� ��ԭ���ʱ��Servoendpoint_E[1]=0;
			 }
		} 		
}
//============================================================================================
void Toyo_Read_Motor_LC_Status(unsigned char i){                         //iΪAdressNum��ʾ���
	 Delay_MS(20);                                                    //���ӱ�Ҫ����ʱ
   ServoCurrPosition[i] = Toyo_Read_CurrentPoint(i);
	 if( (ServoCurrPosition[i]>=0)&&(ServoCurrPosition[i]<=2)){       //HOME��λ�� 
		   Servoendpoint_HOME[1]=1;
	 }
	 else
	 {
		   Servoendpoint_HOME[1]=0;                                           
	 }
	 
	 if( (ServoCurrPosition[i]>=13)&&(ServoCurrPosition[i]<=17))      //ԭ��ʱ���ߵ�15��λ�� 
	 {
		   ServoOrigin_R[1]=1;                                          //ԭ����,�����ԭ��15λ��  
	 }
	 else
	 {
		   ServoOrigin_R[1]=0;                                          //���δ��ԭ��λ�� 
	 }
	 
	 if( (ServoCurrPosition[i]>=(Initial_Zaxis_Plus-2))&&(ServoCurrPosition[i]<=Initial_Zaxis_Plus+2))//����ߵ���ʼλ��Initial_Zaxis_Plus
	 {
			 ServoOrigin_I[1]=1;                                          //����ߵ���ʼλ��Initial_Zaxis_Plus  
	 }
	 else
	 {
			 ServoOrigin_I[1]=0;                                          //���δ�ߵ���ʼλ��Initial_Zaxis_Plus
	 }
	 
	 if( (ServoCurrPosition[i]>=(STEP-2))&&(ServoCurrPosition[i]<=STEP+2)) //STEP��ʾREADYλ��
	 {
			 ServoMeddle_M[1]=1;                                          //�ŷ�����READYλ��(�ṩ��UP_N����Ƿ�������)
	 }
	 else
	 {
			 ServoMeddle_M[1]=0;                                       
	 }
	 
	 if( (ServoCurrPosition[i]>=(Z_Test_LC-2))&&(ServoCurrPosition[i]<=Z_Test_LC+2))//���ִ��"up"����������ñ�ʾλΪ1 ServoCurrPosition[1]=15,STEPP=5000
	 {
			 Servoendpoint_E[1]=1;                                        //�������Ԥ�趨λ�� 
	 }
	 else
	 {
			 Servoendpoint_E[1]=0;                                        //���δ����Ԥ�趨λ�� ��ԭ���ʱ��Servoendpoint_E[1]=0;
	 }

}
//*******************************************************************************
//�������ֵ
//*******************************************************************************
int Iabs(int val)
{
	int abs = val;
	if(abs>0)
		return abs;
	else
		return -(abs);
}

