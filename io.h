#ifndef __IO_H_
#define __IO_H_

//==================  sensor ���� �Զ������� (������ȷ������֤) =============================================================
#define SENSOR_misumi_Y_MAX    	 ((LPC_GPIO1->FIOPIN>>0)&0x01)	 //misumi�������	   misumi_Sensor_max   
//#define SENSOR_misumi_Y_Mider  	 ((LPC_GPIO1->FIOPIN>>1)&0x01)   //misumi����ж�	   misumi_Sensor_mid  
#define SENSOR_misumi_Y_Min	   	 ((LPC_GPIO1->FIOPIN>>4)&0x01)   //misumi����׶�	   misumi_Sensor_min  

//========================= ����SENSOR�ź�  FRONT/BOTTOM���ξ߱������ο�==================================================================================
#define SENSOR_PUSH_front  		 ((LPC_GPIO1->FIOPIN>>8)&0x01)   //�����sensor     signal3
#define SENSOR_PUSH_bottom 		 ((LPC_GPIO1->FIOPIN>>9)&0x01)   //����ر�sensor     signal4
#define SENSOR_Y_front 	       ((LPC_GPIO1->FIOPIN>>19)&0x01)  //����SENSOR�ź�holder	   SENSOR_Y_Up_front 	   ��sensor11
#define SENSOR_Y_bottom        ((LPC_GPIO1->FIOPIN>>20)&0x01)  //����SENSOR�ź�holder	   SENSOR_Y_Up_bottom    ��sensor12
#define SENSOR_Z_Up 	         ((LPC_GPIO1->FIOPIN>>21)&0x01)  //����SENSOR�ź�	   SENSOR_Z_Up           ��sensor13
#define SENSOR_Z_Down          ((LPC_GPIO1->FIOPIN>>30)&0x01)  //����SENSOR�ź�	   SENSOR_Z_Down         ��sensor14

//========================  ��ѹ�� �ź� ===================================================================================== 
#define FLEX_Negative_Pressure  ((LPC_GPIO1->FIOPIN>>31)&0x01)  //Pogo_Press   SENSOR15
//========================  PogoPin ��ѹ�������ź� ==========================================================================
// #define CONN_UP    				       ((LPC_GPIO1->FIOPIN>>15)&0x01)  //PogoPin           conn up      sensor7
// #define CONN_DOWN    			       ((LPC_GPIO1->FIOPIN>>16)&0x01)  //PogoPin		       conn down    sensor8

// #define PADUP    				         ((LPC_GPIO1->FIOPIN>>17)&0x01)  //PogoPin           3PAD UP	    sensor9
// #define PADDOWN    			         ((LPC_GPIO1->FIOPIN>>18)&0x01)  //PogoPin		       3PAD DOWN    sensor10

#define Start_button_a 			  	 ((LPC_GPIO0->FIOPIN>>5)&0x01) //----˫����ťA-----
#define Start_button_b			  	 ((LPC_GPIO0->FIOPIN>>6)&0x01)	 //----˫����ťB-----
#define Reset_button				     ((LPC_GPIO0->FIOPIN>>7)&0x01) 	 //----��λ��ť------
#define Stop_button			         ((LPC_GPIO0->FIOPIN>>8)&0x01) 	 //----��ͣ��ť------

//================== relay ��� �Զ�������=======================================================================================
#define Relay1_H						     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<23))  //----RELAY1 �ߵ�ƽ-------
#define Relay1_L					       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<23))  //----relay1�͵�ƽ-------

//=======����ŷ���Cylinder ��� �Զ�������(�����ȷ������֤)====================================================================
#define Cylinder1_H			  		   LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<15)) 	//-------���� ģ�ⲣ��----
#define Cylinder1_L			         LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<15))	//-------���� ģ�ⲣ��----
#define Cylinder2_H					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<16))    //-------�н�--------
#define Cylinder2_L					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<16))   //-------�н�--------
#define Cylinder3_H					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<17))	//-------ѹPogoPin---
#define Cylinder3_L					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<17))	//-------ѹPogoPin---
#define Cylinder4_H 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<18))	//-------���--------
#define Cylinder4_L 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<18))	//-------���--------
#define Cylinder5_H 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<19))	//-------3PAD-->��pogopin--------
#define Cylinder5_L 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<19))	//-------3PAD-->��pogopin--------


#define Cylinder7_H 			         LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<21))	
#define Cylinder7_L 			         LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<21))	
//===================== IO �ڳ�ʼ��==============================================================================================
void IO_Init(void);	//IO �˿ڳ�ʼ������
void Sensor_IO_Init(void);//sensor  ����������IO����
void Start_IO_Init(void);	//start  ����IO����
void Cylinder_IO_Init(void);//����IO�������
void Swich_key_IO(void);//���뿪��IO����
void Relay_IO_Init(void);
#endif
