
#include "config.h"
u8 AT_CPIN1[]	="AT+CPIN?";		//��ѯSIM��״̬
u8 AT_CIPSTART1[]="AT+IPSTART=\"TCP\",";//GPRS���ӷ�ʽ\GPRS���ӵ�ַ(�����û�ʵ��IP�޸�)\GPRS���ӵĶ˿�
u8 AT_CIPCLOSE1[]="AT+IPCLOSE";		//TCP���ӹر�
u8 AT_zpppopen1[]	="AT+CGACT=1,1";		//������
//u8 AT_zpppstatus1[]="at+zpppstatus";//��ѯ����״̬
u8 AT_CIPSEND1[]	="AT+IPSEND=";		//GPRS������Ϣ����
u8 AT_GETICCID1[]	="AT+CCID";		//��ѯSIM����
u8 AT_CIMI1[]	="AT+CIMI";		//��ѯSIM����


u8 M6312_RESET_flag=0;



/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/
/*
void GSM_RESET(void)
{

	GPIO_ResetBits(CDMA_IO,CDMA_RESET);
	GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
	Delay_MS(100);//�ֲ�ֻҪ20ms���Ҹ������ʱ��
	GPIO_SetBits(CDMA_IO,CDMA_RESET);
	GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
	Delay_MS(50);//�ֲ�ֻҪ10ms���Ҹ������ʱ��
	GPIO_SetBits(CDMA_IO,CDMA_RESET);
	GPIO_ResetBits(CDMA_IO,CDMA_ON_OFF);

}*/
/*****************************************************************************************************************
ʵ�ֹ���:��SIM800C��������,����SIM800C��������:��Ƭ�����Ǹ�SIM800C�Ĺܽ�(/IGT)��ʱ������Ϊ1s�ĵ͵�ƽ�ź�
*****************************************************************************************************************/
/*
void Start_GSM(void)
{
//	GSM_RESET();
//	Delay_MS(3000);
//  GPIO_SetBits(CDMA_IO,CDMA_ON_OFF);
//	GPIO_SetBits(CDMA_IO,CDMA_RESET);
	
}
*/
/*******************************************************************************
* �� �� ��         : M6312_init()
* ��������		   : M6312��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void M6312_init()
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
				qwl_net_sendstring(AT_CPIN1);//��ѯSIM��״̬
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
									Delay_MS(1000);
								}
							
						
			}break;
			case 4:
			{
				qwl_sendstring("��sim����");
				clear_SystemBuf();
				qwl_net_sendstring(AT_GETICCID1);//��ѯSIM������

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
									Delay_MS(1000);
									net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=OPEN_GPRS;//����������������
								net.delay=0;
								}
							
						
			}break;
		
		default:break;
		}
		



}

/*******************************************************************************
* �� �� ��         : void light_eth_reconnect()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void M6312_reconnect()
{
	switch (net.setp_setp)
	{
		case 0:
		{
			qwl_sendstring("����IP");
		clear_SystemBuf();
if(exchange_platform.Platform_Choice == PLATFORM_CHOICE_PUBLIC)			
	TP_GPRS(AT_CIPSTART1,light_TCP);//����IP
else
	TP_GPRS(AT_CIPSTART1,update_ip);//��������IP

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
			

				if(strsearch_h("ALREADY CONNECT",SystemBuf)>0)
				{net.reconnect_setp=LINK_OK;
					net.setp_setp=0;

//					light_heart_time_flag=heart_max_time;
					qwl_sendstring("�����ɹ�1");
				}
				
					if(strsearch_h("CONNECT OK",SystemBuf)>0)
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
					Delay_MS(1000);
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
void M6312_open_GPRS()
{
	switch (net.setp_setp)
	{
		case 0:
		{
			
			M6312_RESET_flag++;
			if(M6312_RESET_flag>3)
		{
			net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_CHOICE;//����CDMA����
					net.delay=0;
			M6312_RESET_flag=0;
					qwl_sendstring("����cdma��λ");
		}
			clear_SystemBuf();
			qwl_net_sendstring(AT_CIPCLOSE1);
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
					Delay_MS(1000);
				}
				if(strsearch_h("ERROR",SystemBuf)>0)
				{
					net.reconnect_setp=OPEN_GPRS;
					net.setp_setp=3;
					net.delay_times=0;
					net.delay=0;

				qwl_sendstring("����رմ���");
					Delay_MS(1000);
				}
			
		}break;
		case 3:
		{
			clear_SystemBuf();			
			qwl_net_sendstring(AT_zpppopen1);
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
			

				if(strsearch_h("OK",SystemBuf)>0)
				{net.reconnect_setp=LINK_IP;//����ϣ���������
					net.setp_setp=0;
					net.delay_times=0;
					net.delay=0;
					qwl_sendstring("���������ӳɹ�");
					Delay_MS(1000);
				}
				if(strsearch_h("+ZPPPOPEN:CONNECTED",SystemBuf)>0)
				{net.reconnect_setp=LINK_IP;//����ϣ���������
					net.setp_setp=0;
					net.delay_times=0;
					net.delay=0;
					qwl_sendstring("���������ӳɹ�");
					Delay_MS(1000);
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
					Delay_MS(1000);				
				}
			
		}break;
		default:break;
	}
}

/*******************************************************************************
* �� �� ��         :M6312_send(u8 *p,u16 len)
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
	
u8 M6312_send(u8 *p,u16 len)
{
	

	u16 j=0;
	u16 g_delay_us=0;
	u16 g_delay_ms=0;
clear_SystemBuf();
qwl_net_sendstring1(AT_CIPSEND1);
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








