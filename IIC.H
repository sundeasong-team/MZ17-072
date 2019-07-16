#ifndef __IIC_H__
#define __IIC_H__


#define DLY    {unsigned int m=100;while(m--);}
#define SDA_H  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<27); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN | (1<<27);DLY;}
#define SDA_L  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<27); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN & (~(1<<27));DLY;}

#define SCL_H  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<28); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN | (1<<28);DLY;}
#define SCL_L  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<28); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN & (~(1<<28));DLY;}

	extern void I2C_Delay(void);
	extern void I2C_Init(void);
	extern void I2C_Start(void);
	extern void I2C_Stop(void);
	extern unsigned char I2C_Puts(unsigned char dat);
	extern unsigned char I2C_Read(void);
	extern unsigned char I2C_GetAck(void);
	extern void I2C_PutAck(unsigned char ack);

#endif


