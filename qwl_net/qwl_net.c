#include "config.h"



u8 cdma_or_nblot_flag=0;
u8 send_NSORF_flag=0;

CONNECT_TYPE net={0};

EXCHANGE_PLATFORM exchange_platform={0,0,180,0,10,PLATFORM_CHOICE_PUBLIC};
//ProtocolResource _qProtocol = {0};	/*Э�黺��*/

//u8 USART3_rec_start_flag=0;//����3���ܿ�ʼ��־
//u16 USART3_rec_count=0;
//u8 eth_rec_buf[1024];
//u8 eth_rec_buf_1[512];//ת����ֱ������
//u16 USART3_rec_count_1=0;//ֱ�����ݼ���
//u16 eth_rec_CHKSUM=0;
//u8 S1_deal_flag=0;
u8 connect_flag=0;

//u8 light_control_buf[512];//ת����ֱ������
//u16 light_control_len=0;

u8 nblot_or_cdma_flag=0;//������ת�ã�ʶ�����ģ��
//u8 nblot_NRB[]="AT+NRB";

u16 Rx=0;
u8 SystemBuf[RxIn];  //������ڽ�������

const u8 ASCII_hex[24]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

u8 net_error_flag=0;//��������־
u8 deal_process=0;

u8 update_ip[22]={"39.108.148.248,26001"};

u8 CSQ_rec_deal=0;//����CSQ������
u8 CSQ_buf=0;
u8 CSQ=0;
u8 CSQ_DELAY=0;
	
void USART1_sendchar(u8 ch)
{
			USART_SendData(USART1,ch);//ͨ������USARTx���͵�������
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

}


/*******************************************************************************
* �� �� ��         : sendchar()
* ��������		   : ����1��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void qwl_net_sendchar(u8 ch)
{
   	
	USART1_sendchar(ch);
}

/*******************************************************************************
* �� �� ��         : send�ַ���()
* ��������		   : ����1��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void qwl_net_sendstring(u8 *p)
{
  	while(*p)
  	{
  		qwl_net_sendchar(*p);
  		p++;
	}
	qwl_net_sendchar(0x0d);
	qwl_net_sendchar(0x0a);
}
/*******************************************************************************
* �� �� ��         : send�ַ���()
* ��������		   : ����1��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void qwl_net_sendstring1(u8 *p)
{
  	while(*p)
  	{
  		qwl_net_sendchar(*p);
  		p++;
	}
}

/*******************************************************************************
* �� �� ��         : net_state
* ��������		   	: 
* ��    ��         :
* ��    ��         :0��ʾ��������
*******************************************************************************/
 void qwl_net_s_deal(void)
 {
	// 			light_heart_time_flag++;
		net.delay++;
	 ARMCHAIR_S_IQR();//��Ħ��ר���붨ʱ��
	 if(exchange_platform.Platform_Choice == PLATFORM_CHOICE_UPDATE)
		exchange_platform.UPDATE_link_overtime++;
//	nblot_read_flag1_delay++;
 }
