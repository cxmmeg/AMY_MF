
#include "config.h"
u8 AT_CPIN[]	="AT+CPIN?";		//��ѯSIM��״̬
u8 AT_CIPSTART[]="at+zipsetup=0,";//GPRS���ӷ�ʽ\GPRS���ӵ�ַ(�����û�ʵ��IP�޸�)\GPRS���ӵĶ˿�
u8 AT_CIPCLOSE[]="at+zpppclose";		//TCP���ӹر�
u8 AT_zpppopen[]	="at+zpppopen";		//������
u8 AT_zpppstatus[]="at+zpppstatus";//��ѯ����״̬
u8 AT_CIPSEND[]	="at+zipsend=0,";		//GPRS������Ϣ����
u8 AT_GETICCID[]	="AT+GETICCID";		//��ѯSIM����
u8 AT_CIMI[]	="AT+CIMI";		//��ѯSIM����





u8 light_TCP[22]={"171.208.222.113,6037"};
u8 SIM_ID[20]={0};
u8 cdma_RESET_flag=0;

u8 g_flag=0;//G����־
/*******************************************************************************
* �� �� ��         : CDMA_IO_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void CDMA_IO_Init()	  //�˿ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE); /* ����GPIOʱ�� */

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=CDMA_RESET|CDMA_ON_OFF;	 //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	  //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(CDMA_IO,&GPIO_InitStructure); /* ��ʼ��GPIO */	
	GPIO_ResetBits(CDMA_IO,CDMA_RESET);
	GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);

}
/*********************************************************************
 ** ��������: strsearch_h ()
 ** ��������: ��ָ���������������ҵ���ͬ������
 ** ��ڲ���: ptr2Ҫ���ҵ�����, ptr1��ǰ����
 ** ���ڲ���: 0-û�� �ҵ�   >1 ���ҵ�
 *********************************************************************/
u16 strsearch_h(u8 *ptr2,u8 *ptr1_at)//���ַ���*ptr2��*ptr1�е�λ��
//����������������ַ���*ptr2�Ƿ���ȫ������*ptr1��
//����:  0  û���ҵ�
//65535 �ӵ�N���ַ���ʼ��ͬ
{
u16 i,j,k;
        
      
	if(Rx==0) return(0);
	
	for(i=0,j=0;i<Rx;i++)
	{
        	if(ptr1_at[i]==ptr2[j])
       		{	//��һ���ַ���ͬ
        		for(k=i;k<Rx+1;k++,j++)
        		{

        			if(ptr2[j]==0)//�Ƚ���ȷ
        			{                return(k);               //����ֵ������������0
                                }
        			if(ptr1_at[k]!=ptr2[j]) break;

        		}
        		j=0;
        	}
	}
	  ////   qwl_sendstring("-------------------\n");
	  //   qwl_sendstring(ptr1_at);
		//  qwl_sendstring("-------------------\n");
	return(0);
}

/****************************************************************************************************************/
///////////////////////////////////////void clear_SystemBuf();������ڽ��ջ�����/////////////////////////////////
/****************************************************************************************************************/
void clear_SystemBuf()
{

	Rx=0;
}



