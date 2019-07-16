#include "lpc17xx.h"
#include "Control.h"
#include "Uart.h"
#include "Uart3.h"
#include "string.h"
#include "Math.h"
#include "24c128.h"
#include "io.h"
#include "delay.h"
#include "repeathandle.h"
#include "PCA9655E.H"
#include <stdio.h>
#include <ctype.h>

uint8_t sent_flag = 0;
uint8_t StartAFlag=0,StartBFlag=0;
uint8_t begain_flag =0;
uint8_t holder_Y_flag=0;
uint8_t holder_Z_flag=0;
//-----------------------------------
char RET;//��ǰ�����ķ���ֵ	
int  STEP=0;//UP_N ��Ĳ���ֵ
char print;//������ӡ�����
int Motor_Speed=0;

unsigned char  Ascill_16[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

extern uint8_t debug_run_flag;
extern uint8_t uart1_buf[BUFSIZE];
extern uint8_t Ion_flag;

//================================================
uint8_t  downflag=0;
uint32_t Z_Test_LC=0;  //��ȡEEPROM Z_Test_LC
uint32_t Z_Ready_LC=0; //��ȡEEPROM Z_Ready_LC
uint32_t Initial_Zaxis_Plus=0;
//uint32_t ZaxisPlus=0;//����z��ĵ�ǰ�߶ȵ������� ZaxisPlus�� �棩 �� zPlus��ȡ�� ����  ������up down ���挍�F��һ�κ��m�߶ȵ�λ��


unsigned long hw_date=0;
unsigned char hw_ver_shi=0;
unsigned char hw_ver_ge=0;
unsigned long fw_date=0;
unsigned char fw_ver_shi=0;
unsigned char fw_ver_ge=0;
unsigned int  vtorch_fw_number=0;
extern unsigned char config_buf[50];
//================================================
void sensor_check(void){
			//����sensor���
			if(SENSOR_PUSH_front==0){
			 Uart_Printf("SENSOR_PUSH_front==0\r\n");
			}
			else{
			 Uart_Printf("SENSOR_PUSH_front==1\r\n");
			}
			if(SENSOR_PUSH_bottom==0){
			 Uart_Printf("SENSOR_PUSH_bottom==0\r\n");
			}
			else{
			 Uart_Printf("SENSOR_PUSH_bottom==1\r\n");
			}
			//=========start a b stop��ť���=========
	    if(Start_button_a==0){
		   Uart_Printf("Start_button_a==0\r\n");
		  }
	    else{
		   Uart_Printf("Start_button_a==1\r\n");
      }
		 
		  if(Start_button_b==0){
		   Uart_Printf("Start_button_b==0\r\n");
		  }
	    else{
		   Uart_Printf("Start_button_b==1\r\n");
      }
		  if(Stop_button==0){
		  Uart_Printf("Stop_button==0\r\n");
		  }
	    else{
		  Uart_Printf("Stop_button==1\r\n");
      }
		 //=======up down ����sensor ���======
		 if(SENSOR_Z_Up==0){
		  Uart_Printf("SENSOR_Z_Up==0\r\n");
		 }														  
	   else{
		  Uart_Printf("SENSOR_Z_Up==1\r\n");
     }
		 if(SENSOR_Z_Down==0){
		  Uart_Printf("SENSOR_Z_Down==0\r\n");
		 }
	   else{
		  Uart_Printf("SENSOR_Z_Down==1\r\n");
     }
		 //holder���� 
		  if(SENSOR_Y_front==0){
				Uart_Printf("SENSOR_Y_front==0\r\n");
			}
			else{
				Uart_Printf("SENSOR_Y_front==1\r\n");
      }
			if(SENSOR_Y_bottom==0){
				Uart_Printf("SENSOR_Y_bottom==0\r\n");
			}
			else{
				Uart_Printf("SENSOR_Y_bottom==1\r\n");
      }
			//===========��ѹ��1.2.3���====================
			if(FLEX_Negative_Pressure==0){
				Uart_Printf("FLEX_Negative_Pressure==0\r\n");	
			}
			else{
			  Uart_Printf("FLEX_Negative_Pressure==1\r\n");
			}					
}

//=============================================
void Motor_init(){
	 Delay_MS(4000); //�ȴ���������ʼ��OK 	
   RET=ServoInitialize(1);             //���RET=0���ʾ�������� RET=1��ʾ����ģʽ��������  RET=2��ʾ����������� RET=3��ʾ�ŷ�ON����
	 Uart_Printf("ini1=%02xH \r\n",RET); //���������û��delay RET=1 ����ԭ��������ͳ�ʱ	  
	 RET=ServoGoOrigin(1);               //�����HOME��
	 Uart_Printf("org1=%02xH \r\n",RET); //���RET=0���ʾ�������� RET=1 ����ԭ��������ͳ�ʱ
	 read_home_status();                 //���read_home_status()����û��whileѭ����ô��Ҫ��֮ǰ��1s��ʱ�ȴ��������
}
//=============================================
void fixture_init(void){
	  //ServoSetSpeed(1,Motor_Speed);  //���ô洢��EEPROM�е���ٶ� (���ڵ����HOME��ǰ���ֹ���δ��������������)
	  ServoGoAssignPoint(1,15);      //�����ߵ�����λ��15Ϊhome��  
	  while(ServoOrigin_R[1]==0)     //�ȴ��������
	  {
		  Toyo_Read_Motor_LC_Status(1);
	  }	
		//=======================================
		VACOFF();
		delay_ms(5);
		BLOW_MIRRORON();
		delay_ms(10);
		CM_PW_ON();
		delay_ms(10);
		LEDOFF();		//�ص�
		delay_ms(100);
		HOLDERUP();		//holder����
		delay_ms(100);
		HOLDERON();		//holder�����棬��ʼ����ֹײ��
		delay_ms(100);
		PUSHOFF();			//������
		ION_OFF();				//�ر�����
		sent_flag = 0;
		Ion_flag=0;
	  Print_String("BaudRate=19200,Parity=None,Data Bits=8,Stop Bits=1,Flow Control=None\r\n");
	  Uart_Printf("system init ok\r\n");	 
}
//=============================================================
//��UP"  Command
void PadUP(void){
			if((downflag==0)){//�տ�ʼ��ʱ��downflag=0���������
				downflag=1;//�������ߵĿ��� 
				ServoGoAssignPoint(1,Z_Test_LC);    //������
        while(Servoendpoint_E[1]==0)      //�ȴ��˵�����
				{					
					Toyo_Read_Motor_LC_Status(1);
				}
	      Print_String("OK\r\n");
				Print_String("@_@\r\n");				
				//Uart_Printf("ZaxisPlus=%d\r\n",m0_step_record);		 
			}
			else{//����else Ҫȥ����,����Ϊ�������λ�� ���˶��UP����
			Print_String("OK\r\n");
			Print_String("@_@\r\n");
			}
	
}
//=============================================================
//"Down" Command
void PadDown(void){
			if(downflag==1){//�տ�ʼ��ʱ���downflag=0���ֻ��������
				downflag=0;//�������ߵĿ���         
					ServoGoAssignPoint(1,15);    //�����ߵ�ԭ��
					while(ServoOrigin_R[1]==0)   //�ȴ�ԭ������
					{
						Toyo_Read_Motor_LC_Status(1);
					}
					Print_String("OK\r\n");
					Print_String("@_@\r\n");
				
				//Uart_Printf("ZaxisPlus=%d\r\n",m0_step_record);
			
			}
			else{//����else Ҫȥ����,����Ϊ�������λ�� ���˶��DOWN����
				Print_String("OK\r\n");
				Print_String("@_@\r\n");
			}

}
//=============CY1-CY5=================================
//=============��ŷ�1-- VACUUM================
void  Cylinder1_On(void)
{
	PCA9655E_OUT(0,0,1);
}
void  Cylinder1_Off(void)
{
	PCA9655E_OUT(0,0,0);
}
//=========��ŷ�2---LED==========================
void  Cylinder2_On(void)
{
	PCA9655E_OUT(0,1,1);
}
void  Cylinder2_Off(void)
{
	PCA9655E_OUT(0,1,0);
}
//========================(˫��)��ŷ�3----PUSH��������ƽ�,�������� ==========
void  Cylinder3_On(void)
{
	PCA9655E_OUT(0,2,1);
}
void  Cylinder3_Off(void)
{
	PCA9655E_OUT(0,2,0);
}
//========================(˫��)��ŷ�4----PUSH��������ƽ�,�������� ==========
void  Cylinder4_On(void)
{
	PCA9655E_OUT(0,3,1);
}
void  Cylinder4_Off(void)
{
	PCA9655E_OUT(0,3,0);
}
//========================(˫��)��ŷ�5----holder����/�½�=========
void Cylinder5_On(void)
{
	PCA9655E_OUT(0,4,1);
}
void Cylinder5_Off(void)
{
	PCA9655E_OUT(0,4,0);
}

//========================(˫��)��ŷ�6-----holder����/�½�=========
void Cylinder6_On(void)
{
	PCA9655E_OUT(0,5,1);
}
void Cylinder6_Off(void)
{
	PCA9655E_OUT(0,5,0);
}

//========================(˫��)��ŷ�7-----holderǰ��/����========
void Cylinder7_On(void)
{
	PCA9655E_OUT(0,6,1);
}
void Cylinder7_Off(void)
{
	PCA9655E_OUT(0,6,0);
}

//========================(˫��)��ŷ�8------holderǰ��/����========
void Cylinder8_On(void)
{
	PCA9655E_OUT(0,7,1);
}
void Cylinder8_Off(void)
{
	PCA9655E_OUT(0,7,0);
}

//========================��ŷ�9------CAMERA POWER==========
void Cylinder9_On(void)
{
	PCA9655E_OUT(1,0,0);
}
void Cylinder9_Off(void)
{
	PCA9655E_OUT(1,0,1);
}

//========================��ŷ�10-----Ԥ��---==========
void Cylinder10_On(void)
{
	PCA9655E_OUT(1,1,1);
}
void Cylinder10_Off(void)
{
	PCA9655E_OUT(1,1,0);
}
//========================��ŷ�11-----Ԥ��---==========
void Cylinder11_On(void)
{
	PCA9655E_OUT(1,2,1);
}
void Cylinder11_Off(void)
{
	PCA9655E_OUT(1,2,0);
}
//========================��ŷ�12-----Ԥ��---==========
void Cylinder12_On(void)
{
	PCA9655E_OUT(1,3,1);
}
void Cylinder12_Off(void)
{
	PCA9655E_OUT(1,3,0);
}
//========================��ŷ�13-----Ԥ��---==========
void Cylinder13_On(void)
{
	PCA9655E_OUT(1,4,1);
}
void Cylinder13_Off(void)
{
	PCA9655E_OUT(1,4,0);
}
//========================��ŷ�14-----Ԥ��---==========
void Cylinder14_On(void)
{
	PCA9655E_OUT(1,5,1);
}
void Cylinder14_Off(void)
{
	PCA9655E_OUT(1,5,0);
}
//========================��ŷ�15-----Ԥ��---==========
void Cylinder15_On(void)
{
	PCA9655E_OUT(1,6,1);
}
void Cylinder15_Off(void)
{
	PCA9655E_OUT(1,6,0);
}
//========================��ŷ�16-----Ԥ��---==========
void Cylinder16_On(void)
{
	PCA9655E_OUT(1,7,1);
}
void Cylinder16_Off(void)
{
	PCA9655E_OUT(1,7,0);
}
//=====================================================
void VACON(void)//�����
{
	Cylinder1_On();		 
}
void VACOFF(void)//�ر����
{		
	Cylinder1_Off();	 	
}
//=====================================================
void LEDON(void){
  Cylinder2_On();
}
void LEDOFF(void){
	Cylinder2_Off();
}
//===============================================
void PUSHON(){//Y��������ƽ�,��������
	if(holder_Z_flag==1)
	{
	unsigned int i=0;	
	Cylinder3_Off();
	delay_ms(1);
	Cylinder4_On();	 
	delay_ms(1);
	while(1){
	  if( (SENSOR_PUSH_bottom==0)&&(i<10000000) ){
		   Print_String("PushON OK\r\n");break;
    }
    if(i++>10000000){
			 Print_String("PushON FAIL\r\n");break;
		}		 
	 }
 }
 else Print_String("Holder PUSH Error!\r\n"); 
}
void PUSHOFF(){//Y��������Ƴ�����������
	unsigned int i=0;	
	Cylinder3_On();
	delay_ms(1);
	Cylinder4_Off();	 
	delay_ms(1);
	while(1){
	  if( (SENSOR_PUSH_front==0)&&(i<10000000) ){
		   Print_String("PushOFF OK\r\n");break;
    }
    if(i++>10000000){
			 Print_String("PushOFF FAIL\r\n");break;
		}		 
	 }	
}
void HOLDERUP(){//holder����
	unsigned int i=0;	
	Cylinder5_On();
	delay_ms(1);
	Cylinder6_Off();	 
	delay_ms(1);
	holder_Z_flag=1;
	while(1){
	  if( (SENSOR_Z_Up==0)&&(i<10000000) ){
		   Print_String("HoldUP OK\r\n");break;
    }
    if(i++>10000000){
			 Print_String("HoldUP FAIL\r\n");break;
		}		 
	 }
}
void HOLDERDOWN(void){//holder�½�
//	if(holder_Y_flag==0)
//		{
		unsigned int i=0;	
		Cylinder5_Off();
		delay_ms(1);
		Cylinder6_On();	 
		delay_ms(1);
		holder_Z_flag=0;
		while(1){
			if( (SENSOR_Z_Down==0)&&(i<10000000) ){
				 Print_String("HoldDown OK\r\n");break;
			}
			if(i++>10000000){
				 Print_String("HoldDown FAIL\r\n");break;
			}		 
		 }
//	 }
//	else Print_String("Holder down Error!\r\n");
}

void HOLDERON(){//holderǰ��
	if(holder_Z_flag==1)
	{
	unsigned int i=0;	
	Cylinder7_On();
	delay_ms(1);
	Cylinder8_Off();	 
	delay_ms(1);
	holder_Y_flag=1;
	while(1){
	  if( (SENSOR_Y_bottom==0)&&(i<10000000) ){
		   Print_String("HoldON OK\r\n");break;
    }
    if(i++>10000000){
			 Print_String("HoldON FAIL\r\n");break;
		}		 
	 }
 }
 else Print_String("Holder PUSH Error!\r\n"); 
}
void HOLDEROFF(void){//holder�˻�
	unsigned int i=0;	
	Cylinder7_Off();
	delay_ms(1);
	Cylinder8_On();	 
	delay_ms(1);
	holder_Y_flag=0;
	while(1)
	{
	  if( (SENSOR_Y_front==0)&&(i<10000000) ){
		   Print_String("HoldOFF OK\r\n");break;
    }
    if(i++>10000000)
		{
			 Print_String("HoldOFF FAIL\r\n");break;
		}		 
	 }	
}
void  CM_PW_ON(void){//CEREMA_POWERON
  Cylinder9_On();
}
void  CM_PW_OFF(void){//CEREMA_POWEROFF
  Cylinder9_Off();
}
void  ION_ON(void){//ION_ON
  Relay1_L;
	delay_ms(5);
	Cylinder10_On();
}
void  ION_OFF(void){//ION_OFF
	if(Ion_flag==0)
	{
		Relay1_H;
		delay_ms(5);
		Cylinder10_Off();
	}
}
void BLOW_MIRRORON(void)
{
	Cylinder11_Off();
}
void BLOW_MIRROROFF(void)
{
	Cylinder11_On();
}

//============================== �ξ߸�λ =======================================
void reset(uint8_t f)
{		
	  unsigned int i=0;
		LEDOFF();		//�ص�
		delay_ms(10);
		BLOW_MIRRORON();
		//================�������������====================
		Move_Ready_LC(); 
		delay_ms(100);
		//===================================================
		HOLDERUP();		//holder����
		//================================================
		delay_ms(100);
		HOLDERON();		//��ȥ�Ӳ�Ʒ
		//============================================
		delay_ms(50);
		VACOFF();
		delay_ms(200);
		//================����½�������λ��===========
		Move_Test_LC(); //0
		delay_ms(100);
		//==============================================
		ION_ON();//1009
	
		HOLDEROFF();		//holder�˳�
		delay_ms(100);
		//=================================
		PUSHOFF();			//������
		delay_ms(100);
		//=======================1009
		//ION_OFF();				//�ر�����
		sent_flag = 0;	
		//========================================��ʱ����==================================================
		while(1){
		if( ((SENSOR_Z_Up==0)&&(SENSOR_Y_front==0)&&(SENSOR_PUSH_front==0)&&(Stop_button==1))&&(i<20000000) ){//5s
			 Print_String("Reset OK\r\n");
			 Print_String("@_@\r\n");break;
		}
		if(i++>20000000){//5s
			 Print_String("Reset FAIL\r\n");
 			printf("Z_UP:%d\r\nY_F:%d\r\nPUSH_F:%d\r\nSTOP_B:%d",SENSOR_Z_Up,SENSOR_Y_front,SENSOR_PUSH_front,Stop_button);
			 Print_String("@_@\r\n");break;
		}			 
	 }
	 //========================================��ʱ����==================================================	
}
//=====================================================
/*
void ResetConfig1(void){	
	  unsigned int i=0;
		Cylinder2_Off();//�н�װ�ùر�
		delay_ms(1);
		Cylinder3_Off();//����PogoPin��ѹ����
		delay_ms(1);
		Cylinder5_Off();//�϶������½�
		delay_ms(1);
		Cylinder4_Off();//�ر������
		//================
		StartBFlag=0;
		StartAFlag=0;
		sent_flag = 0;
		begain_flag = 0;
		//========================================��ʱ����==================================================
		while(1){
		if( ((CONN_UP==0)&&(Stop_button==1))&&(i<20000000) ){//5s
			 Print_String("OK\r\n");break;
		}
		if(i++>20000000){//5s
			 Print_String("FAIL\r\n");break;
		}			 
	 }
	 //========================================��ʱ����==================================================
}
*/
//=====================================================
void Help()
{
			Print_String("\r\n");	 
	    Print_String("GO HOME       			DUT GO TEST LOCATION\r\n");	
	    Print_String("RESET MOTOR       	MOTOR RESET\r\n");	
	    Print_String("SAVE TEST LC        SAVE MOTOR TEST LOCATION\r\n");	
	    Print_String("SAVE READY LC       SAVE MOTOR READY LOCATION\r\n");	
	    Print_String("READ TEST LC    	  SAVE MOTOR TEST LOCATION\r\n");
	    Print_String("READ READY LC       SAVE MOTOR READY LOCATION\r\n");
			Print_String("MOVE TEST LC        MOTOR MOVE TEST LOCATION\r\n");
	    Print_String("MOVE READY LC       MOTOR MOVE READY LOCATION\r\n");
	    Print_String("UP_N                MOTOR MOVE N/100 MM\r\n");
			Print_String("READ ERR CODE       READ MOTOR ERROR CODE\r\n");	
			Print_String("CLEAR ARM       		CLEAR MOTOR ARM\r\n");
			Print_String("VACON              VAC ON\r\n");
			Print_String("VACOFF             VAC OFF\r\n");
	    Print_String("LEDON              LED ON\r\n");
			Print_String("LEDOFF             LED OFF\r\n");
	    Print_String("CAMERAPOWERON      CAMERA POWER ON\r\n");
			Print_String("CAMERAPOWEROFF     CAMERA POWER OFF\r\n");
	    Print_String("PUSHON             GATE CYLINDER IN\r\n");
			Print_String("PUSHOFF            GATE CYLINDER OUT\r\n");
	    Print_String("HOLDERUP           HOLDER CYLINDER UP\r\n");
			Print_String("HOLDERDOWN         HOLDER CYLINDER DWON\r\n");
	    Print_String("HOLDERON         	 HOLDER CYLINDER IN\r\n");
			Print_String("HOLDEROFF          HOLDER CYLINDER OUT\r\n");
			Print_String("SENSOR_CHECK       PRINT ALL SENSOR\r\n");
			Print_String("FWVER              FW VERSION NUM\r\n");
			Print_String("ID_N               FIXTURE ID\r\n");
			Print_String("ZS_N               MOTOR SPEED\r\n");
     	Print_String("RESET              RESET FIXTURE\r\n");
	    Print_String("HELP               LIST COMMAND\r\n");	
			Print_String("IONON              ION OPEN\r\n");
			Print_String("IONOFF             ION CLOSE\r\n");
     	Print_String("ION_ALL_OPEN       ION ALWAYS OPEN\r\n");
	    Print_String("ION_NORMAL         ION NORMAL WORK\r\n");
     	Print_String("BLOW_MIRRORON      MIRROR BLOW OPEN\r\n");
	    Print_String("BLOW_MIRROROFF     MIRROR BLOW OFF\r\n");	
}
//=============================================================
unsigned char set_vtorch_fw_number(char *str){
	//����vtorch fw number 0xf00c--0xf00f 
	unsigned char l,i;
  unsigned int dat=0;//FW=0X1111
  if( (str[0]=='F')&&(str[1]=='W')&&(str[2]=='=')&&(str[3]=='0')&&(str[4]=='X') )
  {
		l=strlen(str);
    for(i=5;i<9;i++)
    {
      if((str[i]>='0')&&(str[i]<='9'))    
        dat += (str[i]-'0')*pow(10,l-i-1);			//��ȡ����
      else 
        return 1;
    }
    vtorch_fw_number=dat;												//��������
	  config_buf[12]=(vtorch_fw_number>>24)&0xff;
		config_buf[13]=(vtorch_fw_number>>16)&0xff;
		config_buf[14]=(vtorch_fw_number>>8)&0xff;
    config_buf[15]=(vtorch_fw_number>>0)&0xff;
		AT24C128_W_P(0xf00c,&config_buf[12],4); 		
  }
  else{
     return 1;
  }
  return 0;
}
//=======================  ���ذ汾��  ========================
void ver(void)
{
	   Print_String("0X0105\r\n");
	   Print_String("for 2019/7/15 Andy Song Revise\r\n");
	   Print_String("Add Vacuum Check\r\n");
}
//========================д�ξ�ID��===========================
unsigned char Writeid(char *str){//ID=000001
  int dat[12]={0};
  int da[6]={0};
	if( (str[0]=='I')&&(str[1]=='D')&&(str[2]=='=')&&(strlen(str)==9) )
  {
     //=====================================
			if(  ((str[3]>='0')&&(str[3]<='9')) ){
			  dat[0]=str[3]-0x30;
			  dat[0]=dat[0]*16;
			}
      if(  ((str[3]>='A')&&(str[3]<='F')) ){
				dat[0]=str[3]-0x37;	
				dat[0]=dat[0]*16;
			}	
      if(  ((str[4]>='0')&&(str[4]<='9')) ){
			  dat[1]=str[4]-0x30;
			}	
			if(  ((str[4]>='A')&&(str[4]<='F')) ){
				dat[1]=str[4]-0x37;	
			}	
      da[0]=dat[0]+dat[1];			
			AT24C128_W(0xFA,da[0]);	
			//=====================================
			if(  ((str[5]>='0')&&(str[5]<='9')) ){
			  dat[2]=str[5]-0x30;
			  dat[2]=dat[2]*16;
			}	
			if(  ((str[5]>='A')&&(str[5]<='F')) ){
				dat[2]=str[5]-0x37;	
				dat[2]=dat[2]*16;	
			}	
			
			if(  ((str[6]>='0')&&(str[6]<='9')) ){
			  dat[3]=str[6]-0x30;
			}	
			if(  ((str[6]>='A')&&(str[6]<='F')) ){
				dat[3]=str[6]-0x37;	
			}	
			da[1]=dat[2]+dat[3];
			//Uart_Printf("%d",da[1]);
			AT24C128_W(0xFB,da[1]);
			//=====================================
			if(  ((str[7]>='0')&&(str[7]<='9')) ){
			  dat[4]=str[7]-0x30;
			  dat[4]=dat[4]*16;
			}	
			if(  ((str[7]>='A')&&(str[7]<='F')) ){
				dat[4]=str[7]-0x37;	
				dat[4]=dat[4]*16;	
			}	
			
			if(  ((str[8]>='0')&&(str[8]<='9')) ){
			  dat[5]=str[8]-0x30;
			}	
			if(  ((str[8]>='A')&&(str[8]<='F')) ){
				dat[5]=str[8]-0x37;	
			}	
			da[2]=dat[4]+dat[5];
			//Uart_Printf("%d",da[2]);
			AT24C128_W(0xFC,da[2]);	
	  }
	  else{
	     return 1;
	  }
	  return 0;
}
//=====================================================
void UnitID(uint8_t f){	
  unsigned short i=0;
	unsigned char  dat[10];
	//Print_String("\r\n");
	for(i=0xFA;i<=0xFC;i++){
	 	dat[i-0xfA]=AT24C128_R(i);
	 }
	Print_String("0X");
	for(i=0;i<3;i++){
	  Uart_Printf("%.2X",dat[i]);
	}
	if(f==1){
		Uart_Printf("\r\nOK\r\n");
	}
}
//=====================================================
//����hw number 0xf000--0xf005
unsigned char set_hw_number(char *str)	
{
  unsigned char i;
  unsigned long dat=0;//HW=20161024_V1.0
   if( (str[0]=='H')&&(str[1]=='W')&&(str[2]=='=')&&(str[11]=='_')&&(str[12]=='V')&&(str[14]=='.') )
  {
    for(i=5;i<11;i++)
    {
      if((str[i]>='0')&&(str[i]<='9'))    
        dat += (str[i]-'0')*pow(10,10-i);	//��ȡ����
      else 
        return 1;
    }
    
		hw_date=dat;//��������
		config_buf[0]=(hw_date>>24)&0xff;
		config_buf[1]=(hw_date>>16)&0xff;
		config_buf[2]=(hw_date>>8)&0xff;
    config_buf[3]=(hw_date>>0)&0xff;	
		AT24C128_W_P(0xf000,&config_buf[0],4); 
		
	  config_buf[4]=(str[13]-'0');
		config_buf[5]=(str[15]-'0');
		hw_ver_shi=config_buf[4];
		hw_ver_ge=config_buf[5];
		AT24C128_W(0xf004,config_buf[4]);
		AT24C128_W(0xf005,config_buf[5]); //0xffff		
  }
  else{
     return 1;
  }
  return 0;
}
//=====================================================
//����fw number 0xf006--0xf00b  
unsigned char set_fw_number(char *str)
{
  unsigned char i;
  unsigned long dat=0;//FW=20161024_V1.0
  if( (str[0]=='F')&&(str[1]=='W')&&(str[2]=='=')&&(str[11]=='_')&&(str[12]=='V')&&(str[14]=='.') )
  {
    for(i=5;i<11;i++)
    {
      if((str[i]>='0')&&(str[i]<='9'))    
        dat += (str[i]-'0')*pow(10,10-i);				//��ȡ����
      else 
        return 1;
    }
    fw_date=dat;															//��������
	  config_buf[6]=(fw_date>>24)&0xff;
		config_buf[7]=(fw_date>>16)&0xff;
		config_buf[8]=(fw_date>>8)&0xff;
    config_buf[9]=(fw_date>>0)&0xff;
		AT24C128_W_P(0xf006,&config_buf[6],4); 
		
		config_buf[10]=(str[13]-'0');
		config_buf[11]=(str[15]-'0');
		fw_ver_shi=config_buf[10];
		fw_ver_ge=config_buf[11];
		AT24C128_W(0xf00a,config_buf[10]);
		AT24C128_W(0xf00b,config_buf[11]); //0xffff
		
  }
  else{
     return 1;
  }
  return 0;
}




//========================================================
int GetStringNum( char *cmd)
{
	uint8_t k=0;
	int b[4];
	char buf[40];
	b[0]=0;
	while (cmd[k] !='\0') { 
		if(isdigit(cmd[k])){
			buf[k] =cmd[k]; 	    
		}else{
			buf[k] = ' '; 
		}
		++k; 
	} 
	buf[k]='\0';
	sscanf(buf,"%u", &b[0]);//%u�޷���ʮ����  b[0]��buf
	return b[0];//2000 ��Ӧ500
}

//====================================================
void key_scan(void)
{
//-------------------����A OR ����B---------------------------
	if((Start_button_a == 0)||(Start_button_b == 0))
	{
		delay_ms(50);
		if(((Start_button_a == 0)||(Start_button_b == 0))&&(sent_flag == 0))
		{
			delay_ms(50);
			/*******1009
			ION_ON();		//�����Ӱ�
			delay_ms(100);
			****/
			PUSHON();			//�رճ���
			delay_ms(100);
			HOLDERON();			//HOLDER ǰ��
			delay_ms(100);
			ION_OFF();			//1009
			delay_ms(100);
			LEDON();			//����
			delay_ms(100);
			VACON();
			delay_ms(1000);
			
			//================�������������ȡ�߲�Ʒ=============
			Move_Ready_LC();
			delay_ms(200);
			//===================================================
			HOLDEROFF();
			delay_ms(100);
			HOLDERDOWN();
			delay_ms(100);
			//================����½�������λ��=================
			Move_Test_LC(); 
			//===================================================
			delay_ms(100);
			BLOW_MIRROROFF();
			sent_flag =1;
			if((SENSOR_PUSH_bottom==0)&&(SENSOR_Y_front==0)&&(SENSOR_Z_Down==0))
			{
					if(FLEX_Negative_Pressure == 0)
					{
						Print_String("Start\r\n");
					}
					else
					{
						Uart_Printf("VAC_ERROR\r\n");
					}
			 }
			 else
			 {
					Uart_Printf("SensorStatusErr\r\n");
					sensor_check();
			 }
		}           
	}while((!Start_button_a)||(!Start_button_b));		
}

//====================================================
void GO_HOME(void)
{
	ION_ON();		//�����Ӱ�
	delay_ms(100);
	PUSHON();			//�رճ���
	delay_ms(100);
	HOLDERON();			//HOLDER ǰ��
	delay_ms(100);
	LEDON();			//����
	delay_ms(100);
	VACON();
	delay_ms(100);
	//================�������������ȡ�߲�Ʒ=============
	Move_Ready_LC();
	//===================================================
	HOLDEROFF();
	delay_ms(100);
	HOLDERDOWN();
	delay_ms(200);
	//================����½�������λ��=================
	Move_Test_LC(); 
	//===================================================
}
//===================z�����ٶ�config_buf[22--25] 0xf016--0xf019======================
unsigned char SetZS(char *str){//ZS_100
	unsigned char l,i;
	unsigned int dat=0;
  if( (str[0]=='Z')&&(str[1]=='S')&&(str[2]=='_')&&(str[6]=='\0')&&(strlen(str)==6) ){
		  l=strlen(str);
			for(i=3;i<l;i++)
			{
				if((str[i]>='0')&&(str[i]<='9'))    
					dat += (str[i]-'0')*pow(10,l-i-1);				//��ȡ����
				else 
					return 1;
			}
			if(dat<=100){
				Motor_Speed=dat;
				SaveZS(Motor_Speed);
 			  ServoSetSpeed(1,Motor_Speed);
				
			}

  }
  else
	{
	 return 1;
  }
  return 0;
}
//======= Z Motor speed 0x000c--0x000f=======================================================
uint32_t GetZS(void){
	 Motor_Speed= GetZSValue();
	 Uart_Printf("GetZS=%d\r\n",Motor_Speed);
	 return Motor_Speed;

}
//=====================================================================================
void Set_Motor_Initial_Position(uint32_t Z_Plus){
   Set_Z_Initial_Position(Z_Plus);
}

//===============================================================================================
void Up_N(uint32_t tempValue){     
	debug_run_flag=1;
	STEP=tempValue;                  //savez��ʱ���õ�
	ServoGoAssignPoint(1,tempValue);
	while(ServoMeddle_M[1]==0)       //�ȴ�����
	{
		Toyo_Read_Motor_LC_Status(1);
	}
	Uart_Printf("STEP=%d\r\n",tempValue);
}



void Move_Test_LC(void){
	ServoGoAssignPoint(1,Z_Test_LC);
	Servoendpoint_E[1]=0;
	while(Servoendpoint_E[1]==0)       //�ȴ�����
	{
		Toyo_Read_Motor_LC_Status(1);
	}

}
void Move_Ready_LC(void){

	ServoGoAssignPoint(1,Z_Ready_LC);
	STEP=Z_Ready_LC;//Readyλ�õ����м�λ��������
	ServoMeddle_M[1]=0;
	while(ServoMeddle_M[1]==0)       //�ȴ�����
	{
		Toyo_Read_Motor_LC_Status(1);
	}
}

void Motor_Test(void)
{
	uint16_t times=0;
  while(times<1000){
// 		ServoGoAssignPoint(1,10015);
// 		STEP=10015;
// 		ServoMeddle_M[1]=0;
// 		while(ServoMeddle_M[1]==0)       //�ȴ�����
// 		{
// 			Toyo_Read_Motor_LC_Status(1);
// 		}
// 		ServoGoAssignPoint(1,15);
// 		
// 		ServoOrigin_R[1]=0;
// 		while(ServoOrigin_R[1]==0)       //�ȴ�����
// 		{
// 			Toyo_Read_Motor_LC_Status(1);
// 		}
// 		times++;
// 	  Uart_Printf("times=%d\r\n",times);	
		
		ServoGoAssignPoint(1,10015);
		delay_ms(2000);
		ServoGoAssignPoint(1,15);
		delay_ms(2000);
		times++;
	  Uart_Printf("times=%d\r\n",times);
	
 }
	
}

