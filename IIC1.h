#ifndef __IIC1_H__
#define __IIC1_H__

#define DLY1    {unsigned int m=100;while(m--);}
#define SDA1_H  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<19); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN | (1<<19);DLY1;}
#define SDA1_L  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<19); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN & (~(1<<19));DLY1;}

#define SCL1_H  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<20); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN | (1<<20);DLY1;}
#define SCL1_L  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<20); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN & (~(1<<20));DLY1;}

extern void I2C1_Delay(void);
extern void I2C1_Init(void);
extern void I2C1_Start(void);
extern void I2C1_Stop(void);
extern unsigned char I2C1_Puts(unsigned char dat);
extern unsigned char I2C1_Read(void);
extern unsigned char I2C1_GetAck(void);
extern void I2C1_PutAck(unsigned char ack);

#endif