/*******************************************************************************
* �� �� ��         : read_csq
* ��������		   : �����������
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void READ_CSQ()
{
if(exchange_platform.Platform_Choice != PLATFORM_CHOICE_UPDATE)
		{
					if(CSQ_DELAY>59)
					{
						CSQ_DELAY=0;
						qwl_net_sendstring("AT+CSQ");//��ѯ�ź�ǿ��
					}
		}
											//j=strsearch_h("+CSQ:",SystemBuf,Rx);								

}	
/*******************************************************************************
* �� �� ��         : net_state
* ��������		   	: 
* ��    ��         :
* ��    ��         :0��ʾ��������
*******************************************************************************/
u8 net_state()
{	
	if(exchange_platform.Platform_Choice ==	PLATFORM_CHOICE_UPDATE)
	{
		if(exchange_platform.UPDATE_link_overtime >exchange_platform.MAX_UPDATE_link_overtime )
		{
			exchange_platform.UPDATE_link_overtime =0;
			qwl_sendstring("��ʱ��δ���ϣ��ص�����ƽ̨2");
			exchange_platform.Platform_Choice =PLATFORM_CHOICE_PUBLIC;
			API_nblot_init_0();//��ʼ��ģ�飬��λ��ʼ
		}

		if(exchange_platform.UPDATE_overtime_readnb >30)
		{
			exchange_platform.UPDATE_overtime_readnb =0;
			if(net.mode_type ==MODE_TYPE_BC95)
				{	
					if(net.reconnect_setp==LINK_OK)
																			{
					qwl_sendstring("��ʱ��NB1");//�������
					qwl_net_sendstring("AT+NSORF=0,512");//�������
				}
			}
		}	
	}
								
	if(net_error_flag==0)
	{
		send_NSORF_flag=0;
	}								
	if(net.reconnect_setp!=LINK_OK)//��û�����ɹ�ʱ���������������ϵı�־������������������
	{
		connect_flag=0;
	}
	switch(net.reconnect_setp)
		{
			case LINK_OK:
			{//net.delay_times=0;
				
						if(connect_flag==0)//����ʱ�����ȷ�����������ֹ��������ʱ��̫�ã���Զ���Ӳ���
		{
			connect_flag=1;
			net_error_flag=0;
			exchange_platform.UPDATE_overtime_readnb =0;
			//upload_link_send();
/*			if(heart_max_time<=3)
			light_heart_time_flag=0;//heart_max_time;
			else
			{
				light_heart_time_flag=heart_max_time-3;
			}*/
		}	
			
									

			if(net_error_flag>=5)
			{
				net_error_flag=0;
				//deal_process_overtimes=0;
				//deal_process=0;
				net.delay_times=0;
				if(net.mode_type ==MODE_TYPE_ZTE)
					net.reconnect_setp=OPEN_GPRS;
				else
					net.reconnect_setp=LINK_IP;
				net.delay=0;
				net.setp_setp =0;
			}
				else if(net_error_flag>0)
			{
				if(send_NSORF_flag!=net_error_flag)
					{
						send_NSORF_flag=net_error_flag;//����ÿ�γ�ʱ��һ��
								if(net.mode_type ==MODE_TYPE_BC95)
								{	
											if(net.reconnect_setp==LINK_OK)
											{
								qwl_net_sendstring1("AT+NSORF=0,");//�����	NBIOT��Ҫ									
								qwl_net_sendchar(0x35);
								qwl_net_sendchar(0x31);
								qwl_net_sendchar(0x32);
								qwl_net_sendchar(0x0d);
								qwl_net_sendchar(0x0a);
											}
								}
					}
			}
			if(net.mode_type ==MODE_TYPE_ZTE)	
			READ_CSQ();
			
	if(net.mode_type ==MODE_TYPE_BC95)
	{	

		nblot_read();
		exchange_platform.UPDATE_overtime_readnb =0;
/*		if(nblot_read_flag1>0)
		{
			u8 k;
			if(nblot_read_flag1_delay>2)//�ȴ���ʱ������������
			{

				nblot_read_flag1_delay=0;//�������ͳɹ���ȷ�ϱ�־
				qwl_net_sendstring1("AT+NSORF=0,");//�����
				qwl_net_sendchar(0x35);
				qwl_net_sendchar(0x31);
				qwl_net_sendchar(0x32);
				qwl_net_sendchar(0x0d);
				qwl_net_sendchar(0x0a);


				qwl_sendstring("�ȴ���������ʱ4\r\n");
				nblot_read_flag1=0;//������һ�����̴���

			}

			//	k=nblot_group();
			if(k==0)
			nblot_read_flag1=0;
		}*/
		
		
		
	}
				
	
			}break;
			case LINK_IP:
			{
				if(net.mode_type ==MODE_TYPE_ZTE)
				{
							switch(g_flag)
							{
								case 2:
								{
									M6312_reconnect();//�ƶ�ģ��ΪM6312
								}
								break;
								case 3:
								{
									G510_reconnect();//�ƶ�ģ��ΪG510
								}
								break;
								default:
								{
									cdma_reconnect();//�ƶ�ģ��ΪMG2618���ߵ���MC8618
								}break;
							}
					
					}
				else
				nblot_reconnect();
			}break;
			case OPEN_GPRS:
			{
				if(net.mode_type ==MODE_TYPE_ZTE)
				{
					
								switch(g_flag)
							{
								case 2:
								{
									M6312_open_GPRS();//�ƶ�ģ��ΪM6312
								}
								break;
								case 3:
								{
									G510_open_GPRS();//�ƶ�ģ��ΪG510
								}
								break;
								default:
								{
									cdma_open_GPRS();//�ƶ�ģ��ΪMG2618���ߵ���MC8618
								}break;
							}
					
				}
				else
				{
					nblot_open_GPRS();
				}
			}break;
			case MODE_INIT:
			{
				
				if(net.mode_type ==MODE_TYPE_ZTE)
				{
					switch(g_flag)
					{
						case 2:
						{
							M6312_init();//�ƶ�ģ��ΪM6312
						}
						break;
						case 3:
						{
							G510_init();//�ƶ�ģ��ΪG510
						}
						break;
						default:
						{
							cdma_init();//�ƶ�ģ��ΪMG2618���ߵ���MC8618
						}break;
					}

				}
				else
				nblot_init();
			}break;
			case MODE_CHOICE:
			{
			cdma_or_nblot();//ѡ����NBLOT����CDMA	
			}
			default:break;
		}
		return net.reconnect_setp;
}


