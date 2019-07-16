#ifndef __IO_H_
#define __IO_H_

//==================  sensor 输入 自定义引脚 (输入正确，已验证) =============================================================
#define SENSOR_misumi_Y_MAX    	 ((LPC_GPIO1->FIOPIN>>0)&0x01)	 //misumi电机顶端	   misumi_Sensor_max   
//#define SENSOR_misumi_Y_Mider  	 ((LPC_GPIO1->FIOPIN>>1)&0x01)   //misumi电机中端	   misumi_Sensor_mid  
#define SENSOR_misumi_Y_Min	   	 ((LPC_GPIO1->FIOPIN>>4)&0x01)   //misumi电机底端	   misumi_Sensor_min  

//========================= 气缸SENSOR信号  FRONT/BOTTOM以治具本身做参考==================================================================================
#define SENSOR_PUSH_front  		 ((LPC_GPIO1->FIOPIN>>8)&0x01)   //抽屉打开sensor     signal3
#define SENSOR_PUSH_bottom 		 ((LPC_GPIO1->FIOPIN>>9)&0x01)   //抽屉关闭sensor     signal4
#define SENSOR_Y_front 	       ((LPC_GPIO1->FIOPIN>>19)&0x01)  //气缸SENSOR信号holder	   SENSOR_Y_Up_front 	   接sensor11
#define SENSOR_Y_bottom        ((LPC_GPIO1->FIOPIN>>20)&0x01)  //气缸SENSOR信号holder	   SENSOR_Y_Up_bottom    接sensor12
#define SENSOR_Z_Up 	         ((LPC_GPIO1->FIOPIN>>21)&0x01)  //气缸SENSOR信号	   SENSOR_Z_Up           接sensor13
#define SENSOR_Z_Down          ((LPC_GPIO1->FIOPIN>>30)&0x01)  //气缸SENSOR信号	   SENSOR_Z_Down         接sensor14

//========================  负压表 信号 ===================================================================================== 
#define FLEX_Negative_Pressure  ((LPC_GPIO1->FIOPIN>>31)&0x01)  //Pogo_Press   SENSOR15
//========================  PogoPin 下压与上升信号 ==========================================================================
// #define CONN_UP    				       ((LPC_GPIO1->FIOPIN>>15)&0x01)  //PogoPin           conn up      sensor7
// #define CONN_DOWN    			       ((LPC_GPIO1->FIOPIN>>16)&0x01)  //PogoPin		       conn down    sensor8

// #define PADUP    				         ((LPC_GPIO1->FIOPIN>>17)&0x01)  //PogoPin           3PAD UP	    sensor9
// #define PADDOWN    			         ((LPC_GPIO1->FIOPIN>>18)&0x01)  //PogoPin		       3PAD DOWN    sensor10

#define Start_button_a 			  	 ((LPC_GPIO0->FIOPIN>>5)&0x01) //----双启按钮A-----
#define Start_button_b			  	 ((LPC_GPIO0->FIOPIN>>6)&0x01)	 //----双启按钮B-----
#define Reset_button				     ((LPC_GPIO0->FIOPIN>>7)&0x01) 	 //----复位按钮------
#define Stop_button			         ((LPC_GPIO0->FIOPIN>>8)&0x01) 	 //----急停按钮------

//================== relay 输出 自定义引脚=======================================================================================
#define Relay1_H						     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<23))  //----RELAY1 高电平-------
#define Relay1_L					       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<23))  //----relay1低电平-------

//=======（电磁阀）Cylinder 输出 自定义引脚(输出正确，已验证)====================================================================
#define Cylinder1_H			  		   LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<15)) 	//-------挡块 模拟玻璃----
#define Cylinder1_L			         LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<15))	//-------挡块 模拟玻璃----
#define Cylinder2_H					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<16))    //-------夹紧--------
#define Cylinder2_L					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<16))   //-------夹紧--------
#define Cylinder3_H					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<17))	//-------压PogoPin---
#define Cylinder3_L					     LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<17))	//-------压PogoPin---
#define Cylinder4_H 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<18))	//-------真空--------
#define Cylinder4_L 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<18))	//-------真空--------
#define Cylinder5_H 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<19))	//-------3PAD-->顶pogopin--------
#define Cylinder5_L 			       LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<19))	//-------3PAD-->顶pogopin--------


#define Cylinder7_H 			         LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN|((1<<21))	
#define Cylinder7_L 			         LPC_GPIO0->FIOPIN=LPC_GPIO0->FIOPIN&(~(1<<21))	
//===================== IO 口初始化==============================================================================================
void IO_Init(void);	//IO 端口初始化函数
void Sensor_IO_Init(void);//sensor  传感器输入IO配置
void Start_IO_Init(void);	//start  输入IO配置
void Cylinder_IO_Init(void);//汽缸IO输出配置
void Swich_key_IO(void);//拨码开关IO配置
void Relay_IO_Init(void);
#endif
