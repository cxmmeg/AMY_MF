
#include "config.h"

u16 eth_Len=0;

u8 eth_rec_buf[500];
u16 eth_rec_CHKSUM=0;

u8 USART3_rec_start_flag=0;//����3���ܿ�ʼ��־
u16 USART3_rec_count=0;
	
	



u16 GATEWAY_heart_time_flag=59;   // ����������ʱ��ʱ��ר�����ؼ�ʱ
u16 GATEWAY_heart_max_flag=10;   // ���������ӳ�ʱ�䣬Ĭ��60��
u8 	GATEWAY_deal_time=0;				//������ʱ����
u8  getwayheart_sendok_flag=1;//�����з��ͳɹ���ȷ�ϱ�־
	
u16 GATEWAY_list_time_flag=00;   // �б�����ʱ��ʱ��ר�����ؼ�ʱ
u8 	GATEWAY_list_time=0;					//�б��ʱ����
u8  getwaylist_sendok_flag=0;//�б��з��ͳɹ���ȷ�ϱ�־
u8	getwaylist_send_flag=0;//�б��ͱ�־��������0ʱ��Ҫ���б��ϴ�
u8	getwaylist_send_times=0;//�б��ϴ�����

u16 GATEWAY_off_time_flag=00;   // �б�����ʱ��ʱ��ר�����ؼ�ʱ
u8 	GATEWAY_off_time=0;					//�б��ʱ����
u8  getwayoff_sendok_flag=0;//�б��з��ͳɹ���ȷ�ϱ�־
u8	getwayoff_send_flag=0;//�б��ͱ�־��������0ʱ��Ҫ���б��ϴ�
u8	getwayoff_send_times=0;//�б��ϴ�����	

u8 amy_config_rec_flag=0;//��Ħ�����ñ�־λ
u8 amy_check_ID[16];//��Ħ��Ҫ��ѯ���ն��Ȼ���
	
u8 amystate_send_flag[250]={0};//��Ħ��״̬���ͱ�־
	
u8 plc_cofig_buf[16];//��Ħ��������ʱ�洢���ڷ���
u8 plc_cofig_buf_1[3];//��Ħ������ʱ�䣬ģʽ
	
//u8 deal_process=0;//������̱�־���ʵ粿Ҫ��һ��һ�����������ôα�־

u8 amy_check_pointer=0x00;//��Ħ�β�ѯָ��
u8 amy_storflash_pointer=0;//���ڴ洢��flash��������250̨��ѯ���ʹ洢
u8 amy_storflash_flag=0;//���ڴ洢��flash��������250̨��ѯ���ʹ洢��//��־��0��flash��1ѭ����һ��Ȼ�����flash��2���ȴ����նˣ�Ȼ�����flash
u8 suipian_delay=0;
POLL_ZD_TYPE poll_zd={0};

