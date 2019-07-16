#include "lpc17xx.h"
#include "Uart.h"
#include "24c128.h"
#include "IIC.h"
#include "Control.h"

//======================================
#define U8  unsigned char 
#define U16 unsigned short
#define U32 unsigned int

unsigned char config_buf[50];
extern uint32_t Z_Test_LC;
extern uint32_t Z_Ready_LC;
extern uint32_t Initial_Zaxis_Plus;


extern int Motor_Speed;
extern unsigned long hw_date;
extern unsigned char hw_ver_shi;
extern unsigned char hw_ver_ge;
extern unsigned long fw_date;
extern unsigned char fw_ver_shi;
extern unsigned char fw_ver_ge;
extern unsigned int  vtorch_fw_number;

//======================================
/*
  hw number                     config_buf[0--5]   0xf000--0xf005
  fw number                     config_buf[6--11]  0xf006--0xf00b
	vtorch_fw_number              config_buf[12--15] 0xf00c--0xf00f
	TESTERID                      config_buf[16--21] 0xf010--0xf015
	
	Test LC                                          0x0004--0x0007
	Ready LC                                         0x0008--0x000b
	Motor Speed                                      0x000c--0x000f  
  Initial_Zaxis_Plus                               0x0017--0x001a	

*/
//======================================
void init_config(void)
{
	unsigned long i=0;
	AT24C128_R_P(0xf000,&config_buf[0],50);//0000 f000
	
	hw_date=((config_buf[0]<<24)&0xff000000) | ((config_buf[1]<<16)&0x00ff0000) | ((config_buf[2]<<8)&0x0000ff00) | ((config_buf[3]<<0)&0x000000ff); 
  hw_ver_shi=config_buf[4];
	hw_ver_ge =config_buf[5];
	
	fw_date=((config_buf[6]<<24)&0xff000000) | ((config_buf[7]<<16)&0x00ff0000) | ((config_buf[8]<<8)&0x0000ff00) | ((config_buf[9]<<0)&0x000000ff); 
  fw_ver_shi=config_buf[10];
	fw_ver_ge =config_buf[11];
	
	vtorch_fw_number=((config_buf[12]<<24)&0xff000000) | ((config_buf[13]<<16)&0x00ff0000) | ((config_buf[14]<<8)&0x0000ff00) | ((config_buf[15]<<0)&0x000000ff); 
	//=============读取TESTERID========================
	for(i=0xf010;i<=0xf015;i++){//0xf010--0xf015 
	 	config_buf[i-0xf010+16]=AT24C128_R(i);//config_buf[16--21]
	}
	Z_Test_LC=Read_Test_LC();                     //0x04 0x05 0x06 0x07获取z轴测试位置
	Z_Ready_LC=Read_Ready_LC();                   //0x08 0x09 0x0a 0x0b获取z轴预备位置
	Motor_Speed=GetZSValue();                     //0x0c 0x0d 0x0e 0x0f获取z轴电机速度
	Initial_Zaxis_Plus=Get_Z_Initial_Position();  //读取eeprom初始脉冲数0x17 0x18 0x19 0x1A
	Uart_Printf("Z_Test_LC=%d\r\n",Z_Test_LC);	  
	Uart_Printf("Z_Ready_LC=%d\r\n",Z_Ready_LC);	  
	Uart_Printf("Motor_Speed=%d\r\n",Motor_Speed);//读取电机速度
	Uart_Printf("Initial_Zaxis_Plus=%d\r\n",Initial_Zaxis_Plus);		
	
}
//======================================
void IICDelay(U32 i){
 	while(i--);
}
//======================================
void AT24C128_W(U16 addr,U8 dat){
	I2C_Start();
	I2C_Puts(0xa0);
	I2C_Puts(addr/256 );
	I2C_Puts(addr%256 );
	I2C_Puts(dat);
	I2C_Stop();
	IICDelay(80000);
}
//======================================
U8 AT24C128_R(U16 addr){
    U8 dat; 
	I2C_Start();
	I2C_Puts(0xa0);
	I2C_Puts(addr/256 );
	I2C_Puts(addr%256 );
	I2C_Start();
	I2C_Puts(0xa1);
	dat=I2C_Read();
	I2C_PutAck(1);
	I2C_Stop();
	return dat;
}
//======================================
void AT24C128_W_P(U16 addr,U8 *dat,U8 len){
	I2C_Start();
	I2C_Puts(0xa0);
	I2C_Puts(addr/256 );
	I2C_Puts(addr%256 );
	while(len--){
		I2C_Puts(*dat++);
	}
	I2C_Stop();
	IICDelay(80000);
}
//======================================
void AT24C128_R_P(U16 addr,U8 *dat,U8 len){
    U8 i=0;
	for(i=0;i<len;i++)*dat++=AT24C128_R(addr+i);
}
//======================================
void eepromtest(){
	U8 i=0;
	U8 dat[100];
	for(i=0;i<100;i++)dat[i]=77;//AT24C128_W(i,i);
    for(i=0;i<=31;i++)AT24C128_W_P(0,dat,32);
    for(i=32;i<=35;i++)AT24C128_W_P(32,&dat[32],4);
//	for(i=0;i<100;i++)dat[i]=0;
	//AT24C128_R_P(0,dat,35);
	for(i=0;i<100;i++)dat[i]=AT24C128_R(i);
	for(i=0;i<100;i++){
	   Uart_Printf("dat[%d]=%d\n",i,AT24C128_R(i));
	}
}
//======================================
void eepromwrite(){
	U8 i=0;
	U8 dat[35];
	for(i=0;i<35;i++)dat[i]=i;
    AT24C128_W_P(0,dat,32);
	AT24C128_W_P(32,&dat[32],4);
	Uart_Printf("ok\n");
}
//======================================
void eepromread(){
	U8 i=0;
	U8 dat[35];
	for(i=0;i<35;i++)dat[i]=0;
	for(i=0;i<35;i++)dat[i]=AT24C128_R(i);
	for(i=0;i<35;i++){
	   Uart_Printf("dat[%d]=%d\n",i,dat[i]);
	}
}
//=========================================================
void Save_Test_LC(uint32_t test_lc){//0x0004--0x0007
	
	uint8_t dat[4]={0};
  Z_Test_LC=test_lc;
	dat[0]=test_lc%256;
	dat[1]=(test_lc>>8)%256;
	dat[2]=(test_lc>>16)%256;
	dat[3]=(test_lc>>24)%256;
	AT24C128_W(0x04,dat[0]);
	AT24C128_W(0x05,dat[1]);
	AT24C128_W(0x06,dat[2]);
	AT24C128_W(0x07,dat[3]);
	
}
//=========================================================
uint32_t Read_Test_LC(void){//0x0004--0x0007 
	uint32_t plus=0;
  plus=AT24C128_R(4);
	plus |=(AT24C128_R(5)<<8);
	plus |=(AT24C128_R(6)<<16);
	plus |=(AT24C128_R(7)<<24);
	Z_Test_LC=plus;
  return plus;
}
//=========================================================
void Save_Ready_LC(uint32_t ready_lc)//0x0008--0x000b
{
  uint8_t dat[4]={0};
  Z_Ready_LC=ready_lc;
	dat[0]=ready_lc%256;
	dat[1]=(ready_lc>>8)%256;
	dat[2]=(ready_lc>>16)%256;
	dat[3]=(ready_lc>>24)%256;
	AT24C128_W(0x08,dat[0]);
	AT24C128_W(0x09,dat[1]);
	AT24C128_W(0x0a,dat[2]);
	AT24C128_W(0x0b,dat[3]);
}
//=========================================================
uint32_t Read_Ready_LC(void){//0x0008--0x000b
	uint32_t plus=0;
  plus=AT24C128_R(0x08);
	plus |=(AT24C128_R(0x09)<<8);
	plus |=(AT24C128_R(0x0a)<<16);
	plus |=(AT24C128_R(0x0b)<<24);
	Z_Ready_LC=plus;
  return plus;
}
//=========================================================
//Z Motor Speed点位模式速度  0x000c--0x000f
void SaveZS(uint32_t time1){
	uint8_t dat[4]={0};
	dat[0]=time1%256;
	dat[1]=(time1>>8)%256;
	dat[2]=(time1>>16)%256;
	dat[3]=(time1>>24)%256;
	AT24C128_W(0x000f,dat[0]);
	AT24C128_W(0x000e,dat[1]);
	AT24C128_W(0x000d,dat[2]);
	AT24C128_W(0x000c,dat[3]);
}
//=========================================================
//Z Motor speed  0x000c--0x000f
uint32_t GetZSValue(void){
	uint32_t time1=0;
  time1=AT24C128_R(0x000f);
	time1 |=(AT24C128_R(0x000e)<<8);
	time1 |=(AT24C128_R(0x000d)<<16);
	time1 |=(AT24C128_R(0x000c)<<24);
	return time1;
}
//=========================================================
uint32_t Get_Z_Initial_Position(void){
	uint32_t plus=0;
  plus=AT24C128_R(0x17);
	plus |=(AT24C128_R(0x18)<<8);
	plus |=(AT24C128_R(0x19)<<16);
	plus |=(AT24C128_R(0x1A)<<24);
  return plus;	
}
//=========================================================
uint32_t Get_Motor_Initial_Position(void){
	uint32_t plus=0;
	plus=Get_Z_Initial_Position();
	Initial_Zaxis_Plus=plus;
	Uart_Printf("Initial_Zaxis_Plus=%d\r\n",Initial_Zaxis_Plus);
  return plus;	
}
//=========================================================
void Set_Z_Initial_Position(uint32_t Z_Plus){
 	uint8_t dat[4]={0};
	dat[0]= Z_Plus%256;
	dat[1]=(Z_Plus>>8)%256;
	dat[2]=(Z_Plus>>16)%256;
	dat[3]=(Z_Plus>>24)%256;
	AT24C128_W(0x17,dat[0]);
	AT24C128_W(0x18,dat[1]);
	AT24C128_W(0x19,dat[2]);
	AT24C128_W(0x1A,dat[3]);
	Initial_Zaxis_Plus=Get_Z_Initial_Position();//设置完成直接更新不需断电
}
