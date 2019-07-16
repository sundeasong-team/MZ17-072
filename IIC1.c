#include "lpc17xx.h"
#include "IIC1.h"
//=============================================================================
#define  READ    1
#define  WRITE   1
//=============================================================================
//SDA=P0.27
//SCL=P0.28

#define U8  unsigned char 
#define U16 unsigned short
#define U32 unsigned int

/******************************************************************************
������I2C1_Delay()
���ܣ�ģ��I2C������ʱ
˵��������ݾ������������ʱֵ
******************************************************************************/
void I2C1_Delay(){
	unsigned int t;
	t = 10;//
	while ( --t != 0 );		//��ʱ2*t����������
}
//=============================================================================
U8 SDA1()
{	
   	LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) & (~(1<<19));
		return ((LPC_GPIO0->FIOPIN >>19)& 0x01);
}
/******************************************************************************
������I2C1_Init()
���ܣ�I2C���߳�ʼ����ʹ���ߴ��ڿ���״̬
˵������main()�����Ŀ�ʼ����Ӧ��ִ��һ�α�����
******************************************************************************/
void I2C1_Init(){
  LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | ((1<<19)|(1<<20));
	SCL1_H;
	I2C1_Delay();
	SDA1_H;
	I2C1_Delay();
}


/******************************************************************************
������I2C1_Start()
���ܣ�����I2C���ߵ���ʼ����
˵����
	SCL���ڸߵ�ƽ�ڼ䣬��SDA�����½���ʱ����I2C����
	������Ҳ���������ظ���ʼ����
******************************************************************************/
void I2C1_Start(){
	SDA1_H;
	I2C1_Delay();
	SCL1_H;
	I2C1_Delay();
	SDA1_L;
	I2C1_Delay();
	SCL1_L;
	I2C1_Delay();
}
/******************************************************************************
������I2C1_Write()
���ܣ���I2C����д1���ֽڵ�����
������dat��Ҫд�������ϵ�����
******************************************************************************/
void I2C1_Write(unsigned char dat){
 	unsigned char t = 8;
	do{
		if(dat&0x80)SDA1_H
		else SDA1_L
		dat <<= 1;
		SCL1_H;
		I2C1_Delay();
		SCL1_L;
		I2C1_Delay();
	} while ( --t != 0 );

}
/******************************************************************************
������I2C1_GetAck()
���ܣ���ȡ�ӻ�Ӧ��λ��Ӧ����Ӧ�𣩣������жϣ��ӻ��Ƿ�ɹ�������������
���أ�
	0���ӻ�Ӧ��
	1���ӻ���Ӧ��
˵�����ӻ����յ�ÿһ���ֽں�Ҫ����Ӧ��λ����������յ���Ӧ����Ӧ����ֹ����
******************************************************************************/
U8 I2C1_GetAck(){
	U8 ack;
	SDA1_H;
	I2C1_Delay();
	SCL1_H;
	I2C1_Delay();
	ack = SDA1();
	SCL1_L;
	I2C1_Delay();
	return ack;
}
/******************************************************************************
������I2C1_Stop()
���ܣ�����I2C���ߵ�ֹͣ����
˵����SCL���ڸߵ�ƽ�ڼ䣬��SDA����������ʱֹͣI2C����
******************************************************************************/
void I2C1_Stop(){
	unsigned int t;
	SDA1_L;
	I2C1_Delay();
	SCL1_H;
	I2C1_Delay();
	SDA1_H;
	I2C1_Delay();
    //����ĳЩ������˵������һ�β���Start֮ǰ����������һ������ʱ�Ǳ����
	t = 15;
	while ( --t != 0 );
}

/******************************************************************************
������I2C1_Puts()
���ܣ�����ͨ��I2C������ӻ�����1���ֽڵ�����
������
	SlaveAddr���ӻ���ַ����7λ�Ǵӻ���ַ�����λ�Ƕ�д��־��
	SubAddr���ӻ����ӵ�ַ
	size�����ݴ�С�����ֽڼƣ�
	dat��Ҫ���͵�����
���أ�
	0�����ͳɹ�
	1���ڷ��͹����г����쳣
******************************************************************************/
U8 I2C1_Puts(unsigned char dat){
	I2C1_Write(dat);
	if ( I2C1_GetAck() ){
		I2C1_Stop();
		return 1;
	}
	return 0;
}
/******************************************************************************
������I2C1_PutAck()
���ܣ���������Ӧ��λ��Ӧ����Ӧ�𣩣�����֪ͨ�ӻ��������Ƿ�ɹ����մӻ�����
������
	ack=0��������Ӧ��
	ack=1������Ӧ��
˵�����������յ�ÿһ���ֽں�Ҫ����Ӧ�����յ����һ���ֽں�Ӧ��������Ӧ��
******************************************************************************/
void I2C1_PutAck(U8 ack){
  if(ack)SDA1_H
	else SDA1_L
	I2C1_Delay();
	SCL1_H;
	I2C1_Delay();
	SCL1_L;
	I2C1_Delay();
}
/******************************************************************************
������I2C1_Read()
���ܣ��Ӵӻ���ȡ1���ֽڵ�����
���أ���ȡ��1���ֽ�����
******************************************************************************/
unsigned char I2C1_Read(){
	unsigned char dat;
	unsigned char t = 8;
	SDA1_H;	//�ڶ�ȡ����֮ǰ��Ҫ��SDA���ߣ�ʹ֮��������״̬
	do{
		SCL1_H;
		I2C1_Delay();
		dat <<= 1;
		if( SDA1() ) dat++;
		SCL1_L;
		I2C1_Delay();
	} while ( --t != 0 );
	return dat;
}