//***************************************************
//��������
//*********************************************************
/*******************************************************************************
* �� �� ��         :net_send(u8 *p,u16 len)
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
	
u8 net_send(u8 *p,u16 len)
{

	if(net.mode_type ==MODE_TYPE_ZTE)
	{
		
		switch(g_flag)
					{
						case 2:
						{
							M6312_send(p,len);//�ƶ�ģ��ΪM6312
						}
						break;
						case 3:
						{
							G510_send(p,len);//�ƶ�ģ��ΪG510
						}
						break;
						default:
						{
							cdma_send(p,len);//�ƶ�ģ��ΪMG2618���ߵ���MC8618
						}break;
					}
					

	}
	else
	{
		nblot_send(p,len);
		
	}

return 0;
} 



/*******************************************************************************
* �� �� ��         : REC_CSQ
* ��������		   	: 
* ��    ��         :
* ��    ��         :
*******************************************************************************/
void REC_CSQ(u8 k)
{
	if(k=='+')
	{
		CSQ_rec_deal=1;
	}
	switch(CSQ_rec_deal)
	{
		case 1:
		{
			if(k=='C')
				CSQ_rec_deal++;
		}
		break;
		case 2:
		{
			if(k=='S')
				CSQ_rec_deal++;
		}
		break;
		case 3:
		{
			if(k=='Q')
				CSQ_rec_deal++;
		}
		break;
		case 4:
		{
			if(k==':')
				CSQ_rec_deal++;
		}
		break;
		case 5:
		{
			if(k>=0x30)
				CSQ_buf=k;
			else
				CSQ_buf=0;
			CSQ_rec_deal++;
		}break;
				case 6:
		{
				if(CSQ_buf>=0x30)
				{
					if(k>=0x30)
					CSQ_buf=((CSQ_buf-0x30)*10)+(k-0x30);
					else
					CSQ_buf=(CSQ_buf-0x30);	
					
					CSQ=CSQ_buf;
					CSQ_buf=0xff;
				}
				else
				CSQ_buf=k;
				
				CSQ_rec_deal++;
		}
		break;
				case 7:
		{
			if(CSQ_buf!=0xff)
			{
				if(k>=0x30)
				CSQ_buf=((CSQ_buf-0x30)*10)+(k-0x30);
				else
					{
						CSQ_buf=(CSQ_buf-0x30);
					}
				CSQ=CSQ_buf;
					CSQ_rec_deal=0;
			}
		}
		break;
		default:break;
	}
}
/*******************************************************************************
* �� �� ��         : net_rec
* ��������		   	: 
* ��    ��         :
* ��    ��         :
*******************************************************************************/
void net_rec(u8 k)
{
	//qwl_sendchar(k);
	if(Rx<RxIn)
		{
			SystemBuf[Rx]=k;
 			Rx++;
			
		}
		
		if(net.mode_type ==MODE_TYPE_BC95)
		{
			if(net.reconnect_setp ==LINK_OK)
			nblot_rec(k);//��NB�Ƿ�����Ҫ��ȡ����

		}

		REC_CSQ(k);

		/*
		if(nblot_rec_times<500)
		{
		nblot_rec_buf[nblot_rec_times]=k;
			nblot_rec_times++;
		}
		if(net.mode_type ==MODE_TYPE_BC95)
		{nblot_rec(k);

		}
		else
		{
//				if(qwl_peizhi==0)
//				{
				eth_rec_deal(k);
//				}
		}
		*/
		
		
}


