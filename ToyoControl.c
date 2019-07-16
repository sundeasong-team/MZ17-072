 /***********************************************************************************************
 目标板：ControlBoard V1.0开发板
 程序功能：Toyo协议功能块  Modbus通信协议  19200 n.8.1
 工程建立日期：2017.10.16
 编写人：
 修改1：
 修改2：
*************************************************************************************************/
#include "ToyoControl.h"
#include <string.h>	
#include "stdio.h"
#include "delay.h"
#include "Uart.H"
#include "Uart2.h"//485
#include "repeathandle.h"
#define TIME_OUT 255								 		    //超时时间 0-255
unsigned char Rx_String[100];	  				 		//接受到的字符串 去除lrc校验及回车换行
unsigned char Rx_Flag;
extern unsigned char Ascill_16[];
/*******************************************************************************
* Function Name  :
* Description    : char to int
********************************************************************************/
unsigned char ChrToInt(char chr)
{
    unsigned char n=0;

    if (chr>='0' && chr<='9')
      {
	   n=chr-'0';
	  }
    else if (chr>='a' && chr<='f')
      {
	    n=chr-'a'+10;
	  }
    else if (chr>='A' && chr<='F')
      {
        n=chr-'A'+10;
      }
    return n;
}

/*******************************************************************************
* Function Name  : ":01050427FF00"
* Description    : 设置命令模式有效指令
* Parameter		   : 站地址 1 or 2 or 3 十进制
* Return		     : TRUE FALSE
********************************************************************************/
// unsigned char IAI_Set_OrderEnable(unsigned char AdressNum)						
// {
// 	unsigned char Tx_String[]={':','0','0','0','5','0','4','2','7','F','F','0','0','\0'};													
// 	Tx_String[2]=AdressNum|0x30;						        //地址转换
// 	return Get_Send_Command(AdressNum,Tx_String);		//发送和接收一样返回0
// }
/*******************************************************************************
* Function Name  : 2011
* Description    : 设置伺服ON指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Set_ServoON(unsigned char AdressNum)								
{
	unsigned char Tx_String[]={':','0','0','0','6','2','0','1','1','0','0','0','0','\0'};
	Tx_String[2]=AdressNum|0x30;
	return Get_Send_Command(AdressNum,Tx_String);

}
/*******************************************************************************
* Function Name  : 
* Description    : 设置伺服OFF指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Set_ServoOFF(unsigned char AdressNum)							
{
	unsigned char Tx_String[]={':','0','0','0','6','2','0','1','1','0','0','0','1','\0'};

	Tx_String[2]=AdressNum|0x30;

	return Get_Send_Command(AdressNum,Tx_String);
}

/*******************************************************************************
* Function Name  : :0105040B0000 :0105040BFF00
* Description    : 回原点指令	   双指令
* Parameter		   : 站地址 1 or 2 or 3
* Return		     : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Go_Origin(unsigned char AdressNum)
{
	unsigned char Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','3','\0'};
 	Tx_String1[2]=AdressNum|0x30;
	return Get_Send_Command(AdressNum,Tx_String1);

}
/*******************************************************************************
* Function Name  : 0-100%
* Description    : 设置当前速度指令
* Parameter		 : 站地址 1 or 2 or 3	 速度	1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Set_Speed(unsigned char AdressNum,unsigned char CSpeed ) 								
{
	unsigned char Tx_String[]={':','0','0','0','6','2','0','1','4','0','0','0','0','\0'};
	Tx_String[2]=AdressNum|0x30;
  Tx_String[12]=Ascill_16[CSpeed%16];						  //取余数 查表取字符
	Tx_String[11]=Ascill_16[CSpeed/16%16];					//取余数 查表取字符
	Tx_String[10]=Ascill_16[CSpeed/256%16];					//取余数 查表取字符
	Tx_String[9]= Ascill_16[CSpeed/4096%16];			  //取余数 查表取字符
	return Get_Send_WData(AdressNum,Tx_String);

}
/*******************************************************************************
* Function Name  : :0105040AFF00
* Description    : 设置暂停指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
// unsigned char IAI_Set_Pause(unsigned char AdressNum)
// {
// 	unsigned char Tx_String[]={':','0','0','0','5','0','4','0','3','0','0','0','0','\0'};

// 	Tx_String[2]=AdressNum|0x30;
// 	return Get_Send_Command(AdressNum,Tx_String);

// }
/*******************************************************************************
* Function Name  :
* Description    : 设置清除报警指令	  双指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Set_ClearArm(unsigned char AdressNum)
{
	unsigned char Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','6','\0'};
  Tx_String1[2]=AdressNum|0x30;
  return Get_Send_Command(AdressNum,Tx_String1);

}
/*******************************************************************************
* Function Name  : :0110990000020400000000
* Description    : 回零点指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
// unsigned char IAI_Go_Zero(unsigned char AdressNum)
// {
// 	unsigned char Tx_String[]={':','0','0','1','0','9','9','0','0','0','0','0','2','0','4','0','0','0','0','0','0','0','0','\0'};
// 	Tx_String[2]=AdressNum|0x30;
// 	Tx_String[19]='0';Tx_String[20]='0';
// 	Tx_String[21]='0';Tx_String[22]='0';	
// 	return Get_Send_WData(AdressNum,Tx_String);
// }
/*******************************************************************************
* Function Name  : 1020
* Description    : 读取当前电机控制器状态指令
* Parameter		   : 站地址 1 or 2 or 3
* Return		     : 当前状态 8421位状态
********************************************************************************/
unsigned int Toyo_Read_CurrentStatus(unsigned char AdressNum)
{
	unsigned char Tx_String[]={':','0','0','0','3','1','0','2','0','0','0','0','1','\0'};
	unsigned char HEX1,HEX2,HEX3,HEX4;

	Tx_String[2]=AdressNum|0x30;

	if(Get_Send_Flag(AdressNum, Tx_String) ==0 )
	{
		HEX1=ChrToInt(Rx_String[7]);
		HEX2=ChrToInt(Rx_String[8]);
		HEX3=ChrToInt(Rx_String[9]);
		HEX4=ChrToInt(Rx_String[10]);
		Clear_Rx_String();

		return HEX1*4096+HEX2*256+HEX3*16+HEX4;
	}
	return 0xff;//正常情况下会返回非0值
}