/*******************************************************************************
* �� �� ��         : USART3_sendchar(u8 ch)
* ��������		   : ����3��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void USART3_sendchar(u8 ch)
{
			USART_SendData(USART3,ch);//ͨ������USARTx���͵�������
		
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);

}

/*******************************************************************************
* �� �� ��         : USART3_sendstring(u8 *p)
* ��������		   : ����3��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void USART3_sendstring(u8 *p)
{
  	while(*p)
  	{
  		USART3_sendchar(*p);
  		p++;
	}

}	

/*******************************************************************************
* �� �� ��         :USART3_sendstring1(u8 *p,u16 len)
* ��������		   : ����3��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void USART3_sendstring1(u8 *p,u16 len)
{
	u16 i;
  	for(i=0;i<len;i++)
  	{
  		USART3_sendchar(p[i]);
			//qwl_sendchar(p[i]);//������
	}

}	
/*******************************************************************************
* �� �� ��         : usart_init
* ��������		   : ���ڳ�ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void usart3_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO

	USART_InitTypeDef  USART_InitStructure;	  //���ڽṹ�嶨��

	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ�嶨��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//  ʹ��ʱ��   
   
  /*����3ʹ��GPIOB��10,11��*/
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);//  ʹ��ʱ��
	
  //USART3 TX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  //USART3 Rx  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate=9600;   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//���ݳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;				//��Ч��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //�������ͺͽ���ģʽ
	USART_Init(USART3,&USART_InitStructure);	/* ��ʼ��USART3 */
	USART_Cmd(USART3, ENABLE);		   /* ʹ��USART3 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(USART3,USART_FLAG_TC);//���USARTx�Ĵ������־λ


  //ʹ����Ӧ�жϣ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

//������������������������������������������������������������������������������
// ��������  : USART5_Init
// ��������  : ����5��ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void usart5_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef  USART_InitStructure;	  //���ڽṹ�嶨��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); 

	/* USART5 Tx (PC.12),��� USART5 Rx (PD.02)���� */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate=9600;   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//���ݳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;				//��Ч��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //�������ͺͽ���ģʽ
	USART_Init(UART5,&USART_InitStructure);	/* ��ʼ��USART5 */
	USART_Cmd(UART5, ENABLE);		   /* ʹ��USART5 */
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(UART5,USART_FLAG_TC);//���USARTx�Ĵ������־λ

	/*������5�����ж�*/
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;												  
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* �� �� ��         : wg_amy_state()
* ��������		   : ����ֱ����Ħ�Σ����״̬
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void wg_amy_state()
{u8 i;
	if(wg_state_flag==1)
	{
		wg_state_flag=0;
		for(i=0;i<16;i++)
		amy_check_ID[i]=AmyID[i];
							
		if(Shared_Module.Module==ruilaikesi)
		{
								i=armchair_io_state();
		}
		else
		{
			if(amy_cotrol_flag==1)
			{
			i=1;
			}
			else
				i=0;
		}
						if(i==0)//��Ħ��״̬��ȡ�ɹ�
							{
								
							qwl_sendstring("���Ƴɹ�\r\n");
								amystate_pack_send(0,0xff);//״̬����ϴ�
							}
							else if(i==1)
							{	
							qwl_sendstring("����ʧ��\r\n");
							amystate_pack_send(1,0xff);//״̬����ϴ�
							}	
							else if(i==2)
							{	
							qwl_sendstring("��Ħ�λ���\r\n");
							amystate_pack_send(3,0xff);//״̬����ϴ�
							}	
				
	}
	else if(wg_state_flag==2)
	{
		wg_state_flag=0;
		for(i=0;i<16;i++)
		amy_check_ID[i]=AmyID[i];
		if(Shared_Module.Module==ruilaikesi)
		{
								i=armchair_io_state_1();
		}
		else
			i=1;
						if(i==0)//��Ħ��״̬��ȡ�ɹ�
							{
													amy_cotrol_time=0;//����ʱ����0	
							qwl_sendstring("��Ħ������ʧ�ܣ���Ҫ�رհ�Ħ��\r\n");
								amystate_pack_send(0,0xff);//״̬����ϴ�
							}
							else if(i==1)
							{	
							qwl_sendstring("��Ħ�������ɹ�\r\n");
							amystate_pack_send(1,0xff);//״̬����ϴ�
							}	

	}
}


