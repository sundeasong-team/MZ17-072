#include "RepeatHandle.h"
#include "math.h"
#include "stdio.h"
#include "delay.h"
#include "Uart.H"
#define SON 1
#define SOFF 0

#define REPEAT_TIMES  1							 	        //错误重发次数
unsigned char ServoBusyBit[MAX_ADRESS];				//伺服忙碌
unsigned char ServoPauseBit[MAX_ADRESS];			//伺服暂停
unsigned char ServoReadyBit[MAX_ADRESS];			//伺服控制器就绪
unsigned char ServoOriginBit[MAX_ADRESS];			//伺服原点  不能用它当作回原点完成的状态来判断
unsigned char ServoOnBit[MAX_ADRESS];			    //伺服ON    ServoPwrOnBit[MAX_ADRESS];
unsigned char ServoErrorBit[MAX_ADRESS];			//伺服报警
unsigned char ServoOrigin_reset[MAX_ADRESS];
unsigned char ServoOrigin_R[MAX_ADRESS];      //伺服到达自己定义的原点位置
unsigned char ServoOrigin_I[MAX_ADRESS];      //伺服到达初始位置
unsigned char ServoMeddle_M[MAX_ADRESS];      //伺服到达中间调试位置(提供给UP_N电机是否走完用)
unsigned char Servoendpoint_E[MAX_ADRESS];    //伺服到达端点位置
unsigned char Servoendpoint_HOME[MAX_ADRESS]; //伺服电机本身的原点位置
//=================main.c中定义的变量=======================
extern uint32_t Z_Test_LC;                        //电机压到2KG的高度
extern char print;                            //调机打印，监控
extern uint32_t Initial_Zaxis_Plus;           //PT治具初始位置
extern int  STEP;                             //UP_N 后的步数值
//==========================================================
int ServoCurrPosition[MAX_ADRESS];            //当前位置
int ServoTarPosition[MAX_ADRESS];             //目标位置
int ServoOriginPosition[MAX_ADRESS];          //原点位置
/*******************************************************************************
* Function Name  : 
* Description    : 伺服初始化
* Parameter		 : 1 or 2 or 3
* Return		 :  
********************************************************************************/
unsigned char ServoInitialize(unsigned char AdressNum)//AdressNum=1
{
	 unsigned char i=0;
   for(i=0;i<MAX_ADRESS;i++)	  //初始化All标志位
   {
		ServoBusyBit[i]=SOFF;			  //伺服忙碌SOFF=0
		ServoPauseBit[i]=SOFF;			//伺服暂停SOFF=0
		ServoOriginBit[i]=SOFF;			//伺服原点SOFF=0
		ServoErrorBit[i]=SOFF;			//伺服报警SOFF=0
		ServoOnBit[i]=SOFF;			    //伺服ON  SOFF=0
		ServoReadyBit[i]=SOFF;      //伺服控制器就绪
		ServoMeddle_M[i]=SOFF;      //伺服到达中间调试位置(提供给UP_N电机是否走完用)
   }
//    i=0;
//    while(i<REPEAT_TIMES)			  			              //命令模式开启 Tx_String[]={':','0','1','0','5','0','4','2','7','F','F','0','0','\0'};
//    {
//    		if(IAI_Set_OrderEnable(AdressNum) == 0)		  	//失败重发 收到:01050427FF00D0
// 		{	
// 			break;										                    //成功跳出最近一个循环
// 		}
// 		Delay_MS(10);
// 		i++;
//    }
//    if(i >= REPEAT_TIMES)	   return 1;

   i=0;
   while(i<REPEAT_TIMES)			  			             //清除报警Tx_String1[]={':','0','1','0','5','0','4','0','7','F','F','0','0','\0'};Tx_String1[]={':','0','1','0','5','0','4','0','7','0','0','0','0','\0'};
   {
   		if(Toyo_Set_ClearArm(AdressNum) == 0)		  	 //失败重发:01050407FF00F0 :010504070000EF
		{	
			break;										                   //成功跳出
		}
			Delay_MS(10);
		i++;
   }
   if( i >= REPEAT_TIMES )	   return 2;
     	
   i=0;
   while(i<REPEAT_TIMES)			  			             //伺服ON Tx_String[]={':','0','1','0','5','0','4','0','3','F','F','0','0','\0'};
   {
   		if(Toyo_Set_ServoON(AdressNum) == 0)		  		 //失败重发	:01050403FF00F4
		{			
			break;									                     //成功跳出
		}
			Delay_MS(10);
		i++;												
   }

   if( i >= REPEAT_TIMES )	   return 3;
   return 0;	
}