/*******************************************************************************
* Function Name  : 100A
* Description    : 读取当前位置指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : 当前位置 十进制	 返回值/100 即当前位置 200.00
********************************************************************************/
 int Toyo_Read_CurrentPoint(unsigned char AdressNum)
{
	unsigned char Tx_String[]={':','0','0','0','3','1','0','0','A','0','0','0','2','\0'};
	unsigned char HEX1,HEX2,HEX3,HEX4,HEX5,HEX6,HEX7,HEX8;

	Tx_String[2]=AdressNum|0x30;

	if( Get_Send_Flag(AdressNum, Tx_String)==0 )
	{
		HEX1=ChrToInt(Rx_String[7]);
		HEX2=ChrToInt(Rx_String[8]);
		HEX3=ChrToInt(Rx_String[9]);
		HEX4=ChrToInt(Rx_String[10]);
		
		HEX5=ChrToInt(Rx_String[11]);
		HEX6=ChrToInt(Rx_String[12]);
		HEX7=ChrToInt(Rx_String[13]);
		HEX8=ChrToInt(Rx_String[14]);
		Clear_Rx_String();
		
		return HEX1*268435456+ HEX2*16777216+HEX3*1048576+HEX4*65536+HEX5*4096+HEX6*256+HEX7*16+HEX8;
		//return HEX1*4096+HEX2*256+HEX3*16+HEX4;
	}
	return 0xffff;//出错返回
	}