/*******************************************************************************
* �� �� ��         : amy_CHKSUM_produce()
* ��������		   : ˢ���б�ȥ���þ�û���ӵģ�10������Ϊһ���޳���λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void amy_CHKSUM_produce()
{ u16 i;
	u8 j;
	amy_CHKSUM=0;
	for(i=0;i<250;i++)
		{
			
	if(amy_record_buf[i*21]>0)
			{
				for(j=0;j<16;j++)
				{amy_CHKSUM=amy_CHKSUM+amy_record_buf[(i*21)+4+j];
				}
			}
		}	
}

/*******************************************************************************
* �� �� ��         :GATEWAY_heard_link
* ��������		   	: ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void GATEWAY_heart_link()//��������
{
	qwl_sendstring("��������");
	qwl_sendchar(NO[CSQ>>4]);
	qwl_sendchar(NO[CSQ&0x0f]);
	if(poll_zd.poll_max_time >0)
	{
		qwl_sendchar(NO[(poll_zd.poll_time>>12)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_time>>8)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_time>>4)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_time>>0)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_max_time>>12)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_max_time>>8)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_max_time>>4)&0x0f]);
		qwl_sendchar(NO[(poll_zd.poll_max_time>>0)&0x0f]);
	}
	
		GATEWAY_heart_send_pack();//�����������
		USART3_sendstring1(post_buf,post_len);
		if(net.reconnect_setp==LINK_OK)
		{
			
			net_send(post_buf,post_len);
		}	
}
/*******************************************************************************
* �� �� ��         :GATEWAY_heart_send
* ��������		   	: �������Ͳ����鴦��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void GATEWAY_heart_send()//�������Ͳ����鴦��
{
	switch (GATEWAY_deal_time)
	{
		case 0:
		{
			if(GATEWAY_heart_time_flag>=GATEWAY_heart_max_flag)//�ﵽԤ���ķ���ʱ��ֵ���ϴ�����
			{
				GATEWAY_heart_time_flag=0;//����ʱ�����¼�ʱ
				GATEWAY_heart_link();//��������
				GATEWAY_deal_time=1;//�ȴ�����
				getwayheart_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־
			}
			else
				deal_process=0;//������һ�����̴���
						
		}break;
		case 1:
		{
			if(GATEWAY_heart_time_flag>5)//�ȴ���ʱ������������
			{
				GATEWAY_deal_time=0;//�ȴ�ʧ��
				getwayheart_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־
				amy_config=amy_config|0x40;//��λ�������ʾ
				qwl_sendstring("�ȴ���������ʱ1\r\n");
				deal_process=0;//������һ�����̴���
				GATEWAY_heart_max_flag=10;//��ʱ���������
				
				net_error_flag++;

			}
			
			if(getwayheart_sendok_flag==1)//�������ͳɹ�
			{//getwayheart_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־
				amy_config=(amy_config&0xbf);//��λ�������ʾ
			GATEWAY_deal_time=0;//�ȴ��ɹ������¶�ʱ��������
				qwl_sendstring("���������ͳɹ�1\r\n");
				deal_process=0;//������һ�����̴���
				net_error_flag=0;
			}
						

		}break;
		default:break;
	}
	
}
/*******************************************************************************
* �� �� ��         :GATEWAY_list_link
* ��������		   	: �б��ϴ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
/*void GATEWAY_list_link()//��������
{
	qwl_sendstring("�б�������ϴ�\r\n");	
	GATEWAY_list_send_pack();//�����������
	USART3_sendstring1(post_buf,post_len);
if(net.reconnect_setp==LINK_OK)
{
	net_send(post_buf,post_len);
}	

				
				
}

*/
/*******************************************************************************
* �� �� ��         :GATEWAY_list_send
* ��������		   	: �б��ϴ������Ҽ��鴦��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
/*void GATEWAY_list_send()//�б��ϴ������Ҽ��鴦��
{
	switch(GATEWAY_list_time)
	{
		case 0:
		{
			u8 i;

							if(getwaylist_send_flag>0)//�����豸�������豸���£�Ҫ���ϴ�
						{
										if((amy_total)==0)
										{qwl_sendstring("�ն����������Ҳ��Ҫ�ϴ�һ��\r\n");
											getwaylist_send_times=0;
										}											
										else
										{
											i=amy_total/13;
											if((amy_total%13)>0)
												i++;
											
											getwaylist_send_times=i;
										}
													GATEWAY_list_time_flag=0;//�б�ʱ�����¼�ʱ

								
																
													GATEWAY_list_link();//�б��ϴ�

														GATEWAY_list_time=1;//�ȴ�����
														getwaylist_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־

											
						}
						else
							deal_process=1;//������һ�����̴���
		}break;
		case 1:
		{
			if(GATEWAY_list_time_flag>3)//�ȴ���ʱ������������
			{
				getwaylist_send_times=0;
				GATEWAY_list_time=0;//�ȴ�ʧ��
				getwaylist_send_flag=0;//�б�Ҫ�������ϴ���־������Ѿ�����
				//getwaylist_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־
				amy_config=amy_config|0x40;//��λ�������ʾ
				qwl_sendstring("�ȴ���������ʱ2\r\n");
				deal_process=1;//������һ�����̴���
			}

			if(getwaylist_sendok_flag==1)//�б��ͳɹ�
			{getwaylist_sendok_flag=0;//�б��ͳɹ���ȷ�ϱ�־
				amy_config=(amy_config&0xbf);//��λ�������ʾ
			
				if(getwaylist_send_times==0)
				{getwaylist_send_flag--;//�б�Ҫ�������ϴ���־������Ѿ��������
					deal_process=1;//������һ�����̴���
					GATEWAY_list_time=0;//�ȴ��ɹ������¶�ʱ��������
				}
				else
				{GATEWAY_list_time=2;
				}
				qwl_sendstring("���������ͳɹ�2\r\n");
				
			}
						

		}break;
		
		case 2:
		{

											
											
													GATEWAY_list_time_flag=0;//�б�ʱ�����¼�ʱ
								
																
													GATEWAY_list_link();//�б��ϴ�

														GATEWAY_list_time=1;//�ȴ�����
														getwaylist_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־
			}break;
		default:break;
	}
	
}
*/
/*******************************************************************************
* �� �� ��         :POLL_list_link
* ��������		   	: �б��ϴ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void POLL_list_link()//��������
{
	qwl_sendstring("�б�������ϴ�\r\n");	
	poll_list_send_pack();//�����������
	USART3_sendstring1(post_buf,post_len);
if(net.reconnect_setp==LINK_OK)
{
	net_send(post_buf,post_len);
}	

				
				
}


/*******************************************************************************
* �� �� ��         :POLL_list_send
* ��������		   	: �б��ϴ������Ҽ��鴦��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void POLL_list_send()//�б��ϴ������Ҽ��鴦��
{
	switch(GATEWAY_list_time)
	{
		case 0:
		{

							if(poll_zd.state_send_flag>0)//�����豸�������豸���£�Ҫ���ϴ�
						{
GATEWAY_list_time_flag=0;//�б��ϴ�

																
													POLL_list_link();//�б��ϴ�

														GATEWAY_list_time=1;//�ȴ�����
														getwaylist_sendok_flag=2;//�������ͳɹ���ȷ�ϱ�־

											
						}
						else
							deal_process=1;//������һ�����̴���
		}break;
		case 1:
		{
			if(GATEWAY_list_time_flag>5)//�ȴ���ʱ������������
			{
GATEWAY_list_time_flag=0;//�б��ϴ�

				GATEWAY_list_time=0;//�ȴ�ʧ��
				poll_zd.state_send_flag=0;
				//poll_zd.state_send_flag=0;//�б�Ҫ�������ϴ���־������Ѿ�����
				//getwaylist_sendok_flag=0;//�������ͳɹ���ȷ�ϱ�־
				amy_config=amy_config|0x40;//��λ�������ʾ
				qwl_sendstring("�ȴ���������ʱ2\r\n");
				deal_process=1;//������һ�����̴���
			}

			if(getwaylist_sendok_flag==1)//�б��ͳɹ�
			{getwaylist_sendok_flag=0;//�б��ͳɹ���ȷ�ϱ�־
				amy_config=(amy_config&0xbf);//��λ�������ʾ
							
					getwaylist_send_flag=0;//gatewaylist_send_flag--;//�б�Ҫ�������ϴ���־������Ѿ��������
					deal_process=1;//������һ�����̴���
					GATEWAY_list_time=0;//�ȴ��ɹ������¶�ʱ��������

					poll_zd.state_send_flag=0;
				qwl_sendstring("���������ͳɹ�2\r\n");
				
			}
						

		}break;
		
		
		default:break;
	}
	
}

/*******************************************************************************
* �� �� ��         :POLL_list_link
* ��������		   	: �б��ϴ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void amy_off_link()//��������
{
	qwl_sendstring("��Ħ�ι���ֹͣ�ϴ�\r\n");	
	amy_off_send_pack();//�����������
	USART3_sendstring1(post_buf,post_len);
if(net.reconnect_setp==LINK_OK)
{
	net_send(post_buf,post_len);
}	

				
				
}


/*******************************************************************************
* �� �� ��         :POLL_list_send
* ��������		   	: �б��ϴ������Ҽ��鴦��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void amy_off_send()//�б��ϴ������Ҽ��鴦��
{
	switch(GATEWAY_off_time)
	{
		case 0:
		{

				if((getwayoff_send_flag&0x01)>0)
				{
						

																
													amy_off_link();//�б��ϴ�

														GATEWAY_off_time=1;//�ȴ�����
														getwayoff_sendok_flag=2;//�������ͳɹ���ȷ�ϱ�־

									GATEWAY_off_time_flag=0;//�б��ϴ�	
				}
		}break;
		case 1:
		{
			if(GATEWAY_off_time_flag>5)//�ȴ���ʱ������������
			{
				GATEWAY_off_time_flag=0;//�б��ϴ�

				GATEWAY_off_time=0;//�ȴ�ʧ��
				qwl_sendstring("�ȴ���������ʱ3\r\n");
				
			}

			if(getwayoff_sendok_flag==1)//�б��ͳɹ�
			{getwayoff_sendok_flag=0;//�б��ͳɹ���ȷ�ϱ�־
				amy_config=(amy_config&0xbf);//��λ�������ʾ
							
					getwayoff_send_flag=0;//gatewaylist_send_flag--;//�б�Ҫ�������ϴ���־������Ѿ��������
					GATEWAY_off_time=0;//�ȴ��ɹ������¶�ʱ��������
				qwl_sendstring("���������ͳɹ�3\r\n");
				
			}
						

		}break;
		
		
		default:break;
	}
	
}
/*******************************************************************************
* �� �� ��         :GATEWAY_heart_send
* ��������		   	: �������Ͳ����鴦��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void SIM_ID_SEND()//�������Ͳ����鴦��
{
	if(SIM_GET_FLAG>0)
	{SIM_GET_FLAG=0;
		SIM_ID_send_pack();
	USART3_sendstring1(post_buf,post_len);
if(net.reconnect_setp==LINK_OK)
{
	net_send(post_buf,post_len);
}	
	}
}
/*******************************************************************************
* �� �� ��         : suibian_send()
* ��������		   : 
* ��    ��         : 
* ��    ��         : 
*******************************************************************************/
void suipian_send()
{
	if(suipian_delay>15)
	{
		suipian_delay=0;
		if(exchange_platform.Platform_Choice != PLATFORM_CHOICE_UPDATE)
		{
		if(net.reconnect_setp==LINK_OK)
			if(net.mode_type ==MODE_TYPE_ZTE)
				{
							switch(g_flag)
								{
									case 2:
									{
										
									}
									break;
									case 3:
									{
										
									}
									break;
									default:
									{
										qwl_sendstring("����������ݣ�Ϊ�˱��ֿ��ٷ�Ӧ");
										net_send(0x00,1);//�ƶ�ģ��ΪMG2618���ߵ���MC8618
									}break;
								}
				}
		}
	}

}
/*******************************************************************************
* �� �� ��         :GATEWAY_send()
* ��������		   	: ���ط��ʹ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void GATEWAY_send()//�������������ʹ������
{

	amy_CHKSUM_produce();
	GATEWAY_heart_send();//�������ͣ���������֤�Ƿ��ͳɹ�
	POLL_list_send();
	SIM_ID_SEND();
	suipian_send();//Ϊ�˱��ֿ��ٷ�Ӧ
	amy_off_send();//���Ͱ�Ħ��ֹͣ�ź�
/*	switch(deal_process)//�������ڴ��ͣ��ȴ�
		{	case 0:
			{

			
			GATEWAY_list_send();//�б��ϴ�����������֤�Ƿ��ͳɹ�
			}break;
			case 1: 
			{
			GATEWAY_heart_send();//�������ͣ���������֤�Ƿ��ͳɹ�
			}break;
			default: break;
		}*/

}
/*******************************************************************************
* �� �� ��         :amystate_pack_send
* ��������		   	: ��Ħ�δ���ϴ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void amystate_pack_send(u8 state,u8 addr)
{

	amystate_send_pack(state,addr);//״̬�������
	USART3_sendstring1(post_buf,post_len);
	if(net.reconnect_setp==LINK_OK)
{
	net_send(post_buf,post_len);
}	
}
/*******************************************************************************
* �� �� ��         :amy_state_send()
* ��������		   	: �ն�״̬����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void amy_state_send()
{
	u16 i;
	u8 j;
	u8 n;
	u16 k;
	u8 jj=0x03;
	u8 addr[3];
//	u8 *read_flash;

	//u32 AMYID_adr=0x0803D000;//��Ħ�δ洢ID��ַ
	//u8 *AmyID;
	for(i=0;i<amy_max_total;i++)
		{if(config_recwait_buf[i]>0)//��ѯ��û��Ҫ�ȴ��������ն�
			break;
		}
if(i>=amy_max_total)//û����Ҫ�������նˣ����Ҳ�ѯ
		{		
				switch (poll_zd.poll_deal )
				{
					case 0://�б�״̬������״̬��
					{
						if(poll_zd.list_please_flag >0)//�����б����ٽ��в�ѯ����Ȼ��������ѯ
						{
								if(poll_zd.poll_max_time>0)
								{
									if(poll_zd.poll_time >=poll_zd.poll_max_time )
									{
										poll_zd.poll_time=0;
										poll_zd.poll_deal =1;
									}
								}
								else
								{
									poll_zd.list_please_flag=0;
									poll_zd.poll_time=0;
									poll_zd.poll_deal =1;
								}
						}
						
					}break;
					case 1://�����б��е��ն�״̬����ѯ��ʽ
					{
						amy_check_pointer++;
								if(amy_check_pointer>=amy_max_total)
									amy_check_pointer=0;
						for(;amy_check_pointer<amy_max_total;amy_check_pointer++)
						{j=poll_zd.zd_list[(amy_check_pointer/8)];
							//qwl_sendchar(NO[j>>4]);
							//qwl_sendchar(NO[j&0x0f]);
							
							n=amy_check_pointer%8;
							//qwl_sendchar(NO[n>>4]);
							//qwl_sendchar(NO[n&0x0f]);
							j=j>>n;
							//qwl_sendchar(NO[j>>4]);
							//qwl_sendchar(NO[j&0x0f]);
							if((j&0x01)!=0)
							{	
								
								break;
								
							}

						}
						if(amy_check_pointer>=amy_max_total)
							{
								poll_zd.poll_deal =0;
								poll_zd.state_send_flag =1;
									amy_check_pointer=0;
								qwl_sendstring("��ѯһ�����");
							}
							else{

									config_recwait_buf[amy_check_pointer]=0x01;
									addr[0]=0;
									addr[1]=amy_check_pointer;
									addr[2]=q2x1278_config_buf[4];
									amy_config_please(&addr[0]);
									config_recwait_time=0;
								
									}
					}break;
				
					default:break;
				}
			
					
					
					
					
		}
		
/*		if(i>=amy_max_total)//û����Ҫ�������նˣ����Ҳ�ѯ
		{
				if(amy_storflash_flag==0)//���������Ȳ�flash������
					{
						read_flash=(u8*)(flash_AMY_adr1);
										for(i=amy_storflash_pointer;i<amy_max_total;i++)
										{
											amy_storflash_pointer++;//��ѯ��־
											if(amy_storflash_pointer>=amy_max_total)
											{amy_storflash_flag=1;
												amy_storflash_pointer=0;
											}
											
											if(read_flash[i*21])
											{
												qwl_sendstring("�Ȳ�FLASH����");
											addr[0]=read_flash[i*21+1];
											addr[1]=read_flash[i*21+2];
											addr[2]=q2x1278_config_buf[4];
											amy_config_please(&addr[0]);
											config_recwait_time=0;	
												break;
											}
										}
					}
			

	
	
		
			else
				{
					amy_check_pointer++;
					amy_storflash_pointer++;	
					
					if(amy_check_pointer>=amy_max_total)
					amy_check_pointer=0;
					
										if(amy_storflash_pointer>=amy_max_total)//��һ����ѭ�󣬴洢��flash
												{
													amy_storflash_pointer=0;
													
													if(amy_storflash_flag==1)//���б�洢��flash
													{
														if(amy_total>0)//���ն������ٴ���FLASH
														{
																qwl_sendstring("����FLASH");
															//for(k=0;k<5250;k++)
															//	{
															//		qwl_sendchar(NO[amy_record_buf[k]>>4]);
															//			qwl_sendchar(NO[amy_record_buf[k]&0x0f]);
															//	}
																amy_storflash_flag=2;
																
																Write_Flash_ALLAMY();
														}
													 }	
												 }

					addr[0]=0;
					addr[1]=amy_check_pointer;
					addr[2]=q2x1278_config_buf[4];
					amy_config_please(&addr[0]);
					config_recwait_time=0;
				}
		}
		
	*/	
	amy_storflash_flag=2;
	
	if(config_recwait_time>1100)//��ʱ1100ms,��1
	{config_recwait_time=0;
		
			for(i=0;i<amy_max_total;i++)
		{
			if(config_recwait_buf[i]>0)
			{
				if((config_recwait_buf[i]&0x0f)>5)
				{qwl_sendstring("״̬�ϴ�����ʱ��̫��1");
					config_recwait_buf[i]=6;
				}
				if((config_recwait_buf[i]&0xf0)==0)
				{
					config_recwait_buf[i]++;
//					qwl_sendstring("��ʱ��һ");
//					qwl_sendchar(NO[(i>>4)]);
//					qwl_sendchar(NO[i&0x0f]);
				}
			}
		}
	}
	
	for(i=0;i<amy_max_total;i++)
	{
		if(config_recwait_buf[i]>0)
		{																	
			if(config_recwait_buf[i]&0xf0)
			{				
				config_recwait_buf[i]=0;
							k=i*21;
							for(n=0;n<16;n++)
							{
								amy_check_ID[n]=amy_record_buf[k+4+n];
								
							}
									j=amy_record_buf[k+20];
											if(j&0x04)//ת����ƽ̨��ʶ��Ĺ�����
													{j=1;
													}
													else if(j&0x20)//ת����ƽ̨��ʶ��Ĺ�����
													{j=3;
													}
												else if(amy_record_buf[k]>2)//��ʾ������3���ն��ϴ�ʱ����û�յ��ն�����
												{j=2;
													qwl_sendstring("״̬�ϴ�����ʱ��̫��2");
												}
												else if(1)
												{j=0;
												}
												
												poll_zd.zd_state[(i*2/8)]=(poll_zd.zd_state[(i*2/8)]&(~(jj<<(i*2%8)))|(j<<(i*2%8)));//��ȡ�ն�״̬
												
												if(amystate_send_flag[i]==1)//��Ħ��״̬���ͱ�־
												{amystate_pack_send(j,i);//״̬����ϴ�
													amystate_send_flag[i]=0;//��Ħ��״̬���ͱ�־
												}
			}
			else if((config_recwait_buf[i]&0x0f)>1)
				{
//					qwl_sendstring("��ʱ��ȡ");
//					qwl_sendchar(NO[(i>>4)]);
//					qwl_sendchar(NO[i&0x0f]);
					k=i*21;
							

												if(amy_record_buf[k]>0)//��ʾ������3���ն��ϴ�ʱ����û�յ��ն�����
												{
														j=2;//��ʱ
												poll_zd.zd_state[(i*2/8)]=(poll_zd.zd_state[(i*2/8)]&(~(jj<<(i*2%8)))|(j<<(i*2%8)));//��ȡ�ն�״̬
													qwl_sendstring("״̬�ϴ������������3");
													qwl_sendchar(NO[i>>4]);
													qwl_sendchar(NO[i&0x0f]);
													if(amy_storflash_flag!=0)//��ѯflashʱ�������ն˼Ӽ�
													{
														//if(amystate_send_flag[i]==0)//��Ħ��״̬��Ҫ����ʱ���������ն˼Ӽ�����
														//{
																	if(amy_record_buf[k]>=3)
																			{
																				for(k=0;k<21;k++)
																				{	qwl_sendchar(amy_record_buf[i*21+k]);
																					amy_record_buf[i*21+k]=0;//��ʱû����ʾ���޳�
																				
																				}
																				getwaylist_send_flag=1;//�б�Ҫ�������ϴ���־
																				getwaylist_send_times=0;
																								if(amy_total==0)
																								{qwl_sendstring("�ն˼�������1\r\n");
																								}
																				qwl_sendstring("�ն˼�������\r\n");				
																				amy_total--;//�����½�

																			}
																			else
																			{
																				
																				//if(amy_check_pointer==0)//�ٴ����󣬻����������
																				//amy_check_pointer=249;
																				//else
																				//amy_check_pointer--;
																					
																			amy_record_buf[k]++;
																			}
															//}
														}
												}
												
										
													
											else
												{qwl_sendstring("ָ��");
													
													qwl_sendchar(NO[amy_check_pointer>>4]);
													qwl_sendchar(NO[amy_check_pointer&0x0f]);
													qwl_sendchar(NO[i>>4]);
													qwl_sendchar(NO[i&0x0f]);												

													qwl_sendchar(0x0d);
													qwl_sendchar(0x0a);
													j=2;//��ʱ
												poll_zd.zd_state[(i*2/8)]=(poll_zd.zd_state[(i*2/8)]&(~(jj<<(i*2%8)))|(j<<(i*2%8)));//��ȡ�ն�״̬
												}

												
						
												j=2;//��ʱ
												//poll_zd.zd_state[(i*2/8)]=(poll_zd.zd_state[(i*2/8)]&(~(jj<<(i*2%8)))|(j<<(i*2%8)));//��ȡ�ն�״̬
											if(amystate_send_flag[i]==1)//��Ħ��״̬���ͱ�־
												{

													for(n=0;n<16;n++)
														{
															amy_check_ID[n]=amy_record_buf[k+4+n];

														}
													qwl_sendstring("��״̬��ʱ");
													amystate_pack_send(j,i);//״̬����ϴ�
													amystate_send_flag[i]=0;//��Ħ��״̬���ͱ�־
												}
												config_recwait_buf[i]=0;
			}
			
		}
	}
	
		
}

/*******************************************************************************
* �� �� ��         :amyack_send
* ��������		   	: ��Ħ�δ���ϴ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void amystate_send(u8 state)
{

	amyack_send_pack(state);
	USART3_sendstring1(post_buf,post_len);
			if(net.reconnect_setp==LINK_OK)
		{
			net_send(post_buf,post_len);
		}	
}



