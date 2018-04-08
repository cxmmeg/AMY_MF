#include "config.h"

const u8 NUM[32]={0x1f,0x08,0x06,0x00,0x1b,0x10,0x0f,0x10,0x06,0x18,0x0d,0x18
,0x1d,0x18,0x07,0x00,0x1f,0x18,0x0f,0x18,0x17,0x18,0x1c,0x18,0x19,0x08,0x1e,0x10,0x19,0x18,0x11,0x18};
//����Ϊ����0~F
const u8 NUM_d[2]={0x1e,0x10};	//��ʾ����"d"����ʾģ���ַ
const u8 NUM_p[2]={0x13,0x18};	//��ʾ����"p"����ʾģ��Ƶ��
const u8 NUM_n[2]={0x17,0x08};	//��ʾ����"n"����ʾ�ն���������
const u8 NUM_L[2]={0x1c,0x18};	//��ʾ����"L"����ʾ���Ӳ�������
u8 TM1618_DISPLAY_NO[3]={0};
u8 change_led_flag=0;//ʹ��ʾ��˸
u8 key_delay_out=0;//������ʱ��δ�ж������Զ��˳����ý���
/****************************************************************************
* Function Name  : Tm618_Init
* Description    : ��ʼ��GPIO.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
void Tm618_IO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=TM1618_SCL|TM1618_SDA|TM1618_STB;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	TM1618_SCL_H;
	TM1618_SDA_H;
	TM1618_STB_H;
}
/*******************************************************************************
* �� �� ��         : TM1618_SDA_OUT
* ��������		   : SDA�������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TM1618_SDA_OUT()
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=TM1618_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : TM1618_SDA_IN
* ��������		   : SDA��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TM1618_SDA_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=TM1618_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOD,TM1618_SDA);
}
/*******************************************************************************
* �� �� ��         : Write_Byte
* ��������		   : д8λ�ֽ�
* ��    ��         : 8λ����ֵ
* ��    ��         : ��
*******************************************************************************/
void Write_Byte(u8 byte)
{  
    u8 i=0;  
    for(i=0;i<8;i++)
		{  
        TM1618_SCL_L; 
				Delay_uS(100);
        if(byte&0x01){  
            TM1618_SDA_H; 
						Delay_uS(100);
        }else{  
            TM1618_SDA_L;
						Delay_uS(100);
        }  
        TM1618_SCL_H; 
					Delay_uS(100);
        byte>>=1;  
    }    
} 
/*******************************************************************************
* �� �� ��         : Write_Cmd
* ��������		   : д����   
* ��    ��         : 8λ����ֵ
* ��    ��         : ��
*******************************************************************************/
void Write_Cmd(u8 cmd)
{  
    TM1618_STB_H;
		Delay_uS(100);
    TM1618_STB_L;
		Delay_uS(100);
    Write_Byte(cmd);
  
} 