/*******************************************************************************
* Function Name  : 
* Description    : 伺服回原点
* Parameter		   : 1 or 2 or 3
* Return		     : 
********************************************************************************/
unsigned char ServoGoOrigin(unsigned char AdressNum)
{
	unsigned char i=0;

   while( i<REPEAT_TIMES )						          //回原点设置
   {
   	if(Toyo_Go_Origin(AdressNum) == 0)		  		  //失败重发	Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','3','\0'}
		{			                                    
			break;									                  //成功跳出
		}
	  Delay_MS(50);                               //发送多条中间加50ms延时
		i++;														
   }
   if( i >= REPEAT_TIMES )	   return 1;        //正常i=0
   return 0;
}
/*******************************************************************************
* Function Name  : 
* Description    : 伺服速度设置
* Parameter		 : 1 or 2 or 3
* Return		 : 
********************************************************************************/
unsigned char ServoSetSpeed(unsigned char AdressNum,unsigned int CSpeed)
{
   unsigned char i=0;
   while( i<REPEAT_TIMES )						            //运行速度设置
   {
   	if(Toyo_Set_Speed(AdressNum,CSpeed) == 0)		  //失败重发			
		{			
			break;                                      //成功跳出   		
		}
	  Delay_MS(10);
		i++;											
   }
   if( i >= REPEAT_TIMES )	   return 1; 
   return 0;
}
/*******************************************************************************
* Function Name  : 
* Description    : 伺服移动指定位置
* Parameter		   :  地址 位置
* Return		 : 
********************************************************************************/
unsigned char ServoGoAssignPoint(unsigned char AdressNum, int Point)
{
	 unsigned char i=0;
	 unsigned char ret =0;
   while( i<REPEAT_TIMES )		  						     //去指定位置
   {
		 	Delay_MS(20);
		  ServoTarPosition[AdressNum] = Point;
   		ret=Toyo_Go_AssignPoint(AdressNum, Point); 
   		if(ret== 0)										             //失败重发
			{			
				break;								    	             //成功跳出
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
	     RET=Toyo_Read_CurrentStatus(1);                //正常情况下返回RET=0x0f
		   if(RET != 0xffff)                              //0-1023
			 {       
				 if(RET & 0x01) 	ServoOriginBit[1]=1;		    //伺服原点			 
			 	 else	
					          ServoOriginBit[1]=0;              //回原点动作  			
				 
				 if(RET & 0x04) 	ServoReadyBit[1]=1;		      //伺服就绪 			 
			 	 else				ServoReadyBit[1]=0;	
				 
				 if(RET & 0x08) 	ServoOnBit[1]=1;			      //伺服ON 	 	 
			 	 else				ServoOnBit[1]=0;	
				 
			 }
			 Delay_MS(10);                                  //命令与下一条命令之间增加必要的延时
			 RET=Toyo_Read_ErrorCode(1);		                //读取错误代码
			 if(RET == 0)
				 ServoErrorBit[1] = 0;                        //正常为0
			 else
				 ServoErrorBit[1] = 1;                        //出错为1
			 
			 if(print==1){
				Uart_Printf("ServoOriginBit=%d\r\n,ServoReadyBit=%d\r\n",ServoOriginBit[1],ServoReadyBit[1]);
				Uart_Printf("ServoOnBit=%d\r\n,ServoErrorBit=%d\r\n",ServoOnBit[1],ServoErrorBit[1]);
			 }
			 if( (ServoOriginBit[1]==1)&&(ServoReadyBit[1]==1)&&(ServoOnBit[1]==1)&&(ServoErrorBit[1]==0) )
			 {
			  break;//正常跳出
			 }
   }	 
}
/*******************************************************************************
* Function Name  : 
* Description    : 定时发送命令 读取当前位置,报警代码,PCIO状态 
********************************************************************************/
void Time_Order_Handle(void) //实际没用到(主循环查询用)
{
    static unsigned char i=0;
    unsigned int RET;
		//================================================================
		for(i=1;i<MAX_ADRESS;i++)						    //轴一 二 三
		{	
			Delay_MS(10);			
			RET=Toyo_Read_CurrentStatus(i);//读取电机当前状态 正常为0x3098
	    if(RET != 0)
			 {
				 if(RET & 0x0008) 	ServoBusyBit[1]=1;			//伺服忙碌  BIT(3)	正常ServoBusyBit[1]=1	 
			 	 else				ServoBusyBit[1]=0;	

				 if(RET & 0x0010) 	ServoOriginBit[1]=1;		//伺服原点	BIT(4)	正常ServoOriginBit[1]=1	UP 5000也为1 代表回过原点		 
			 	 else				ServoOriginBit[1]=0;	
				 
				 if(RET & 0x0020) 	ServoPauseBit[1]=1;			//伺服暂停	BIT(5)	正常ServoPauseBit[1]=0		 
			 	 else				ServoPauseBit[1]=0;	
				 
				 if(RET & 0x0400) 	ServoErrorBit[1]=1;			//伺服报警	BIT(10)	正常ServoErrorBit[1]=0		 
			 	 else				ServoErrorBit[1]=0;	

				 if(RET & 0x1000) 	ServoOnBit[1]=1;			  //伺服ON 	  BIT(12) 正常ServoPwrOnBit[1]=1		 
			 	 else				ServoOnBit[1]=0;	
				 
				 if(RET & 0x2000) 	ServoReadyBit[1]=1;		  //伺服就绪 	BIT(13)正常情况下ServoReadyBit[1]=1 		 
			 	 else				ServoReadyBit[1]=0;	
			 }
			 Delay_MS(10);
			 ServoCurrPosition[i] = Toyo_Read_CurrentPoint(i);//在原点的时候读取当前位置为15,UP后为5000
			 if(print==1)
			 {
				 Uart_Printf("Servo1_CurrentPoint=%d \n",ServoCurrPosition[i]);//Servo1_CurrentPoint
				 Uart_Printf("ServoOriginBit[1]=%d \n",ServoOriginBit[1]);
			 }
			 if( (ServoCurrPosition[i]>=0)&&(ServoCurrPosition[i]<=2)){       //HOME点位置 
				 Servoendpoint_HOME[i]=1;
			 }
			 else
			 {
				 Servoendpoint_HOME[i]=0;                                           
			 }
			 if( (ServoCurrPosition[i]>=13)&&(ServoCurrPosition[i]<=17))      //原点时候走到15的位置 
			 {
				 ServoOrigin_R[i]=1;                                            //原点标记,电机在原点15位置  
			 }
			 else
			 {
				 ServoOrigin_R[i]=0;                                            //电机未在原点位置 UP后5000位置
			 }
			 if(print==1)
			 {
			 	Uart_Printf("ServoOrigin_R[1]=%d\n",ServoOrigin_R[1]);
			 }
			 if( (ServoCurrPosition[i]>=(Z_Test_LC-2))&&(ServoCurrPosition[i]<=Z_Test_LC))//电机执行"up"命令结束后置标示位为1 ServoCurrPosition[1]=15,STEPP=5000
			 {
				   Servoendpoint_E[i]=1;                                        //电机到达预设定位置 
			 }
			 else
			 {
				   Servoendpoint_E[i]=0;                                        //电机未到达预设定位置 在原点的时候Servoendpoint_E[1]=0;
			 }
		} 		
}
//============================================================================================
void Toyo_Read_Motor_LC_Status(unsigned char i){                         //i为AdressNum表示轴号
	 Delay_MS(20);                                                    //增加必要的延时
   ServoCurrPosition[i] = Toyo_Read_CurrentPoint(i);
	 if( (ServoCurrPosition[i]>=0)&&(ServoCurrPosition[i]<=2)){       //HOME点位置 
		   Servoendpoint_HOME[1]=1;
	 }
	 else
	 {
		   Servoendpoint_HOME[1]=0;                                           
	 }
	 
	 if( (ServoCurrPosition[i]>=13)&&(ServoCurrPosition[i]<=17))      //原点时候走到15的位置 
	 {
		   ServoOrigin_R[1]=1;                                          //原点标记,电机在原点15位置  
	 }
	 else
	 {
		   ServoOrigin_R[1]=0;                                          //电机未在原点位置 
	 }
	 
	 if( (ServoCurrPosition[i]>=(Initial_Zaxis_Plus-2))&&(ServoCurrPosition[i]<=Initial_Zaxis_Plus+2))//电机走到初始位置Initial_Zaxis_Plus
	 {
			 ServoOrigin_I[1]=1;                                          //电机走到初始位置Initial_Zaxis_Plus  
	 }
	 else
	 {
			 ServoOrigin_I[1]=0;                                          //电机未走到初始位置Initial_Zaxis_Plus
	 }
	 
	 if( (ServoCurrPosition[i]>=(STEP-2))&&(ServoCurrPosition[i]<=STEP+2)) //STEP表示READY位置
	 {
			 ServoMeddle_M[1]=1;                                          //伺服到达READY位置(提供给UP_N电机是否走完用)
	 }
	 else
	 {
			 ServoMeddle_M[1]=0;                                       
	 }
	 
	 if( (ServoCurrPosition[i]>=(Z_Test_LC-2))&&(ServoCurrPosition[i]<=Z_Test_LC+2))//电机执行"up"命令结束后置标示位为1 ServoCurrPosition[1]=15,STEPP=5000
	 {
			 Servoendpoint_E[1]=1;                                        //电机到达预设定位置 
	 }
	 else
	 {
			 Servoendpoint_E[1]=0;                                        //电机未到达预设定位置 在原点的时候Servoendpoint_E[1]=0;
	 }

}
//*******************************************************************************
//计算绝对值
//*******************************************************************************
int Iabs(int val)
{
	int abs = val;
	if(abs>0)
		return abs;
	else
		return -(abs);
}