//*******************************************************
//TP����
//******************************************************
void TP_GPRS(u8 *p,u8 *q)
{//u8 ip_init_word[]={"121.42.196.113,28070"};//112.74.98.39,8647����Ĭ��IP
u8 i=0;
while(*p)
  	{
  		qwl_net_sendchar(*p);
			qwl_sendchar(*p);
  		p++;
			i++;
			if(i>20)
				break;
	}
		i=0;
	//q=&ip_init_word[0];
	while(*q)
  	{
  		qwl_net_sendchar(*q);
			qwl_sendchar(*q);
  		q++;
			i++;
			if(i>22)
				break;
	}
	qwl_sendchar(0x0D);
  	qwl_sendchar(0x0A);
	qwl_net_sendchar(0x0D);
  	qwl_net_sendchar(0x0A);
}
//*******************************************************
//SIM800C��ʼ��
//*********************************************************
/*u8 GSM_init()
{	u8 i=0;
	qwl_net_sendstring("ATE0V1");//�رջ�������DCEΪOK��ʽ
	Delay_MS(1000);

	while(1)
	{

		led_reversal();
		clear_SystemBuf();
		qwl_net_sendstring(AT_CPIN);//��ѯSIM��״̬
		Delay_MS(1000);
		if(strsearch_h("READY",SystemBuf)==0)
		{ 
			i++;
			if(i>5)
			return 1;
		}
		else
		{break;
		}
	}
	clear_SystemBuf();
	qwl_net_sendstring(AT_CIPCLOSE);
	i=0;
	led_reversal();
		while(1)
			{
				Delay_MS(1000);
			if(strsearch_h("ERROR",SystemBuf)==0)
					{ 
						i++;
						if(i>5)
						return 2;
					}
					else
					{break;
					}
					
					if(strsearch_h("+ZPPPSTATUS: CLOSED",SystemBuf)==0)
					{ 
						i++;
						if(i>5)
						return 2;
					}
					else
					{break;
					}
			}
clear_SystemBuf();			
qwl_net_sendstring(AT_zpppopen);	
		i=0;
			led_reversal();
	while(1)
			{
				Delay_MS(1000);
			if(strsearch_h("OPENED",SystemBuf)==0)
					{ 
						i++;
						if(i>8)
						return 2;
					}
					else
					{break;
					}
			}	 
	clear_SystemBuf();
	TP_GPRS(AT_CIPSTART,light_ip);//����IP
			i=0;
			led_reversal();
			while(1)
			{
				Delay_MS(1000);
			if(strsearch_h("+ZTCPESTABLISHED:0",SystemBuf)==0)
					{ 
						i++;
						if(i>5)
						return 2;
					}
					else
					{break;
					}
					
					if(strsearch_h("ERROR",SystemBuf)==0)
					{ 
						i++;
						if(i>5)
						return 2;
					}
					else
					{return 2;
					}
			}	 
	return 0;
}

*/

/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/

void GSM_RESET(void)
{

	GPIO_ResetBits(CDMA_IO,CDMA_RESET);
	GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
	Delay_MS(1000);//�ֲ�ֻҪ20ms���Ҹ������ʱ��
	GPIO_SetBits(CDMA_IO,CDMA_RESET);
	GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
	Delay_MS(50);//�ֲ�ֻҪ10ms���Ҹ������ʱ��
	GPIO_SetBits(CDMA_IO,CDMA_RESET);
	GPIO_ResetBits(CDMA_IO,CDMA_ON_OFF);

}
/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/

void Start_GSM(void)
{
//	GSM_RESET();
//	Delay_MS(3000);
//  GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
//	GPIO_SetBits(CDMA_IO,CDMA_RESET);
	
}

/*******************************************************************************
* �� �� ��         : cdma_init()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_init()
{
	//u8 i;
		switch (net.setp_setp)
	{
			case 0:
			{
					clear_SystemBuf();
				qwl_net_sendstring("ATE0V1");//��ѯSIM��״̬
				net.delay=0;
				net.setp_setp=1;
				net.delay=0;
				net.delay_times=0;
			}break;
			case 1:
			{
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
						}
							if(strsearch_h("OK",SystemBuf)>0)
								{ qwl_sendstring("���Թرճɹ�");
									net.delay_times=0;
								net.setp_setp=2;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;
								}
			}break;
			case 2:
			{
				clear_SystemBuf();
				qwl_net_sendstring(AT_CPIN);//��ѯSIM��״̬
				net.delay=0;
				net.setp_setp=3;
				net.delay=0;
				//net.delay_times=0;
			}break;
			case 3:
			{
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							net.setp_setp=2;
							qwl_sendstring("��sim��״̬��ʱ");
							net.delay_times++;
							if(net.delay_times>12)
							{qwl_sendstring("sim��������");
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}	
							if(strsearch_h("READY",SystemBuf)>0)
								{ qwl_sendstring("��sim���ɹ�");
									net.delay_times=0;
								net.setp_setp=4;
								net.reconnect_setp=MODE_INIT;//������������
								net.delay=0;
									Delay_MS(500);
								}
							
						
			}break;
			case 4:
			{
				qwl_sendstring("��sim����");
				clear_SystemBuf();
				if(g_flag==0)
				qwl_net_sendstring(AT_GETICCID);//��ѯSIM������
				else
					qwl_net_sendstring(AT_CIMI);
				net.delay=0;
				net.setp_setp=5;
				net.delay=0;
				//net.delay_times=0;
			}break;
			case 5:
			{u16 j;
				u16 i;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							net.setp_setp=4;
							qwl_sendstring("��sim���ų�ʱ");
							net.delay_times++;
							if(net.delay_times>5)
							{qwl_sendstring("sim���Ų�����");
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}	
						if(g_flag==0)
						j=strsearch_h("+GETICCID:0x",SystemBuf);
						else
						{u8 qqq=0;
						j=strsearch_h("OK",SystemBuf);
							if(j>0)
							{
								for(i=0;i<Rx;i++)
								{
									if(SystemBuf[i]>=0x30)
										if(SystemBuf[i]<=0x39)
										{
											if(qqq>=5)
												break;
											qqq++;
										}
								}
								if(i>=5)
								j=i-5;
								else
									j=0;
							}
							
						}
							if(j>0)
								{ qwl_sendstring("��sim���ųɹ�");
									for(i=0;i<20;i++)
									{SIM_ID[i]=SystemBuf[j+i];
										qwl_sendchar(SIM_ID[i]);
									}
									
									net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=OPEN_GPRS;//����������������
								net.delay=0;
								}
							
						
			}break;
		
		default:break;
		}
		

/*
Start_GSM();
i=GSM_init();
	if(i==1)
			qwl_sendstring("��ѯSIM��״̬ʧ��");
	else if(i==2)
			qwl_sendstring("��������ʧ��");
	else if(i==0)
			qwl_sendstring("�ɹ�����");	
*/
//send_data();//ip_init_word);

}