/*******************************************************************************
* Function Name  : 1005
* Description    : 读取错误代码指令
* Parameter		   : 站地址 1 or 2 or 3
* Return		     : 错误代码 十六进制
********************************************************************************/
unsigned int Toyo_Read_ErrorCode(unsigned char AdressNum)
{
	unsigned char Tx_String[]={':','0','0','0','3','1','0','0','D','0','0','0','1','\0'};
	unsigned char HEX1,HEX2,HEX3,HEX4;
	Tx_String[2]=AdressNum|0x30;
	if( Get_Send_Flag(AdressNum, Tx_String)==0 )
	{
		HEX1=ChrToInt(Rx_String[7]);
		HEX2=ChrToInt(Rx_String[8]);
		HEX3=ChrToInt(Rx_String[9]);
		HEX4=ChrToInt(Rx_String[10]);
		Clear_Rx_String();
		return HEX1*4096+HEX2*256+HEX3*16+HEX4;//正常返回0
	}
	return 0xffff;//正常情况下会返回非0值
}
/*******************************************************************************
* Function Name  : 
* Description    : 只看Rx_Flag是否为1
* Parameter		 : 
* Return		 :
********************************************************************************/
unsigned char Get_Send_Flag(unsigned char AdressNum,unsigned char *Command)
{
	unsigned char Time_Out;

	if( ( AdressNum>0 ) && ( AdressNum< MAX_ADRESS) )
	{
		 Clear_Rx_String();
		 Uart2_Print_String((char*)Command);
		 Time_Out=0;
		 while((Rx_Flag!=1)&&(Time_Out<TIME_OUT))
		 {
		 	Delay_MS(10);//发送多次间隔最少7MS
			Time_Out++;
		 }

		 if(Time_Out >= TIME_OUT){
// 			  Uart_Printf("Rx_Flag=%d\r\n",Rx_Flag);
// 			  Print_String("Get_Send_Flag timeout\r\n");
			 	return 1;			 						//超时退出
		 }			 
		 else			   
		 	return 0;	
	}
	else
		return 3;
}
/*******************************************************************************
* Function Name  : 
* Description    : 返回写入数据后的确认命令
* Parameter		 : 
* Return		 :
********************************************************************************/
unsigned char Get_Send_WData(unsigned char AdressNum,unsigned char *WData)
{
	unsigned char Time_Out;
	if( ( AdressNum>0 ) && ( AdressNum< MAX_ADRESS) )
	{
		 Clear_Rx_String();
		 Uart2_Print_String((char*)WData);

		 Time_Out=0;
		 while((Rx_Flag!=1)&&(Time_Out<TIME_OUT))
		 {
		 	Delay_MS(1);
			Time_Out++;
		 }
		 if (Time_Out >= TIME_OUT) 
		 {
			Uart_Printf("Rx_Flag=%d\r\n",Rx_Flag);
			Print_String("Get_Send_WData timeout\r\n");
			return 1;			 	                          //超时退出
		 }
		 else
		 {
		 	 WData[13]='\0';								          //数据不比较只比操作位
			 if(StringCompare(&WData[0])==0)	  			//自写字符串比较
			 	return 0;
			 else
			 	return 2;
		 }
	}
	else
		return 3;
}
/*******************************************************************************
* Function Name  : 
* Description    : 回指定位置指令
* Parameter		   : 站地址 1 or 2 or 3
* Return		     : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Go_AssignPoint(unsigned char AdressNum,unsigned int Point)
{
  unsigned char Tx_String[]={':','0','0','1','0','2','0','0','2','0','0','0','2','0','4','0','0','0','0','0','0','0','0','\0'};
	unsigned char Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','1','\0'}; 
	Tx_String[2]=AdressNum|0x30;	
	Tx_String1[2]=AdressNum|0x30;
	//位置数据段
	Tx_String[22]=Ascill_16[(Point&0x0000000f)];						  //取余数 查表取字符
	Tx_String[21]=Ascill_16[(Point&0x000000f0) >>4];					//取余数
	Tx_String[20]=Ascill_16[(Point&0x00000f00) >>8];					//取余数
	Tx_String[19]=Ascill_16[(Point&0x0000f000) >>12];					//取余数

	Tx_String[18]=Ascill_16[(Point&0x000f0000) >>16];					//取余数 查表取字符
	Tx_String[17]=Ascill_16[(Point&0x00f00000) >>20];					//取余数
	Tx_String[16]=Ascill_16[(Point&0x0f000000) >>24];					//取余数
	Tx_String[15]=Ascill_16[(Point&0xf0000000) >>28];					//取余数
	if(Get_Send_WData(AdressNum,Tx_String)==0){	
		Delay_MS(10);
		return Get_Send_WData(AdressNum,Tx_String1);
	}
  else
		return 1;
}
/*******************************************************************************
* Function Name  : 
* Description    : 返回发送后的确认命令
* Parameter		   : 
* Return		     :
********************************************************************************/
unsigned char Get_Send_Command(unsigned char AdressNum,unsigned char *Command)//unsigned char Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','3','\0'}; Tx_String1[2]=AdressNum|0x30;
{
	unsigned char Time_Out;
	if( ( AdressNum>0 ) && ( AdressNum< MAX_ADRESS) )
	{
		 Clear_Rx_String();//Rx_Flag=0
		 Uart2_Print_String((char*)Command);
		 Time_Out=0;
		 while((Rx_Flag!=1)&&(Time_Out<TIME_OUT))//刚开始Rx_Flag=0,等待接收中断里面Rx_Flag=1,TIME_OUT应小于255
		 {
		 	Delay_MS(1);
			Time_Out++;
		 }
		 if (Time_Out >= TIME_OUT){//等于255
			 Uart_Printf("Rx_Flag=%d\r\n",Rx_Flag);
			 Print_String("Get_Send_Command timeout\r\n");
			 return 1;//超时退出,正常情况不会退出Time_Out=5
			 
		 }
		 else
		 {
			 if(StringCompare(&Command[0])==0){	  		  	//自写字符串比较正常==0
			 	return 0;
			 }
			 else{ 
				Uart_Printf("Rx_Flag=%d\r\n",Rx_Flag);
				Print_String("Get_Send_Command data err\r\n");
			 	return 2;
			 }
		 }
	}
	else{
		return 3;
	}
		
}
/*******************************************************************************
* Function Name  : 
* Description    : 清除接收缓存区的全部字符及标志位
********************************************************************************/
void Clear_Rx_String()
{
   unsigned char i=0;

	for(i=0;i<UART2_BUFSIZE;i++)	
	{
		Rx_String[i]=0x00;
	}
	Rx_Flag=0;
}
/*******************************************************************************
* Function Name  : 
* Description    : 字符串比较 相等返回0
********************************************************************************/
unsigned char StringCompare(unsigned char *Command)
{
  unsigned char i=0,flag=0;
	i=0;
	while( (*Command) != '\0')
	{		
		if((*Command)!=Rx_String[i])			//按字节比较
		   flag++;

		Command++;	
		i++;
	}

	Clear_Rx_String();

	return flag;		   						//不为0相等
}