/*******************************************************************************
* �� �� ��         : qwl_net_init()
* ��������		   	: 
* ��    ��         :
* ��    ��         :
*******************************************************************************/
void qwl_net_init()
{
	CDMA_IO_Init();
	nblot_init_0();
	
		API_SetTimer(1000,-1,TIMER_TYPE_QWL_NET_DEAL,qwl_net_s_deal,NULL);
}
/*******************************************************************************
* �� �� ��         : cdma_or_nblot_setp_init()
* ��������		   : ģ��ѡ����������һ��ͳһ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_or_nblot_setp_init()
{
										net.delay_times=0;
										net.reconnect_setp=MODE_INIT;
										net.delay=0;
										net.setp_setp=0;

									if(exchange_platform.Platform_Choice ==PLATFORM_CHOICE_UPDATE)//����ʮ�����Ӳ��ϣ��ػ�����ƽ̨,ר������
									{
											exchange_platform.UPDATE_link_times ++;
										if(exchange_platform.UPDATE_link_times>exchange_platform.MAX_UPDATE_link_times )
										{
											qwl_sendstring("������δ���ϣ��ص�����ƽ̨1");
											exchange_platform.UPDATE_link_times=0;
											exchange_platform.Platform_Choice =PLATFORM_CHOICE_PUBLIC;
											API_nblot_init_0();//��ʼ��ģ�飬��λ��ʼ
										}
									}

}

/*******************************************************************************
* �� �� ��         : usart_init
* ��������		   : ���ڳ�ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void usart_init(u32 BaudRate)
{

	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO

	USART_InitTypeDef  USART_InitStructure;	  //���ڽṹ�嶨��

	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ�嶨��
	
	
	USART_ClearFlag(USART1,USART_FLAG_TC);//���USARTx�Ĵ������־λ	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */


	USART_InitStructure.USART_BaudRate=BaudRate;   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//���ݳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;				//��Ч��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //�������ͺͽ���ģʽ
	USART_Init(USART1,&USART_InitStructure);	/* ��ʼ��USART1 */
	USART_Cmd(USART1, ENABLE);		   /* ʹ��USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(USART1,USART_FLAG_TC);//���USARTx�Ĵ������־λ


	/* ����NVIC���� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	   //��USART1��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //ʹ��
	NVIC_Init(&NVIC_InitStructure);

}
/*******************************************************************************
* �� �� ��         : cdma_or_nblot()
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void cdma_or_nblot()
{
		switch (net.setp_setp)
	{
			case 0:
			{
				clear_SystemBuf();	
				net.setp_setp=1;
				net.delay=0;
				net.mode_type =MODE_TYPE_NONE;//ģ��δѡ��ǰ��״̬
				//qwl_sendstring("AT+CGMM");	
				qwl_net_sendstring("AT+NRB");//��λ	
				GSM_RESET();//��λCDMA
				nblot_or_cdma_flag=0;
				g_flag=0;//g����־���
			}break;
			case 1:
			{
							if(strsearch_h("ZTE",SystemBuf)>0)
								{ 
									qwl_sendstring("ģ����CDMA");
									net.mode_type =MODE_TYPE_ZTE;//��CDMA
								cdma_or_nblot_setp_init();
										
								}
				if(strsearch_h("MC",SystemBuf)>0)
								{ 
									qwl_sendstring("ģ����CDMA");
									net.mode_type =MODE_TYPE_ZTE;//��CDMA
								
										cdma_or_nblot_setp_init();

								}	
		
			if(strsearch_h("M6312",SystemBuf)>0)
								{ 
									qwl_sendstring("ģ�����ƶ�M6312");
									net.mode_type =MODE_TYPE_ZTE;//��CDMA
									g_flag=2;
										cdma_or_nblot_setp_init();
								}	
			if(strsearch_h("G510",SystemBuf)>0)
								{ 
									qwl_sendstring("ģ�����ƶ�G510");
									net.mode_type =MODE_TYPE_ZTE;//��CDMA
									g_flag=3;
										cdma_or_nblot_setp_init();
								}					
			if(strsearch_h("MG",SystemBuf)>0)
								{ 
									qwl_sendstring("ģ����G��");
									net.mode_type =MODE_TYPE_ZTE;//��CDMA
									g_flag=1;
										cdma_or_nblot_setp_init();
								}										
				if(strsearch_h("REBOOTING",SystemBuf)>0)
								{ 
									qwl_sendstring("ģ����NBLOT");
									net.mode_type =MODE_TYPE_BC95;//��NBLOT
								/*	if(heart_max_time>170)
									{
									heart_max_time=170;//nb��ʱ3���Ӿͻ����Ӳ��ϣ��������3��������
									}
									*/
										cdma_or_nblot_setp_init();
								}
				if(net.delay>=2)//�ȴ���ʱ������������
					{net.delay=0;
						qwl_sendstring("ʶ��ģ�鳬ʱ");
						
						if(nblot_or_cdma_flag==0)
							net.setp_setp=0;
							else
							{
								net.setp_setp=2;
								
							}
						
						net.delay_times++;
						if(net.delay_times>3)
						{net.delay_times=0;
							if(nblot_or_cdma_flag==0)
							net.setp_setp=2;
							else
							{
								qwl_sendstring("�л���115200");
								usart_init(115200);
								Delay_MS(500);
								API_WatchDog_Feed();
	

							
								qwl_net_sendstring("AT+IPR=9600");
								Delay_MS(500);
								API_WatchDog_Feed();
				
								usart_init(9600);
								
								Delay_MS(500);
								API_WatchDog_Feed();
								net.setp_setp=0;
							}
						}
					}
				
			}break;
	case 2:
			{
/*				qwl_sendstring("����CDMA");
				Start_GSM();
				API_WatchDog_Feed();
						delay_ms(1000);
						API_WatchDog_Feed();
						qwl_net_sendstring("ATE0");//�رջ�������DCEΪOK��ʽ
						delay_ms(1000);
						API_WatchDog_Feed();
				*/
				//Start_GSM();
				clear_SystemBuf();	
				net.setp_setp=1;
				net.delay=0;
				net.mode_type =MODE_TYPE_NONE;//ģ��δѡ��ǰ��״̬
				qwl_net_sendstring("AT+CGMM");	
			//qwl_net_sendstring(nblot_NRB);//��λ	
				g_flag=0;//g����־���
				nblot_or_cdma_flag=1;
				}break;
		
		default:break;
		}

}