/*******************************************************************************
* �� �� ��         : Tm618_Smg_No
* ��������		   : no_data=��ʾ�����֡�no_adr=��ʾ��ַ0~3 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Smg_No(u8 no_data,u8 no_adr)
{
//	u8 number=8;

	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40); 
	Write_Cmd((0xc0+(no_adr*2)));
 

      Write_Byte(NUM[no_data*2]); 
			Write_Byte(NUM[(no_data*2)+1]);

		Write_Cmd(0x8F);
		TM1618_STB_H;
} 

/*******************************************************************************
* �� �� ��         : Tm618_Display
* ��������		   : no_data=��ʾ�����֡�no_adr=��ʾ��ַ0~3 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Display()
{
	u8 i;
	u8 n=0;
	u8 p[2];
	u8 t[2];
	u8 f[2];
	
				switch(TM1618_DISPLAY_NO[0])
			{
				case 0:
				{

					if((amy_config&0x02)==0x02)//���簴Ħ��ģ�������أ�
					{
							p[0]=NUM_n[0];
							p[1]=NUM_n[1];
								if((amy_config&0x20)>0)//
								{p[1]=p[1]|0x10;//G��
								}
								if((amy_config&0x40)>0)
								{p[1]=p[1]|0x20;//dp
									
								}
								
								TM1618_DISPLAY_NO[1]=(amy_total>>4);
								TM1618_DISPLAY_NO[2]=(amy_total&0x0f);
								i=amy_total;	
								if(amy_max_total<=100)
									{
										u8 tt;
										tt=i;
										n=tt/10;
										t[0]=NUM[(n*2)]; 
										t[1]=NUM[(n*2)+1];	
									
										n=tt%10;
										f[0]=NUM[(n*2)]; 
										f[1]=NUM[(n*2)+1];	
									}
									else
									{
										n=i>>4;
										t[0]=NUM[(n*2)]; 
										t[1]=NUM[(n*2)+1];	
									
										n=i&0x0f;
										f[0]=NUM[(n*2)]; 
										f[1]=NUM[(n*2)+1];	
									}
					}
					else//��Ħ��ģ�����ն�
					{u8 tt;
						i=(TM1618_DISPLAY_NO[1]<<4)+TM1618_DISPLAY_NO[2];			
										
								tt=i%100;
								n=i/100;
								p[0]=NUM[(n*2)]; 
								p[1]=NUM[(n*2)+1];	
								if(amy_config&0x10)////��ʾ����ܵ�һλdp��,��������
									{p[1]=p[1]|0x20;
									}	

								n=tt/10;
								t[0]=NUM[(n*2)]; 
								t[1]=NUM[(n*2)+1];	
								if(amy_config1&0x01)////��ʾ����ܵڶ�λdp��,Ƶ���ظ�
									{t[1]=t[1]|0x20;
									}	
									
								n=tt%10;
								f[0]=NUM[(n*2)]; 
								f[1]=NUM[(n*2)+1];	
								if(amy_config1&0x02)////��ʾ����ܵ���λdp��,ID�ظ�
									{f[1]=f[1]|0x20;
									}	


								if((amy_config&0x01)==0)////��ʾ�����G�Σ�433������
									{	p[0]=0x00;
										p[1]=0x10;
										t[0]=0x00;
										t[1]=0x10;
										f[0]=0x00;
										f[1]=0x10;
									}
								/*		p[0]=0x00;
										p[1]=0x00;
									if(amy_config&0x04)//��ʾ����ܵ�һλA��
									{p[0]=p[0]|0x01;
									}
									if(amy_config&0x10)////��ʾ����ܵ�һλG��,��������
									{p[1]=p[1]|0x10;
									}
									if((amy_config&0x01)==0)////��ʾ����ܵ�һλD�Σ�433������
									{p[0]=p[0]|0x08;
									}
									if(amy_config1&0x01)////��ʾ����ܵ�һλdp�Σ�Ƶ���ظ�
									{p[1]=p[1]|0x20;
									}
									if(amy_config1&0x02)////��ʾ����ܵ�һλf�Σ�ID�ظ�
									{p[1]=p[1]|0x08;
									}
									if(amy_config==0x01)
									{
										if(amy_config1==0x00)
										{
										p[0]=NUM[0];
										p[1]=NUM[1];
										}
									}*/
								
				}
				}break;
				case 1:
				{
				p[0]=NUM_d[0];
				p[1]=NUM_d[1];
				}break;
				case 2:
				{
				p[0]=NUM_p[0];
				p[1]=NUM_p[1];
				}break;
				default: break;
			}
			
	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40); 
	Write_Cmd(0xc0);
	

				
			Write_Byte(p[0]); 
			Write_Byte(p[1]);

		 
			if(TM1618_DISPLAY_NO[0]==0)//������ʾ
						{
							if((amy_config&0x02)==0x02)//���簴Ħ��ģ�������أ�
							{
								Write_Byte(t[0]); 
								Write_Byte(t[1]);
								
								Write_Byte(f[0]); 
								Write_Byte(f[1]);
							}
							else
							{
								Write_Byte(t[0]); 
								Write_Byte(t[1]);
								
								Write_Byte(f[0]); 
								Write_Byte(f[1]);
							}

						}	
							else				//��˸��ʾ
							{
								if(change_led_flag==0)
								{		change_led_flag=1;
									if(amy_max_total>100)
									{
										for(i=1;i<3;i++)
											{
											n=TM1618_DISPLAY_NO[i];
											Write_Byte(NUM[(n*2)]); 
											Write_Byte(NUM[(n*2)+1]);
											}
									}
									else
									{
										i=(TM1618_DISPLAY_NO[1]<<4)+TM1618_DISPLAY_NO[2];			
										if(i==0xff)
										{
										for(i=1;i<3;i++)
											{
											n=TM1618_DISPLAY_NO[i];
											Write_Byte(NUM[(n*2)]); 
											Write_Byte(NUM[(n*2)+1]);
											}	
										}
										else
										{
										n=i/10;
										t[0]=NUM[(n*2)]; 
										t[1]=NUM[(n*2)+1];	
										if(amy_config1&0x01)////��ʾ����ܵڶ�λdp��,Ƶ���ظ�
											{p[1]=p[1]|0x20;
											}	
											
										n=i%10;
										f[0]=NUM[(n*2)]; 
										f[1]=NUM[(n*2)+1];
											
											Write_Byte(t[0]); 
											Write_Byte(t[1]);
											
											Write_Byte(f[0]); 
											Write_Byte(f[1]);
										}
											
									}

								}
								else
								{change_led_flag=0;
									for(i=0;i<2;i++)
										{
										Write_Byte(0x00); 
										Write_Byte(0x00);
										}							
								}
							}
			
	
		
		Write_Cmd(0x8F);
		TM1618_STB_H;
} 
/*******************************************************************************
* �� �� ��         : Tm618_Smg_clear
* ��������		   :  �����ʾ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Smg_clear()
{
	u8 i=0;

	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40); 
	Write_Cmd(0xc0);
 
	for(i=0;i<3;i++)
	{
      Write_Byte(0x0); 
			Write_Byte(0x0);
	}
		Write_Cmd(0x8F);
		TM1618_STB_H;
} 

/*******************************************************************************
* �� �� ��         : Tm618_Smg_8881 �޵�
* ��������		   :  ���������ȫ����ʾ888
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Smg_8881()
{

	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40); 
	Write_Cmd(0xc0);

      Write_Byte(0x1f); 
			Write_Byte(0x18);
	
	 Write_Byte(0x1f); 
			Write_Byte(0x18);
	
	 Write_Byte(0x1f); 
			Write_Byte(0x18);
		Write_Cmd(0x8F);
		TM1618_STB_H;
} 
/*******************************************************************************
* �� �� ��         : Tm618_Smg_8882 �е�
* ��������		   :  ���������ȫ����ʾ888
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Smg_8882()
{

	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40); 
	Write_Cmd(0xc0);

      Write_Byte(0xff); 
			Write_Byte(0xff);
	
	Write_Byte(0xff); 
			Write_Byte(0xff);
	
	Write_Byte(0xff); 
			Write_Byte(0xff);
		Write_Cmd(0x8F);
		TM1618_STB_H;
} 
/*******************************************************************************
* �� �� ��         : Tm618_Smg_On
* ��������		   :  ��������������ݣ�0x1f 0x0f   0x17 0x0f
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Smg_On()
{

	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40); 
	Write_Cmd(0xc0);

      Write_Byte(0x1f); 
			Write_Byte(0x0f);
		
			Write_Byte(0x17); 
			Write_Byte(0x0f);
	
		Write_Byte(0x00); 
			Write_Byte(0x00);
		Write_Cmd(0x8F);
		TM1618_STB_H;
} 
/*******************************************************************************
* �� �� ��         : Tm618_Smg_Line
* ��������		   :  ��⡰-�� ����0x00 0x10 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Tm618_Smg_Line(u8 no_adr)
{


	TM1618_SDA_OUT();//����SDA�˿�Ϊ���ģʽ
		Write_Cmd(0x00); 
  Write_Cmd(0x40+no_adr); 
	Write_Cmd((0xc0+(no_adr*2)));
 

      Write_Byte(0x00); 
			Write_Byte(0x10);

		Write_Cmd(0x8F);
		TM1618_STB_H;
} 
/*******************************************************************************
* �� �� ��         :rec_key
* ��������		   : ������ֵ�ӳ���һ������λ  
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 rec_key()
{
	u8 i;
	u8 k=0;
		for(i=0;i<8;i++)
			{  
				k=k>>1;
				TM1618_SCL_L;
				Delay_uS(500);		

				TM1618_SCL_H;	
				Delay_uS(500);
					if((GPIO_ReadInputDataBit(GPIO_TM1618,TM1618_SDA))==1)
					{  
            k=k|0x80;
						Delay_uS(100);
						
					}					
       } 

return k;
}
/*******************************************************************************
* �� �� ��         : read_key
* ��������		   : ������  
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 read_key()
{	
	u8 j;
u8 i;
	u8 k=0;
	TM1618_SDA_OUT();
		Write_Cmd(0x42);
		Delay_uS(100);
				TM1618_SDA_IN();//����SDA�˿�Ϊ����ģʽ
		for(j=0;j<3;j++)
	{
		i=rec_key();
		if(i&0x0f)
			k=2*j+1;
		else if(i&0xf0)
			k=2*j+2;
	}

		TM1618_STB_H;
		
return k;
}

/*
//������ʼ�ź�
void I2C_Start(void)
{
    I2C_SDA_OUT();
	
	I2C_SDA_H;
	I2C_SCL_H;
	delay_us(5);
	I2C_SDA_L;
	delay_us(6);
	I2C_SCL_L;
}
*/