/*******************************************************************************
* �� �� ��         : void light_eth_reconnect()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

u8 light_TCP_test[22]={"171.208.222.113,6037"};
void cdma_reconnect()
{
	switch (net.setp_setp)
	{
		case 0:
		{
			qwl_sendstring("����IP");
			clear_SystemBuf();
			if(exchange_platform.Platform_Choice == PLATFORM_CHOICE_PUBLIC)			
			TP_GPRS(AT_CIPSTART,light_TCP_test);//����IP
			else
			TP_GPRS(AT_CIPSTART,update_ip);//��������IP

			net.setp_setp=1;
			net.delay=0;
			
		}break;
		case 1:
		{
			if(net.delay>=5)//�ȴ���ʱ������������
			{net.delay=0;
				net.setp_setp=0;//�ȴ�ʧ��
				qwl_sendstring("������ʱ");
				net.delay_times++;
				if(net.delay_times++>15)
				{net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=OPEN_GPRS;//����������������
					net.delay=0;
				}
			}
			

				if(strsearch_h("+ZTCPESTABLISHED:0",SystemBuf)>0)
				{net.reconnect_setp=LINK_OK;
					net.setp_setp=0;

//					light_heart_time_flag=heart_max_time;
					qwl_sendstring("�����ɹ�1");
				}
				
					if(strsearch_h("+ZIPSETUP:CONNECTED",SystemBuf)>0)
				{net.reconnect_setp=LINK_OK;
					net.setp_setp=0;

//					light_heart_time_flag=heart_max_time;
					qwl_sendstring("�����ɹ�2");
				}
				
				if(strsearch_h("ERROR",SystemBuf)>0)
				{
					net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=OPEN_GPRS;//����������������
					net.delay=0;
					
				qwl_sendstring("��������");
				}
			
		}break;
		default:break;
	}
}
/*******************************************************************************
* �� �� ��         : void cdma_open_GPRS()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_open_GPRS()
{
	switch (net.setp_setp)
	{
		case 0:
		{
			
			cdma_RESET_flag++;
			if(cdma_RESET_flag>3)
		{
			net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_CHOICE;//����CDMA����
					net.delay=0;
			cdma_RESET_flag=0;
					qwl_sendstring("����cdma��λ");
		}
			clear_SystemBuf();
			qwl_net_sendstring(AT_CIPCLOSE);
			net.setp_setp=1;
			net.delay=0;
			qwl_sendstring("�ر�����");
		}break;
		case 1:
		{
			if(net.delay>=6)//�ȴ���ʱ������������
			{net.delay=0;
				net.setp_setp=0;//�ȴ�ʧ��
				qwl_sendstring("����رճ�ʱ");
				net.delay_times++;
				if(net.delay_times>7)
				{net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_INIT;//����CDMA����
					net.delay=0;
					qwl_sendstring("����CDMA����");
				}
			}
			

				if(strsearch_h("+ZPPPSTATUS: CLOSED",SystemBuf)>0)
				{net.reconnect_setp=OPEN_GPRS;
					net.setp_setp=3;
					net.delay_times=0;
					net.delay=0;
					qwl_sendstring("����رճɹ�");
				}
				if(strsearch_h("ERROR",SystemBuf)>0)
				{
					net.reconnect_setp=OPEN_GPRS;
					net.setp_setp=3;
					net.delay_times=0;
					net.delay=0;

				qwl_sendstring("����رմ���");
				}
			
		}break;
		case 3:
		{
			clear_SystemBuf();			
			qwl_net_sendstring(AT_zpppopen);
			net.setp_setp=4;
			net.delay=0;
			qwl_sendstring("����������");
		}break;
		case 4:
		{
			if(net.delay>=6)//�ȴ���ʱ������������
			{net.delay=0;
				net.setp_setp=0;//�ȴ�ʧ��
				qwl_sendstring("�������ӳ�ʱ");
				net.delay_times++;
				if(net.delay_times>7)
				{net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_INIT;//����CDMA����
					net.delay=0;
					qwl_sendstring("����CDMA����");
				}
			}
			

				if(strsearch_h("OPENED",SystemBuf)>0)
				{net.reconnect_setp=LINK_IP;//����ϣ���������
					net.setp_setp=0;
					net.delay_times=0;
					net.delay=0;
					qwl_sendstring("���������ӳɹ�");
				}
				if(strsearch_h("+ZPPPOPEN:CONNECTED",SystemBuf)>0)
				{net.reconnect_setp=LINK_IP;//����ϣ���������
					net.setp_setp=0;
					net.delay_times=0;
					net.delay=0;
					qwl_sendstring("���������ӳɹ�");
				}
				if(strsearch_h("ERROR",SystemBuf)>0)
				{
					net.setp_setp=0;//�ȴ�ʧ��					
					net.delay_times++;
				if(net.delay_times++>7)
				{net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_INIT;//����CDMA����
					net.delay=0;
					qwl_sendstring("����CDMA����");
				}
					
				qwl_sendstring("���������");
				}
			
		}break;
		default:break;
	}
}
/*******************************************************************************
* �� �� ��         : cdma_init_0()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_init_0()
{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//�Ƚ���NBLOT����CDMAѡ��
								net.delay=0;
}
/*******************************************************************************
* �� �� ��         :cdma_send(u8 *p,u16 len)
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
	
u8 cdma_send(u8 *p,u16 len)
{
	

	u16 j=0;
	u16 g_delay_us=0;
	u16 g_delay_ms=0;
clear_SystemBuf();
qwl_net_sendstring1(AT_CIPSEND);
j=len;
if(len>1000)
{
	qwl_net_sendchar(NO[j/1000]);
	qwl_sendchar(NO[j/1000]);
	j=j%1000;
}
if(len>100)
{
	qwl_net_sendchar(NO[j/100]);
	qwl_sendchar(NO[j/100]);
	j=j%100;
}

qwl_net_sendchar(NO[j/10]);
qwl_net_sendchar(NO[j%10]);
qwl_sendchar(NO[j/10]);
qwl_sendchar(NO[j%10]);
		if(g_flag==0)
		{
		qwl_net_sendchar(0x20);	
		qwl_net_sendchar(0x0d);	
		}
		else
		{
			qwl_net_sendchar(0x0d);	
  	qwl_net_sendchar(0x0A);
				g_delay_us=0;
				g_delay_ms=0;
				while(strsearch_h(">",SystemBuf)==0)
				{
					API_WatchDog_Feed();
					Delay_2us();
					g_delay_us++;
					if(g_delay_us>400)
					{
						g_delay_us=0;
						g_delay_ms++;
						if(g_delay_ms>1000)
						{
							
							break;
						}
					}
				}
		}

for(j=0;j<len;j++)
  	{
  		qwl_sendchar(p[j]);//������
			qwl_net_sendchar(p[j]);
	}
		qwl_sendchar(0x0D);
  	qwl_sendchar(0x0A);
	qwl_net_sendchar(0x0D);
  	qwl_net_sendchar(0x0A);


return 0;
} 








