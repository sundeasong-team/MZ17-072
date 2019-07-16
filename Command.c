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
extern char StrID[30];//ƴ��TESTERID��
extern uint32_t Z_Test_LC; //��ȡEEPROM 
extern uint32_t Z_Ready_LC;//��ȡEEPROM 
extern int STEP;       //�м����UP_N �������߶�
extern char print;     //�Ƿ��ӡ��־

//========================== �ַ�ת������==============
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
	 else if(strcmp("CY1ON",ptr)==0)            {Cylinder1_On();return 0;}//Y����������
	 else if(strcmp("CY1OFF",ptr)==0)           {Cylinder1_Off();return 0;}//Y���������� 
	 else if(strcmp("CY2ON",ptr)==0)            {Cylinder2_On();return 0;} //Z���϶�����
	 else if(strcmp("CY2OFF",ptr)==0)           {Cylinder2_Off();return 0;}//Z���϶����� 
	 else if(strcmp("CY3ON",ptr)==0)            {Cylinder3_On();return 0;} //VACUUM
	 else if(strcmp("CY3OFF",ptr)==0)           {Cylinder3_Off();return 0;}//VACUUM 
	 else if(strcmp("CY4ON",ptr)==0)            {Cylinder4_On();return 0;} //USB SWITCH
	 else if(strcmp("CY4OFF",ptr)==0)           {Cylinder4_Off();return 0;}//USB SWITCH
	 else if(strcmp("CY5ON",ptr)==0)            {Cylinder5_On();return 0;} //LED
	 else if(strcmp("CY5OFF",ptr)==0)           {Cylinder5_Off();return 0;}//LED 
	 else if(strcmp("CY6ON",ptr)==0)            {Cylinder6_On();return 0;} //Ԥ��
	 else if(strcmp("CY6OFF",ptr)==0)           {Cylinder6_Off();return 0;}//Ԥ��
   else if(strcmp("CY7ON",ptr)==0)            {Cylinder7_On();return 0;} //Y����������
	 else if(strcmp("CY7OFF",ptr)==0)           {Cylinder7_Off();return 0;}//Y����������
   else if(strcmp("CY8ON",ptr)==0)            {Cylinder8_On();return 0;} //Z���϶�����
	 else if(strcmp("CY8OFF",ptr)==0)           {Cylinder8_Off();return 0;}//Z���϶�����
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

	 else if(strcmp("VACON",ptr) == 0) 	  	    {VACON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�����
	 else if(strcmp("VACOFF",ptr) == 0) 	 	    {VACOFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�ر����
	 else if(strcmp("LEDON",ptr) == 0) 	  	    {LEDON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//��LED
	 else if(strcmp("LEDOFF",ptr) == 0) 	 	    {LEDOFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�ر�LED
	 else if(strcmp("PUSHON",ptr) == 0) 	  	  {PUSHON();Print_String("@_@\r\n");return 0;}//Y��������ƽ�,˫����ŷ�
	 else if(strcmp("PUSHOFF",ptr) == 0) 	 	    {PUSHOFF();Print_String("@_@\r\n");return 0;}//Y��������Ƴ���˫����ŷ�
	 else if(strcmp("HOLDERUP",ptr) == 0) 	    {HOLDERUP();Print_String("@_@\r\n");return 0;}//holder����������˫����ŷ�
	 else if(strcmp("HOLDERDOWN",ptr) == 0) 	  {HOLDERDOWN();Print_String("@_@\r\n");return 0;}//holder�����½���˫����ŷ�
	 else if(strcmp("HOLDERON",ptr) == 0) 	    {HOLDERON();Print_String("@_@\r\n");return 0;}//holderǰ����˫����ŷ�
	 else if(strcmp("HOLDEROFF",ptr) == 0) 	    {HOLDEROFF();Print_String("@_@\r\n");return 0;}//holder�˻أ�˫����ŷ�
	 
	 else if(strcmp("CAMERAPOWERON",ptr) == 0)  {CM_PW_ON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�����Դon
	 else if(strcmp("CAMERAPOWEROFF",ptr) == 0) {CM_PW_OFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�����Դoff	

	 else if(strcmp("BLOW_MIRRORON",ptr) == 0)  	{BLOW_MIRRORON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
	 else if(strcmp("BLOW_MIRROROFF",ptr) == 0) 	{BLOW_MIRROROFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}	 
	 else if(strcmp("ION_ALL_OPEN",ptr) == 0)  	{Ion_flag=1;Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
	 else if(strcmp("ION_NORMAL",ptr) == 0) 		{Ion_flag=0;Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
     else if(strcmp("IONON",ptr) == 0)  				{ION_ON();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//����open
     else if(strcmp("IONOFF",ptr) == 0) 				{ION_OFF();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//����close 	 
     else if(strcmp("GO_HOME",ptr) == 0) 				{GO_HOME();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//ǰ������λ��
	  //motor	
// 		else if(strcmp("STIMPADDIS",ptr) == 0)		{	                       //Զ���Ʒ	
// 																								if(debug_run_flag==0){ //��������debug_run_flag=0
// 																								 PadDown();            //���Զ���Ʒ
// 																								}
// 																					      return 0;
// 		                                          } 
// 		
// 		else if(strcmp("STIMPADEN",ptr) == 0)		  {                        //������Ʒ
// 																							  if(debug_run_flag==0){ //��������debug_run_flag=0
// 																							   PadUP();              //���������Ʒ
// 																						    }
// 																						    return 0;
// 												                      }
		else if(strcmp("RESET",ptr) == 0) 	 	    {reset(1);Print_String("@_@\r\n");return 0;}
//		else if(set_vtorch_fw_number(ptr)==0)     {Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}        //����FW�汾��
		else if(strcmp("FWVER",ptr) == 0) 	 	    {ver();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}  //��ȡFW�汾��
		else if(strcmp("HELP",ptr) == 0)          {Help();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;} 				
		else if(SetZS(ptr)==0)                    {Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}        //���õ���ٶ�  
		else if(strcmp("GETZS",ptr)==0)           {GetZS();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//��ȡ����ٶ�
		else if(strncmp("UP_",ptr,3) == 0)		    {tempValue=GetStringNum(ptr);Up_N(tempValue);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
		
    else if(strcmp("SAVE TEST LC",ptr) == 0)  {Z_Test_LC=STEP;Save_Test_LC(Z_Test_LC);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                                                          //����z�����λ��
		else if(strcmp("READ TEST LC",ptr) == 0)  {Z_Test_LC=Read_Test_LC();Uart_Printf("Z_Test_LC=%d\r\n",Z_Test_LC);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}          //��ȡz���趨����λ��	
    else if(strcmp("SAVE READY LC",ptr) == 0) {Save_Ready_LC(STEP);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                                                         //����z��Ԥ��λ��
		else if(strcmp("READ READY LC",ptr)==0)		{Z_Ready_LC=Read_Ready_LC();Uart_Printf("Z_Ready_LC=%d\r\n",Z_Ready_LC);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}      //��ȡz��Ԥ��λ��	
	
		else if(strncmp("SETMIP_",ptr,7)==0)		  {tempValue=GetStringNum(ptr);   Set_Motor_Initial_Position(tempValue);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//����PT FT�����ʼλ��
		else if(strcmp("GETMIP",ptr)==0)		      {Get_Motor_Initial_Position();  Print_String("OK\r\n");Print_String("@_@\r\n");return 0;} //for PT FT ��ȡ�����ʼλ��
		
		else if(strcmp("MOVE TEST LC",ptr) == 0)  {Move_Test_LC(); Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�ƶ�������λ��                                
		else if(strcmp("MOVE READY LC",ptr) == 0) {Move_Ready_LC();Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//�ƶ���Ԥ��λ��
		//========================================================================================
		else if(strcmp("HOME",ptr)==0)            {//HOME��ʾ����ԭ��λ�ã�0��
			                                          ServoGoOrigin(1);
			                                          Servoendpoint_HOME[1]=0;//���֮ǰ�л�HOME ����������0
			                                          while(Servoendpoint_HOME[1]==0)    //�ȴ��������
																								{
																									Toyo_Read_Motor_LC_Status(1);//OK 2018 9 14
																								}
                                                Print_String("OK\r\n");Print_String("@_@\r\n");return 0;} //OK 2018 9 14   û����ͻ�OK? ���0��λ��
		else if(strcmp("SERVON",ptr)==0)          {Toyo_Set_ServoON(1);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                         //���ʹ��0K       //OK 2018 9 14  
    else if(strcmp("SERVOFF",ptr)==0)         {Toyo_Set_ServoOFF(1);Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                         //���ʧ��OK      //OK 2018 9 14  
    else if(strcmp("READ ERR CODE",ptr)==0)   {Uart_Printf("ErrorCode=%X\r\n",Toyo_Read_ErrorCode(1));Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}//��������󱨾�����  //OK 2018 9 14
    else if(strcmp("READ_HOME_STATUS",ptr)==0){print=1;read_home_status();print=0;Print_String("OK\r\n");Print_String("@_@\r\n");return 0; }            //�����״̬ //OK 2018 9 14
		else if(strcmp("CLEAR ARM",ptr)==0)       {Toyo_Set_ClearArm(1);Print_String("OK\r\n");Print_String("@_@\r\n");return 0; }                             //�屨��OK  //OK 2018 9 6
	  else if(strcmp("READ CURRENT POINT",ptr)==0){Uart_Printf("CURRENT POINT=%d\r\n",Toyo_Read_CurrentPoint(1));return 0; }                                     //��ȡ��ǰλ��OK  //OK 2018 9 14
		else if(strcmp("RESET MOTOR",ptr)==0)     { //RESET MOTOR��ʾ15��λ��
			                                          ServoGoAssignPoint(1,15);//OK 2018 9 14
			                                          ServoOrigin_R[1]=0;
			                                          while(ServoOrigin_R[1]==0)    //�ȴ��������
																								{
																									Toyo_Read_Motor_LC_Status(1);//OK 2018 9 14
																								}
                                                Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}                       //��λ���OK	
    
																								
		else if(strcmp("MOTOR TEST",ptr)==0)       {Motor_Test();Print_String("OK\r\n");Print_String("@_@\r\n");return 0; }  																					
    //====================��������======================================================================================
	  else if(Writeid(ptr)==0)                   {Print_String("OK\r\n");Print_String("@_@\r\n");return 0;}
		else if(set_hw_number(ptr)==0)             {Uart_Printf("ok\r\n");return 0;}//����Ӳ���汾��
		else if(strcmp("READ_HW",ptr)==0)	         {Uart_Printf("HW_20%d_V%d.%d\r\n",hw_date,hw_ver_shi,hw_ver_ge);return 0;}//��ȡӲ���汾��				 
		else{
	    Uart_Printf("command not found!\r\n"); 
			Print_String("OK\r\n");
			Print_String("@_@\r\n");
	 	  return 1;
	  }
}

