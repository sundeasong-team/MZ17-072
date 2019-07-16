#include "lpc17xx.h"
#include <string.h>
#include "control.h"
#include "Uart.h"
#include "Uart1.h"
#include "Uart3.h"
#include "io.h"
#include "24c128.h"
#include "delay.h"
#include "repeathandle.h"
#include "PCA9655E.H"
uint8_t  debug_run_flag;
uint8_t  Ion_flag=0;
extern unsigned long hw_date;
extern unsigned char hw_ver_shi;
extern unsigned char hw_ver_ge;
extern unsigned long fw_date;
extern unsigned char fw_ver_shi;
extern unsigned char fw_ver_ge;
extern char StrID[30];//拼接TESTERID用
extern uint32_t Z_Test_LC; //获取EEPROM 
extern uint32_t Z_Ready_LC;//获取EEPROM 
extern int STEP;       //中间变量UP_N 后的脉冲高度
extern char print;     //是否打印标志

//========================== 字符转换函数==============
void CharConvert(char *str)
{
		while((*str)!='\0')
		{
				if(((*str)>='a') &&((*str)<='z')) 
				{
						*str=*str-32;
				}
				str++;
		}
}
//=====================================================
int8_t Check( char *ptr){
   uint32_t tempValue=0;	
	 CharConvert(ptr);
   //=========================================================================================================
				if(strcmp("TESTERID",ptr) == 0)		    {UnitID(1);return 0;}
   else if(strcmp("EEPROM WRITE",ptr)==0)     {eepromwrite();return 0;}//eeprom write	
   else if(strcmp("EEPROM READ",ptr)==0)      {eepromread();return 0;}//eeprom read
   else if(strcmp("SENSOR_CHECK",ptr)==0)     {sensor_check();return 0;} 
	 else if(strcmp("CY1ON",ptr)==0)            {Cylinder1_On();return 0;}//Y轴推入气缸
	 else if(strcmp("CY1OFF",ptr)==0)           {Cylinder1_Off();return 0;}//Y轴推入气缸 
	 else if(strcmp("CY2ON",ptr)==0)            {Cylinder2_On();return 0;} //Z轴上顶气缸
	 else if(strcmp("CY2OFF",ptr)==0)           {Cylinder2_Off();return 0;}//Z轴上顶气缸 
	 else if(strcmp("CY3ON",ptr)==0)            {Cylinder3_On();return 0;} //VACUUM
	 else if(strcmp("CY3OFF",ptr)==0)           {Cylinder3_Off();return 0;}//VACUUM 
	 else if(strcmp("CY4ON",ptr)==0)            {Cylinder4_On();return 0;} //USB SWITCH
	 else if(strcmp("CY4OFF",ptr)==0)           {Cylinder4_Off();return 0;}//USB SWITCH
	 else if(strcmp("CY5ON",ptr)==0)            {Cylinder5_On();return 0;} //LED
	 else if(strcmp("CY5OFF",ptr)==0)           {Cylinder5_Off();return 0;}//LED 
	 else if(strcmp("CY6ON",ptr)==0)            {Cylinder6_On();return 0;} //预留
	 else if(strcmp("CY6OFF",ptr)==0)           {Cylinder6_Off();return 0;}//预留
   else if(strcmp("CY7ON",ptr)==0)            {Cylinder7_On();return 0;} //Y轴推入气缸
	 else if(strcmp("CY7OFF",ptr)==0)           {Cylinder7_Off();return 0;}//Y轴推入气缸
   else if(strcmp("CY8ON",ptr)==0)            {Cylinder8_On();return 0;} //Z轴上顶气缸
	 else if(strcmp("CY8OFF",ptr)==0)           {Cylinder8_Off();return 0;}//Z轴上顶气缸
	 else if(strcmp("CY9ON",ptr)==0)            {Cylinder9_On();return 0;} 
	 else if(strcmp("CY9OFF",ptr)==0)           {Cylinder9_Off();return 0;}
	 else if(strcmp("CY10ON",ptr)==0)           {Cylinder10_On();return 0;} 
	 else if(strcmp("CY10OFF",ptr)==0)          {Cylinder10_Off();return 0;}
	 else if(strcmp("CY11ON",ptr)==0)           {PCA9655E_OUT(1,2,1);return 0;}
	 else if(strcmp("CY11OFF",ptr)==0)          {PCA9655E_OUT(1,2,0);return 0;}	 
	 else if(strcmp("CY12ON",ptr)==0)           {PCA9655E_OUT(1,3,1);return 0;}
	 else if(strcmp("CY12OFF",ptr)==0)          {PCA9655E_OUT(1,3,0);return 0;} 
	 else if(strcmp("CY13ON",ptr)==0)           {PCA9655E_OUT(1,4,1);return 0;}
	 else if(strcmp("CY13OFF",ptr)==0)          {PCA9655E_OUT(1,4,0);return 0;}
	 else if(strcmp("CY14ON",ptr)==0)           {PCA9655E_OUT(1,5,1);return 0;}
	 else if(strcmp("CY14OFF",ptr)==0)          {PCA9655E_OUT(1,5,0);return 0;} 
	 else if(strcmp("CY15ON",ptr)==0)           {PCA9655E_OUT(1,6,1);return 0;}
	 else if(strcmp("CY15OFF",ptr)==0)          {PCA9655E_OUT(1,6,0);return 0;}
	 else if(strcmp("CY16ON",ptr)==0)           {PCA9655E_OUT(1,7,1);return 0;}
	 else if(strcmp("CY16OFF",ptr)==0)          {PCA9655E_OUT(1,7,0);return 0;}

	 else if(strcmp("VACON",ptr) == 0) 	  	    {VACON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//打开真空
	 else if(strcmp("VACOFF",ptr) == 0) 	 	    {VACOFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//关闭真空
	 else if(strcmp("LEDON",ptr) == 0) 	  	    {LEDON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//打开LED
	 else if(strcmp("LEDOFF",ptr) == 0) 	 	    {LEDOFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//关闭LED
	 else if(strcmp("PUSHON",ptr) == 0) 	  	  {PUSHON();Print_String("@_@\r\n");return 0;}//Y轴大气缸推进,双联电磁阀
	 else if(strcmp("PUSHOFF",ptr) == 0) 	 	    {PUSHOFF();Print_String("@_@\r\n");return 0;}//Y轴大气缸推出，双联电磁阀
	 else if(strcmp("HOLDERUP",ptr) == 0) 	    {HOLDERUP();Print_String("@_@\r\n");return 0;}//holder气缸上升，双联电磁阀
	 else if(strcmp("HOLDERDOWN",ptr) == 0) 	  {HOLDERDOWN();Print_String("@_@\r\n");return 0;}//holder气缸下降，双联电磁阀
	 else if(strcmp("HOLDERON",ptr) == 0) 	    {HOLDERON();Print_String("@_@\r\n");return 0;}//holder前进，双联电磁阀
	 else if(strcmp("HOLDEROFF",ptr) == 0) 	    {HOLDEROFF();Print_String("@_@\r\n");return 0;}//holder退回，双联电磁阀
	 
	 else if(strcmp("CAMERAPOWERON",ptr) == 0)  {CM_PW_ON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//相机电源on
	 else if(strcmp("CAMERAPOWEROFF",ptr) == 0) {CM_PW_OFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//相机电源off	

	 else if(strcmp("BLOW_MIRRORON",ptr) == 0)  	{BLOW_MIRRORON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
	 else if(strcmp("BLOW_MIRROROFF",ptr) == 0) 	{BLOW_MIRROROFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}	 
	 else if(strcmp("ION_ALL_OPEN",ptr) == 0)  	{Ion_flag=1;Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
	 else if(strcmp("ION_NORMAL",ptr) == 0) 		{Ion_flag=0;Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
     else if(strcmp("IONON",ptr) == 0)  				{ION_ON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//离子open
     else if(strcmp("IONOFF",ptr) == 0) 				{ION_OFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//离子close 	 
     else if(strcmp("GO_HOME",ptr) == 0) 				{GO_HOME();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//前往测试位置
	  //motor	
// 		else if(strcmp("STIMPADDIS",ptr) == 0)		{	                       //远离产品	
// 																								if(debug_run_flag==0){ //正常测试debug_run_flag=0
// 																								 PadDown();            //电机远离产品
// 																								}
// 																					      return 0;
// 		                                          } 
// 		
// 		else if(strcmp("STIMPADEN",ptr) == 0)		  {                        //靠近产品
// 																							  if(debug_run_flag==0){ //正常测试debug_run_flag=0
// 																							   PadUP();              //电机靠近产品
// 																						    }
// 																						    return 0;
// 												                      }
		else if(strcmp("RESET",ptr) == 0) 	 	    {reset(1);Print_String("@_@\r\n");return 0;}
//		else if(set_vtorch_fw_number(ptr)==0)     {Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}        //设置FW版本号
		else if(strcmp("FWVER",ptr) == 0) 	 	    {ver();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}  //读取FW版本号
		else if(strcmp("HELP",ptr) == 0)          {Help();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;} 				
		else if(SetZS(ptr)==0)                    {Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}        //设置电机速度  
		else if(strcmp("GETZS",ptr)==0)           {GetZS();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//获取电机速度
		else if(strncmp("UP_",ptr,3) == 0)		    {tempValue=GetStringNum(ptr);Up_N(tempValue);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
		
    else if(strcmp("SAVE TEST LC",ptr) == 0)  {Z_Test_LC=STEP;Save_Test_LC(Z_Test_LC);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                                                          //保存z轴测试位置
		else if(strcmp("READ TEST LC",ptr) == 0)  {Z_Test_LC=Read_Test_LC();Uart_Printf("Z_Test_LC=%d\r\n",Z_Test_LC);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}          //读取z轴设定测试位置	
    else if(strcmp("SAVE READY LC",ptr) == 0) {Save_Ready_LC(STEP);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                                                         //保存z轴预备位置
		else if(strcmp("READ READY LC",ptr)==0)		{Z_Ready_LC=Read_Ready_LC();Uart_Printf("Z_Ready_LC=%d\r\n",Z_Ready_LC);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}      //读取z轴预备位置	
	
		else if(strncmp("SETMIP_",ptr,7)==0)		  {tempValue=GetStringNum(ptr);   Set_Motor_Initial_Position(tempValue);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//设置PT FT电机初始位置
		else if(strcmp("GETMIP",ptr)==0)		      {Get_Motor_Initial_Position();  Print_String("OK\r\n");Print_String("@_@\r\n");return 0;} //for PT FT 读取电机初始位置
		
		else if(strcmp("MOVE TEST LC",ptr) == 0)  {Move_Test_LC(); Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//移动到测试位置                                
		else if(strcmp("MOVE READY LC",ptr) == 0) {Move_Ready_LC();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//移动到预备位置
		//========================================================================================
		else if(strcmp("HOME",ptr)==0)            {//HOME表示绝对原点位置（0）
			                                          ServoGoOrigin(1);
			                                          Servoendpoint_HOME[1]=0;//如果之前有回HOME 这里重新清0
			                                          while(Servoendpoint_HOME[1]==0)    //等待电机走完
																								{
																									Toyo_Read_Motor_LC_Status(1);//OK 2018 9 14
																								}
                                                Print_String("OK\r\n");Print_String("@_@\r\n");return 0;} //OK 2018 9 14   没回完就回OK? 电机0的位置
		else if(strcmp("SERVON",ptr)==0)          {Toyo_Set_ServoON(1);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                         //电机使能0K       //OK 2018 9 14  
    else if(strcmp("SERVOFF",ptr)==0)         {Toyo_Set_ServoOFF(1);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                         //电机失能OK      //OK 2018 9 14  
    else if(strcmp("READ ERR CODE",ptr)==0)   {Uart_Printf("ErrorCode=%X\r\n",Toyo_Read_ErrorCode(1));Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//读电机错误报警代码  //OK 2018 9 14
    else if(strcmp("READ_HOME_STATUS",ptr)==0){print=1;read_home_status();print=0;Print_String("OK\r\n");Print_String("@_@\r\n");return 0; }            //读电机状态 //OK 2018 9 14
		else if(strcmp("CLEAR ARM",ptr)==0)       {Toyo_Set_ClearArm(1);Print_String("OK\r\n");Print_String("@_@\r\n");return 0; }                             //清报警OK  //OK 2018 9 6
	  else if(strcmp("READ CURRENT POINT",ptr)==0){Uart_Printf("CURRENT POINT=%d\r\n",Toyo_Read_CurrentPoint(1));return 0; }                                     //读取当前位置OK  //OK 2018 9 14
		else if(strcmp("RESET MOTOR",ptr)==0)     { //RESET MOTOR表示15的位置
			                                          ServoGoAssignPoint(1,15);//OK 2018 9 14
			                                          ServoOrigin_R[1]=0;
			                                          while(ServoOrigin_R[1]==0)    //等待电机走完
																								{
																									Toyo_Read_Motor_LC_Status(1);//OK 2018 9 14
																								}
                                                Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                       //复位电机OK	
    
																								
		else if(strcmp("MOTOR TEST",ptr)==0)       {Motor_Test();Print_String("OK\r\n");Print_String("@_@\r\n");return 0; }  																					
    //====================公共函数======================================================================================
	  else if(Writeid(ptr)==0)                   {Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
		else if(set_hw_number(ptr)==0)             {Uart_Printf("ok\r\n");return 0;}//设置硬件版本号
		else if(strcmp("READ_HW",ptr)==0)	         {Uart_Printf("HW_20%d_V%d.%d\r\n",hw_date,hw_ver_shi,hw_ver_ge);return 0;}//读取硬件版本号				 
		else{
	    Uart_Printf("command not found!\r\n"); 
			Print_String("OK\r\n");
			Print_String("@_@\r\n");
	 	  return 1;
	  }
}

