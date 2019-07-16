#ifndef __IIC2_H__
#define __IIC2_H__

#define DLY2    {unsigned int m=100;while(m--);}
#define SDA2_H  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<21); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN | (1<<21);DLY2;}
#define SDA2_L  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<21); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN & (~(1<<21));DLY2;}

#define SCL2_H  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<22); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN | (1<<22);DLY2;}
#define SCL2_L  { LPC_GPIO0->FIODIR=(LPC_GPIO0->FIODIR) | (1<<22); LPC_GPIO0->FIOPIN =LPC_GPIO0->FIOPIN & (~(1<<22));DLY2;}

extern void I2C2_Delay(void);
extern void I2C2_Init(void);
extern void I2C2_Start(void);
extern void I2C2_Stop(void);
extern unsigned char I2C2_Puts(unsigned char dat);
extern unsigned char I2C2_Read(void);
extern unsigned char I2C2_GetAck(void);
extern void I2C2_PutAck(unsigned char ack);

#endif


