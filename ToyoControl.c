 /***********************************************************************************************
 Ŀ��壺ControlBoard V1.0������
 �����ܣ�ToyoЭ�鹦�ܿ�  Modbusͨ��Э��  19200 n.8.1
 ���̽������ڣ�2017.10.16
 ��д�ˣ�
 �޸�1��
 �޸�2��
*************************************************************************************************/
#include "ToyoControl.h"
#include <string.h>	
#include "stdio.h"
#include "delay.h"
#include "Uart.H"
#include "Uart2.h"//485
#include "repeathandle.h"
#define TIME_OUT 255								 		    //��ʱʱ�� 0-255
unsigned char Rx_String[100];	  				 		//���ܵ����ַ��� ȥ��lrcУ�鼰�س�����
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
* Description    : ��������ģʽ��Чָ��
* Parameter		   : վ��ַ 1 or 2 or 3 ʮ����
* Return		     : TRUE FALSE
********************************************************************************/
// unsigned char IAI_Set_OrderEnable(unsigned char AdressNum)						
// {
// 	unsigned char Tx_String[]={':','0','0','0','5','0','4','2','7','F','F','0','0','\0'};													
// 	Tx_String[2]=AdressNum|0x30;						        //��ַת��
// 	return Get_Send_Command(AdressNum,Tx_String);		//���ͺͽ���һ������0
// }
/*******************************************************************************
* Function Name  : 2011
* Description    : �����ŷ�ONָ��
* Parameter		 : վ��ַ 1 or 2 or 3
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
* Description    : �����ŷ�OFFָ��
* Parameter		 : վ��ַ 1 or 2 or 3
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
* Description    : ��ԭ��ָ��	   ˫ָ��
* Parameter		   : վ��ַ 1 or 2 or 3
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
* Description    : ���õ�ǰ�ٶ�ָ��
* Parameter		 : վ��ַ 1 or 2 or 3	 �ٶ�	1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Set_Speed(unsigned char AdressNum,unsigned char CSpeed ) 								
{
	unsigned char Tx_String[]={':','0','0','0','6','2','0','1','4','0','0','0','0','\0'};
	Tx_String[2]=AdressNum|0x30;
  Tx_String[12]=Ascill_16[CSpeed%16];						  //ȡ���� ���ȡ�ַ�
	Tx_String[11]=Ascill_16[CSpeed/16%16];					//ȡ���� ���ȡ�ַ�
	Tx_String[10]=Ascill_16[CSpeed/256%16];					//ȡ���� ���ȡ�ַ�
	Tx_String[9]= Ascill_16[CSpeed/4096%16];			  //ȡ���� ���ȡ�ַ�
	return Get_Send_WData(AdressNum,Tx_String);

}
/*******************************************************************************
* Function Name  : :0105040AFF00
* Description    : ������ָͣ��
* Parameter		 : վ��ַ 1 or 2 or 3
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
* Description    : �����������ָ��	  ˫ָ��
* Parameter		 : վ��ַ 1 or 2 or 3
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
* Description    : �����ָ��
* Parameter		 : վ��ַ 1 or 2 or 3
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
* Description    : ��ȡ��ǰ���������״ָ̬��
* Parameter		   : վ��ַ 1 or 2 or 3
* Return		     : ��ǰ״̬ 8421λ״̬
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
	return 0xff;//��������»᷵�ط�0ֵ
}

/*******************************************************************************
* Function Name  : 100A
* Description    : ��ȡ��ǰλ��ָ��
* Parameter		 : վ��ַ 1 or 2 or 3
* Return		 : ��ǰλ�� ʮ����	 ����ֵ/100 ����ǰλ�� 200.00
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
	return 0xffff;//������
	}

/*******************************************************************************
* Function Name  : 1005
* Description    : ��ȡ�������ָ��
* Parameter		   : վ��ַ 1 or 2 or 3
* Return		     : ������� ʮ������
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
		return HEX1*4096+HEX2*256+HEX3*16+HEX4;//��������0
	}
	return 0xffff;//��������»᷵�ط�0ֵ
}
/*******************************************************************************
* Function Name  : 
* Description    : ֻ��Rx_Flag�Ƿ�Ϊ1
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
		 	Delay_MS(10);//���Ͷ�μ������7MS
			Time_Out++;
		 }

		 if(Time_Out >= TIME_OUT){
// 			  Uart_Printf("Rx_Flag=%d\r\n",Rx_Flag);
// 			  Print_String("Get_Send_Flag timeout\r\n");
			 	return 1;			 						//��ʱ�˳�
		 }			 
		 else			   
		 	return 0;	
	}
	else
		return 3;
}
/*******************************************************************************
* Function Name  : 
* Description    : ����д�����ݺ��ȷ������
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
			return 1;			 	                          //��ʱ�˳�
		 }
		 else
		 {
		 	 WData[13]='\0';								          //���ݲ��Ƚ�ֻ�Ȳ���λ
			 if(StringCompare(&WData[0])==0)	  			//��д�ַ����Ƚ�
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
* Description    : ��ָ��λ��ָ��
* Parameter		   : վ��ַ 1 or 2 or 3
* Return		     : TRUE FALSE
********************************************************************************/
unsigned char Toyo_Go_AssignPoint(unsigned char AdressNum,unsigned int Point)
{
  unsigned char Tx_String[]={':','0','0','1','0','2','0','0','2','0','0','0','2','0','4','0','0','0','0','0','0','0','0','\0'};
	unsigned char Tx_String1[]={':','0','0','0','6','2','0','1','E','0','0','0','1','\0'}; 
	Tx_String[2]=AdressNum|0x30;	
	Tx_String1[2]=AdressNum|0x30;
	//λ�����ݶ�
	Tx_String[22]=Ascill_16[(Point&0x0000000f)];						  //ȡ���� ���ȡ�ַ�
	Tx_String[21]=Ascill_16[(Point&0x000000f0) >>4];					//ȡ����
	Tx_String[20]=Ascill_16[(Point&0x00000f00) >>8];					//ȡ����
	Tx_String[19]=Ascill_16[(Point&0x0000f000) >>12];					//ȡ����

	Tx_String[18]=Ascill_16[(Point&0x000f0000) >>16];					//ȡ���� ���ȡ�ַ�
	Tx_String[17]=Ascill_16[(Point&0x00f00000) >>20];					//ȡ����
	Tx_String[16]=Ascill_16[(Point&0x0f000000) >>24];					//ȡ����
	Tx_String[15]=Ascill_16[(Point&0xf0000000) >>28];					//ȡ����
	if(Get_Send_WData(AdressNum,Tx_String)==0){	
		Delay_MS(10);
		return Get_Send_WData(AdressNum,Tx_String1);
	}
  else
		return 1;
}
/*******************************************************************************
* Function Name  : 
* Description    : ���ط��ͺ��ȷ������
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
		 while((Rx_Flag!=1)&&(Time_Out<TIME_OUT))//�տ�ʼRx_Flag=0,�ȴ������ж�����Rx_Flag=1,TIME_OUTӦС��255
		 {
		 	Delay_MS(1);
			Time_Out++;
		 }
		 if (Time_Out >= TIME_OUT){//����255
			 Uart_Printf("Rx_Flag=%d\r\n",Rx_Flag);
			 Print_String("Get_Send_Command timeout\r\n");
			 return 1;//��ʱ�˳�,������������˳�Time_Out=5
			 
		 }
		 else
		 {
			 if(StringCompare(&Command[0])==0){	  		  	//��д�ַ����Ƚ�����==0
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
* Description    : ������ջ�������ȫ���ַ�����־λ
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
* Description    : �ַ����Ƚ� ��ȷ���0
********************************************************************************/
unsigned char StringCompare(unsigned char *Command)
{
  unsigned char i=0,flag=0;
	i=0;
	while( (*Command) != '\0')
	{		
		if((*Command)!=Rx_String[i])			//���ֽڱȽ�
		   flag++;

		Command++;	
		i++;
	}

	Clear_Rx_String();

	return flag;		   						//��Ϊ0���
}
