#include "config.h"


u8 amy_config=0;
u8 amy_config1=0;
u8 PLC_time_flag=0;
u8 PLC_time_tiqian=0;//�ն�û����ʱʱ�����ǰ����
u8 time_share_process_flag=0;//��Ħ�η�ʱ�����־��־
u8 ceshi=0;//���ڲ���״̬������־
u16 zd_overtime_flag=0;//�ն˳�ʱ��ʶ����ʱû���յ����������б�־��λ
u8 AmyID[16];



#define   MAX_AMY_BUF  5250			//���250̨��Ħ��
u8 amy_record_buf[MAX_AMY_BUF]={0};		//��Ħ�����ݻ�����
u8 amy_rec_flag;//��Ħ�ν��ܱ�־λ
u8 amy_total=0;//��Ħ������
u8 amy_CHKSUM=0;//��Ħ��У���룬�����ն�ID��Ӻ�
u8 armchair_time_flag=0;//��Ħ��״̬��ȡ��ֹ��ʱ��־

#define   MAX_AMY_BUF_1  50			//��־ͷ 2λģ���ַ 1λƵ�ε�ַ 16�ն�ID 1λ״̬ 1λУ���
u8 amy_record_buf_1[MAX_AMY_BUF_1]={0};		//��Ħ�����ݻ�����
u8 amy_rec_flag=0;//��Ħ�ν��ܱ�־λ
u8 amy_rec_flag_1=0;//��Ħ�ν��ܱ�־λ��������

u16 amy_cotrol_time=0;//��Ħ�ο���ʱ��
u16 amy_cotrol_time_flag=0;//��ʱ���м�ʱ
u8 amy_cotrol_flag=0;//��Ħ�ο��Ʊ�־
u8 IO_STATE=0;

u8 wg_ack_buf[200]={0};//�����յ����ݺ󣬽��з������棬�����������ڴ���
u8 wg_stor_pointer=0;//���ش洢ָ��
u8 wg_deal_pointer=0;//���ش���ָ�룬��ǰ������λ��
u8 wg_ack_times=0;//��Ҫ����������


u8 wg_state_flag=0;//��ѯ����������Ħ��״̬��־
u8 wg_set_flag=0;//����ֱ��������Ħ�α�־

u8 zd_update_flag=0;//�ն�������־
u8 armchair_fen_delay=0;
u16 armchair_s_delay=0;
void PLCtime_share_process(void);

u8 SIM_GET_FLAG=0;
Shared_TYPE Shared_Module;//�豸ѡ��

u8 shebei_flag=0;//�豸״̬����

void ac_switch_IN(void);
void ac_switch_OUT(void);
/*******************************************************************************
* �� �� ��         : armchair_io_Init
* ��������		   : armchair_io��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Armchair_IO_Init()	  //�˿ڳ�ʼ��
{
	GPIO_armchair_INIT();
	Tm618_IO_Init();
}
/*******************************************************************************
* �� �� ��         : slot_machines_data_send()
* ��������		   : ÿ���ӷ�һ���͵�ƽ�����150ms��ÿ������1ms
* ��    ��         : 0������1�ر�
* ��    ��         : ��
*******************************************************************************/
void slot_machines_data_send()
{
	qwl_sendstring("������Ħ��");
	armchair_status_OUT=0;
	Delay_MS(40);
	armchair_status_OUT=1;
}
/*******************************************************************************
* �� �� ��         : void Shared_Module_switch(u8 module_state)
* ��������		   : ��������ģ��
* ��    ��         : 0������1�ر�
* ��    ��         : ��
*******************************************************************************/
void Shared_Module_switch(u8 module_state)
{
	switch(Shared_Module.Module)
	{
		case oxygen_plant:
		{
			if(module_state==0)
			armchair_status_OUT=0;//����������
			else
			armchair_status_OUT=1;//�������ر�
		}break;
		case slot_machines:
		{
			qwl_sendstring("����Ͷ�һ�");
			if(amy_cotrol_time>0)
			{u8 i;
				i=amy_cotrol_time/60;
				if(amy_cotrol_time%60)
				{
					i++;
				}
				if(amy_cotrol_flag!=1)
				API_SetTimer(150,i,TIMER_TYPE_QWL_TOUBIJI_DEAL,slot_machines_data_send,NULL);
			}
		}break;
		default:
		{
		}break;
	}
	
}
/*******************************************************************************
* �� �� ��         : API_ARMCHAIR_TIME
* ��������		   : ��ʱ��ʱ��־���ô�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ARMCHAIR_TIME()
{
	config_recwait_time++;
	
	armchair_s_delay++;
	if(armchair_s_delay>1000)//���ʱ
	{armchair_s_delay=0;
			armchair_fen_delay++;
			if(armchair_fen_delay>59)//���Ӽ�ʱ
			{
				armchair_fen_delay=0;
				poll_zd.poll_time ++;//���Ӽ�һ
			}
	}
}
/*******************************************************************************
* �� �� ��         :key_q2x1278_set();
* ��������		   	: 433����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void key_q2x1278_set()
{
	
		switch(TM1618_DISPLAY_NO[0])
	{
		case 0://��־λ0��������ʾ
		{
			if(TM1618_DISPLAY_NO[0]==0)
			TM1618_DISPLAY_NO[0]++;
			amy_config=amy_config|0x08;//�������״̬
			TM1618_DISPLAY_NO[2]=(q2x1278_config_buf[2]&0x0f);//�������ʾ��ֵ
			TM1618_DISPLAY_NO[1]=(q2x1278_config_buf[2]>>4);//�������ʾ��ֵ
			};break;
		case 1://����ģ���ַ��λ
		{u8 i;
			i=(TM1618_DISPLAY_NO[1]<<4)+TM1618_DISPLAY_NO[2];			
			q2x1278_config_buf1=i;//��ʱ�洢ģ���ַ��λ
			
			TM1618_DISPLAY_NO[0]++;
			TM1618_DISPLAY_NO[1]=(q2x1278_config_buf[4]>>4);
			TM1618_DISPLAY_NO[2]=(q2x1278_config_buf[4]&0x0f);

		};break;
		case 2://����ģ���ַ��λ
		{u8 i;

			i=(TM1618_DISPLAY_NO[1]<<4)+TM1618_DISPLAY_NO[2];			
			q2x1278_config_buf[4]=i;
			q2x1278_config_buf[2]=q2x1278_config_buf1;
			q2x1278_config_buf[1]=0x00;//��ַ��λ��ʼ��Ĭ��Ϊ00	
			q2x1278_config_buf[0]=0xc0;//433��������
			q2x1278_config_buf[3]=0x1A;//������
			q2x1278_config_buf[5]=0xc0;//���㷢��
			TM1618_DISPLAY_NO[0]=0;	
			amy_config=amy_config&0xf7;//��������״̬
	
				i=q2x1278_set();//���в�������
			if(i==0)
			qwl_sendstring("433���óɹ�");
			else
				qwl_sendstring("433����ʧ��");
			
			if(q2x1278_config_buf[2]!=0xff)
			PLC_time_tiqian=1;//���ٷ�������//���㰲װ��Ա����֪����Ħ��״̬
		};break;
	default:break;
	}
	

}
/*******************************************************************************
* �� �� ��         :key_process()
* ��������		   	: ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void key_process()
{
	u8 key;
if(TM1618_DISPLAY_NO[0]!=0)//��ʱ�Զ��˳����ý���
{
	if(key_delay_out>59)
	{key_delay_out=0;
			TM1618_DISPLAY_NO[0]=0;	
			amy_config=amy_config&0xf7;//��������״̬
			TM1618_DISPLAY_NO[2]=(q2x1278_config_buf[2]&0x0f);//�������ʾ��ֵ
			TM1618_DISPLAY_NO[1]=(q2x1278_config_buf[2]>>4);//�������ʾ��ֵ
	}
}
		key=read_key();
	if(key>0)//�а����Ž��д�����Ȼ����
	{key_delay_out=0;
	switch(key)
	{
		case 1:
		{
			key_q2x1278_set();//433����
			
		}break;
		case 2:
		{u8 i;
			if(TM1618_DISPLAY_NO[0]!=0)//��������״̬�²ſ��Ե���
			{
			i=(TM1618_DISPLAY_NO[1]<<4)+TM1618_DISPLAY_NO[2];			
			i++;//���ݼ�
				if(TM1618_DISPLAY_NO[0]==0x02)
				{
					if(i>31)
					i=0x0;
				}
				else
				{
				if(i==0xff)
					i=0;
				if(i>=amy_max_total)
					i=0xff;
				}
			TM1618_DISPLAY_NO[2]=(i&0x0f);
			TM1618_DISPLAY_NO[1]=(i>>4);
				
			}
			
		}break;
		case 3:
		{u8 i;
				if(TM1618_DISPLAY_NO[0]!=0)//��������״̬�²ſ��Ե���
			{
			i=(TM1618_DISPLAY_NO[1]<<4)+TM1618_DISPLAY_NO[2];
			i--;//���ݼ�
				if(i==0xfe)
				{
					if(amy_max_total>0)
						i=amy_max_total-1;
					else
						i=0;
				}
				
				if(TM1618_DISPLAY_NO[0]==0x02)
				{
					if(i==0)
					{
					i=31;
					}
				}
					
			TM1618_DISPLAY_NO[2]=(i&0x0f);
			TM1618_DISPLAY_NO[1]=(i>>4);
			}
		}break;
	default:break;
	}
	if(amy_cotrol_time==0)
		Delay_MS(500);
}
	
	
	
}
/*******************************************************************************
* �� �� ��         :q2x1278_rec_or_send()
* ��������		   	: 433�������ػ����ն˷��ͻ��߽��ܽ����жϴ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void q2x1278_rec_or_send()
{

if(exchange_platform.Platform_Choice==PLATFORM_CHOICE_PUBLIC)
	{
         GATEWAY_send();//�������������ʹ������	
	}
				amy_ack_send();
		if((amy_config&0x02)==0x02)//���簴Ħ�������أ�
		{

	if(exchange_platform.Platform_Choice==PLATFORM_CHOICE_PUBLIC)
	{
			

			amy_state_send();
			amy_config_send();

	LED=!LED;
	}
		}	
		
//------		
		else if((amy_config&0x02)==0x00)//���簴Ħ�����ն�
		{
				if((amy_config&0x01)==0x01)//���簴Ħ���У����д�����Ȼֱ���˳�
				{
					
							
						PLCtime_share_process();//�Զ�ȡ��Ħ��״̬���ն˷��ͣ����з�ʱ����
						
				}
			
		}
		
		wg_amy_state();//���ܣ���������ֱ����Ħ�Σ�ֱ��������ѯ��Ħ��״̬

		
}
/*******************************************************************************
* �� �� ��         : ARMCHAIR_DEAL
* ��������		   : ��Ħ�δ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ARMCHAIR_DEAL()
{
//	config_recwait_time++��
		ceshi=1;
	Tm618_Display();//�������ʾ���ڲ����������趨ʱ���������˸��ʾ
		ceshi=2;
	key_process();	//��������
		ceshi=3;
			
	q2x1278_rec_or_send();//���������ж����ն˻������ؽ��д���
		ceshi=4;
	//S1_rec_DEAL();	//����1���ܴ���
	//	ceshi=6;
	q2x1278_fb();	//���ж������ò��ɹ������Է��⴦��
}
/*******************************************************************************
* �� �� ��         : Self_Check()
* ��������		   	: ϵͳ�Լ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Self_Check()
{u8 i;
u8 j;
	
	qwl_sendstring("������˹��������˿��Լ죺\r\n");
				i=armchair_io_state();
			if(i==0)//��Ħ��״̬��ȡ�ɹ�
							{	
							qwl_sendstring("IO�˿�����\r\n");
							shebei_flag=shebei_flag&0xfe;//������˹IO������
							}
							else if(i==1)
							{	
							qwl_sendstring("IO�˿�ʧ��\r\n");
							shebei_flag=shebei_flag|0x01;//������˹IO������
							}	
							else if(i==2)
							{	
							qwl_sendstring("�˿ڻ�Ħ�λ���\r\n");
							shebei_flag=shebei_flag|0x01;//������˹IO������
							}	
							
								ac_switch_IN();
	
							if(GPIO_ReadInputDataBit(GPIOE,ac_status_out))
							{	
								qwl_sendstring("Ͷ�Ҷ˿ڼ���\r\n");
							shebei_flag=shebei_flag&0xFD;//Ͷ�Ҷ˿�
							}
							else
							{
								qwl_sendstring("Ͷ�Ҷ˿ڼ���\r\n");
							shebei_flag=shebei_flag|0x02;//Ͷ�Ҷ˿�
							}
							ac_switch_OUT();
	qwl_sendstring("flashID:");
	qwl_sendchar(NO[dev_id>>4]);
	qwl_sendchar(NO[dev_id&0x0f]);
							if(dev_id>0)
							{
								qwl_sendstring("flash����");
								shebei_flag=shebei_flag&0xF7;//flash����
							}
							else
							{qwl_sendstring("flashʧ��");
								shebei_flag=shebei_flag|0x08;//flash����
							}
							
				if((shebei_flag&0x09)==0)
				{
					if(shebei_flag&0x02)
					{
						Tm618_Smg_8882();//��ʾ������޵�
						
					}
					else
					{
						Tm618_Smg_8881();//��ʾ������е�
					}
				}
					
	i=q2x1278_rec_cofig();//433ģ���飬��ȡ���ò�������ʾ
	if(i==0)
		{

		qwl_sendstring("433����");
		shebei_flag=shebei_flag|0x04;//433����
			TM1618_DISPLAY_NO[0]=0x00;
			TM1618_DISPLAY_NO[2]=(q2x1278_config_buf[2]&0x0f);//�������ʾ��ֵ
			
			TM1618_DISPLAY_NO[1]=(q2x1278_config_buf[2]>>4);//�������ʾ��ֵ
			
			amy_config=amy_config|0x01;//���ش��ڷ�
			
			if(q2x1278_config_buf[2]==0xff)//
			amy_config=amy_config|0x02;//�ն˻�������	
			else
			{
				
				j=q2x1278_config_buf[2];
			
			TM1618_DISPLAY_NO[0]=0;

			TM1618_DISPLAY_NO[2]=(j&0x0f);//�������ʾ��ֵ
			
			TM1618_DISPLAY_NO[1]=(j>>4);//�������ʾ��ֵ
			}
					amy_config=amy_config|0x10;//Ĭ������ͨ�Ų���
		}
	else
		{
			qwl_sendstring("433������");
			shebei_flag=shebei_flag&0xfb;//433�����ڣ�����
			amy_config=amy_config&0xfe;//���ش��ڷ�
			amy_config=amy_config&0xfd;//�ն˻�������
		}	

		
		
//		amy_state();//�������Ħ���նˣ����а�Ħ��״̬��ѯ,ʵʱ��������ﲻ���г�ʼ����
		
		rx_pointer=0;//����1���������
		rx2_pointer=0;//����2���������
		
		

}
/*******************************************************************************
* �� �� ��         : armchair_Init
* ��������		   : armchair��ʼ������,������ʱ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Armchair_Init()	 
{
	q2x1278_init();
	Tm618_Smg_On();
	Init_Amy_Id();
	Self_Check();//ϵͳ�Լ�	
	/*���ð�Ħ�γ�ʱʱ��*/
	API_SetTimer(1,-1,TIMER_TYPE_ARMCHAIR_TIME,API_ARMCHAIR_TIME,NULL);

}
/*******************************************************************************
* �� �� ��         : ac_switch_IN
* ��������		   : AC_SWITCH��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ac_switch_IN()
{
	GPIO_InitTypeDef GPIO_InitStructure;
   			//  ����GPIO��ģʽ��IO�� 
		GPIO_InitStructure.GPIO_Pin=ac_status_out;	 //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;	  //������������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOE,&GPIO_InitStructure); // ��ʼ��GPIO 

}

/*******************************************************************************
* �� �� ��         : ac_switch_OUT
* ��������		   : AC_SWITCH�������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ac_switch_OUT()
{GPIO_InitTypeDef GPIO_InitStructure;
   			//  ����GPIO��ģʽ��IO�� 
		GPIO_InitStructure.GPIO_Pin=ac_status_out;	 //ѡ����Ҫ���õ�IO��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	  //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //���ô�������
	GPIO_Init(GPIOE,&GPIO_InitStructure); // ��ʼ��GPIO 
	armchair_status_OUT=1;
}
/*******************************************************************************
* �� �� ��         : armchair_change_outIO()
* ��������		   : �ı��밴Ħ������˲�ͬ�����IO��״̬���ð�Ħ�ζ˿ڸ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 armchair_change_outIO(void)
{
	u8 i=0;
	
	armchair_switch=0;//��Ħ�γ�ʼ������ֹͣ��������
	Delay_MS(100);
	if(!armchair_status_IN)//==IO_STATE)
		{	
			i=0;
		}
		else
		{
			i=2;
		}
		armchair_switch=1;//��Ħ�γ�ʼ������ֹͣ��������
		
//	ac_switch_IN();
	
//	if(GPIO_ReadInputDataBit(GPIOE,ac_switch))
//	{		ac_switch_OUT();

	
//		GPIO_ResetBits(GPIOE,ac_switch);
//		GPIO_ResetBits(GPIOE,ac_status_in);//���ڲ���IO���������������Ŀ�
//		IO_STATE=0;
//	}
//			else
//			{
//				ac_switch_OUT();
	//			GPIO_SetBits(GPIOE,ac_switch);
		//		GPIO_SetBits(GPIOE,ac_status_in);//���ڲ���IO���������������Ŀ�
			//	IO_STATE=1;

//			}
	
	
	return i;
}
/*******************************************************************************
* �� �� ��         : armchair_io_state
* ��������		   : ��ȡ��Ħ��״̬���������߲�����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 armchair_io_state_1()
{ u8 i;
//	ac_switch_IN();

		if(armchair_status_IN)//==IO_STATE)
		{	
			i=0;//��ѯ��Ħ��״̬��
		}
		else
		{	i=1;
		}
	//	ac_switch_OUT();
	//	if(IO_STATE)
	//		GPIO_SetBits(GPIOE,ac_switch);
	//	else
	//		GPIO_ResetBits(GPIOE,ac_switch);
			

return i;
}
/*******************************************************************************
* �� �� ��         : armchair_io_state
* ��������		   : ��ȡ��Ħ��״̬���������߲�����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 armchair_io_state()
{ u8 i;
//	ac_switch_IN();
		if(armchair_status_IN)//==IO_STATE)
		{	
			i=armchair_change_outIO();//��ѯ��Ħ��״̬��
		}
		else
		{	i=1;
		}
	//	ac_switch_OUT();
	//	if(IO_STATE)
	//		GPIO_SetBits(GPIOE,ac_switch);
	//	else
	//		GPIO_ResetBits(GPIOE,ac_switch);
			

return i;
}

/*******************************************************************************
* �� �� ��         :PLCtime_share_process()
* ��������		   	: �ն˷�ʱ�����־
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void PLCtime_share_process()
{
	if(zd_overtime_flag>1000)
	{zd_overtime_flag=0;
		amy_config=amy_config|0x10;//
	}
	if(armchair_io_state_1()==0)
	{
		LED=!LED;//״̬�Ƴ�������Ħ���ڹ���
	}
	
		switch(time_share_process_flag)
	{
		
		case 0: 
		{u8 i;
			if(PLC_time_tiqian>0)//��־����
					{
						PLC_time_tiqian=0;
						PLC_time_flag=0;
							i=armchair_io_state();
							if(i==0)//��Ħ��״̬��ȡ�ɹ�
							{
								
							qwl_sendstring("���Ƴɹ�\r\n");
								amy_config=amy_config&0xdb;//	
							}
							else if(i==1)
							{	
							qwl_sendstring("����ʧ��\r\n");
							amy_config=amy_config|0x04;
							}	
							else if(i==2)
							{	
							qwl_sendstring("��Ħ�λ���\r\n");
							amy_config=amy_config|0x20;
							}	
								q2x1278_plc_send();		
								time_share_process_flag=3;					
							rx2_pointer=0;//������ܻ�����
							armchair_time_flag=0;

				}
		}break;
		
		case 1: 
		{u8 i;
			if(PLC_time_tiqian>0)//��־����
					{
						PLC_time_tiqian=0;
						PLC_time_flag=0;
							i=armchair_io_state_1();
							if(i==0)//��Ħ��״̬��ȡ�ɹ�
							{
								
							qwl_sendstring("���Ƴɹ�\r\n");
								amy_config=amy_config&0xdb;//	
							}
							else if(i==1)
							{	
							qwl_sendstring("����ʧ��\r\n");
							amy_config=amy_config|0x04;
							}	
						
								q2x1278_plc_send();		
								time_share_process_flag=3;					
							rx2_pointer=0;//������ܻ�����
							armchair_time_flag=0;

				}
		}break;
		
		case 3: 
		{ u8 i;
			i=q2x1278_check_send();
			if(i==0)
							{
								time_share_process_flag=0;
								armchair_time_flag=0;
								qwl_sendstring("�ն˷���OK");
								amy_config=amy_config&0xef;//
								amy_config1=amy_config1&0xfc;//
								zd_overtime_flag=0;//��ʱδ��ͨ�ű�־
								
							}
			else if(i==1)
			{
				time_share_process_flag=0;
				armchair_time_flag=0;
				qwl_sendstring("Ƶ����ͬ");
				amy_config1=amy_config1|0x02;//
				zd_overtime_flag=0;//��ʱδ��ͨ�ű�־
			}
			else if(i==2)
			{
				time_share_process_flag=0;
				armchair_time_flag=0;
				qwl_sendstring("ID��ͬ");
				amy_config1=amy_config1|0x01;//
				zd_overtime_flag=0;//��ʱδ��ͨ�ű�־
			}
							else if(armchair_time_flag>3)//��ʱ
							{	time_share_process_flag=0;
								armchair_time_flag=0;
//								PLC_time_tiqian=1;//���ٷ�������
								qwl_sendstring("�ն˷��ͳ�ʱERROR");
								amy_config=amy_config|0x10;//
								
								//���о��Ա�������ʱҲ���ٷ�
						/*		//��ʱ���ٷ�һ��
								qwl_sendstring("��ʱ�ٷ�һ��");
								i=armchair_io_state();
							if(i==0)//��Ħ��״̬��ȡ�ɹ�
							{
								
								amy_config=amy_config&0xfb;//	
							}
							else
							{	
							amy_config=amy_config|0x04;
							}	
								q2x1278_plc_send();	*/	
							}
								


		}break;

		default: break;
	}
}
/*******************************************************************************
* �� �� ��         : zd_rec_deal
* ��������		   : �ն˽��ܴ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void zd_rec_deal()
{
				u8 i;
				u8 j;

//				u8	ZD_MODE=0;//�ն�ģʽ
	switch(amy_record_buf_1[1])
			{
						case 0xb0://�ն�״̬��ȡ����
						{
							i=((amy_record_buf_1[2]-0x30)<<4)+(amy_record_buf_1[3]-0x30);//ASCIIת��BCD,ģ��ַ
							j=((amy_record_buf_1[4]-0x30)<<4)+(amy_record_buf_1[5]-0x30);
							amy_cotrol_time=(u16)(i<<8)+j;
							if(amy_cotrol_time>0)//��������ʱ���������
							{
							amy_cotrol_flag=1;
							amy_cotrol_time_flag=0;//��Ħ�μ�ʱ����
							armchair_switch=0;//��Ħ�γ�ʼ������ֹͣ��������
								Shared_Module_switch(0x00);//0���� 1�ر�
							qwl_sendstring("��Ħ�ο�ʼ����88");
							GPIO_ResetBits(GPIOD,LED);
							}
//							ZD_MODE=((amy_record_buf_1[21]-0x30)<<4)+(amy_record_buf_1[22]-0x30);//ASCIIת��BCD,Ƶ��
//					USART2_sendchar(0x00);//����ģ���ַ
//				USART2_sendchar(0xff);//
//				USART2_sendchar(q2x1278_config_buf[4]);//Ƶ��
//							USART2_sendstring("SEND_OK");
							PLC_time_tiqian=1;
							time_share_process_flag=1;							
							PLCtime_share_process();

						}break;
						
				case 0xb1://�ն�״̬��ȡ����
						{

							PLC_time_tiqian=1;
							time_share_process_flag=0;							
							PLCtime_share_process();

						}break;
				case 0xf8:
				{
					zd_update_flag=1;
				}
				default:
					break;
			}
}
/*******************************************************************************
* �� �� ��         : wg_rec_deal
* ��������		   : ���ؽ��ܴ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void wg_rec_deal()
{				u8 i;
				u16 k;
				u8 n;
				u8 PLC_ADR[3]={0};
				u8 rec_id_buf[16];//��Ħ��ID��ʱ�洢
//				u8 *read_flash;
				switch(amy_record_buf_1[1])
				{

					case 0xa0:
						{
							for(i=0;i<16;i++)
							rec_id_buf[i]=amy_record_buf_1[i+8];//�洢��Ħ�����ݣ�������Ӧλ��
							
							PLC_ADR[0]=((amy_record_buf_1[2]-0x30)<<4)+(amy_record_buf_1[3]-0x30);//ASCIIת��BCD,ģ��ַ
							PLC_ADR[1]=((amy_record_buf_1[4]-0x30)<<4)+(amy_record_buf_1[5]-0x30);
							PLC_ADR[2]=((amy_record_buf_1[6]-0x30)<<4)+(amy_record_buf_1[7]-0x30);//ASCIIת��BCD,Ƶ��
							
							
							k=PLC_ADR[1];
							k=k*21;

							if(amy_record_buf[k]>0)//�洢ǰ����Ƿ��Ѿ�����
							{
								
			if(config_recwait_buf[PLC_ADR[1]]>0)
			{
				config_recwait_buf[PLC_ADR[1]]=config_recwait_buf[PLC_ADR[1]]|0x80;//��Ӧ��״̬��־λ��1����ʾ״̬�����ɹ���Ҫ����״̬
			}
					
								for(i=0;i<16;i++)
								{	
									if(rec_id_buf[i]!=amy_record_buf[k+i+4])
									break;

								}
										
								if(i==16)
								{
								amy_record_buf[k]=1;//�Ѿ����ڣ���־λ��1����ʼ����ʶ�Ѿ�����
									
								//qwl_sendstring("���ն��Ѿ���");
								amy_record_buf[k+20]=((amy_record_buf_1[24]-0x30)<<4)+(amy_record_buf_1[25]-0x30);
									
									if(wg_ack_times>=20)
									{qwl_sendstring("����̫�࣬��������");
									}
									
									wg_ack_times++;//������������
							if(wg_stor_pointer>=200)
								wg_stor_pointer=0;
							
							//�ݴ棬�ȴ�����
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[0];//ģ���ַ//�����ն˷��ͳɹ�
							wg_stor_pointer++;		
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[1];
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[2];//Ƶ��
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='S';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='E';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='N';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='D';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='_';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='O';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='K';
							wg_stor_pointer++;
							
								}
								else
								{
									if(wg_ack_times>=20)
									{qwl_sendstring("����̫�࣬��������");
									}
									amy_record_buf[k+20]=((amy_record_buf_1[24]-0x30)<<4)+(amy_record_buf_1[25]-0x30);//ʵʱ״̬����Ҫ���£��·�ʽ��ID������
									wg_ack_times++;//������������
							if(wg_stor_pointer>=200)
								wg_stor_pointer=0;
									//�ݴ棬�ȴ�����
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[0];//ģ���ַ//�����ն˷��ͳɹ�
							wg_stor_pointer++;		
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[1];
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[2];//Ƶ��
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='E';//�븽��һ̨����Ƶ����ͬ
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='R';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='R';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='O';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='R';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='0';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='1';
							wg_stor_pointer++;
								}
							}
							else
							{	
								n=strsearch_qwl(rec_id_buf,amy_record_buf,5250,16);//��ȡ�����Ƿ������̨
								if(n==0)
								{

									if(PLC_ADR[2]==q2x1278_config_buf[4])//�ж���ͬƵ���ڵĽ��д洢
									{
										if(config_recwait_buf[PLC_ADR[1]]>0)//�ж��ǲ����Լ�������նˣ������Լ�������޳�
										{
							for(i=1;i<4;i++)
							amy_record_buf[k+i]=PLC_ADR[i-1];//�洢��Ħ�����ݣ�������Ӧλ��
							for(i=4;i<20;i++)
							amy_record_buf[k+i]=amy_record_buf_1[i+4];//�洢��Ħ�����ݣ�������Ӧλ��
							amy_record_buf[k+20]=((amy_record_buf_1[24]-0x30)<<4)+(amy_record_buf_1[25]-0x30);
								amy_record_buf[k]=1;//��־λ��1����ʼ
									config_recwait_buf[PLC_ADR[1]]=config_recwait_buf[PLC_ADR[1]]|0x80;//��Ӧ��״̬��־λ��1����ʾ״̬�����ɹ���Ҫ����״̬
							amy_total++;//������������
							getwaylist_send_flag++;//�б�Ҫ�������ϴ���־
							amy_rec_flag=0;//���¿�ʼ��ȡ��־λ
								qwl_sendstring("���ն˴�ȡ�ɹ�666");

								for(i=0;i<21;i++)
							{
								qwl_sendchar(NO[amy_record_buf[k+i]>>4]);
								qwl_sendchar(NO[amy_record_buf[k+i]&0x0f]);
							}
									
//									read_flash=(u8*)(flash_AMY_adr1);
									
//												if(amy_storflash_flag==2)
//												{
//														for(i=4;i<20;i++)
	//													{
//															if(read_flash[k+i]!=amy_record_buf[k+i])
//																break;
//																
	//													}
//														if(i==20)//�жϱ����Ƿ��Ѿ��У��м��룬û���޳�
//														{qwl_sendstring("���£�����flash");
//														Write_Flash_ALLAMY();//����FLASH��
//														}
//												}
								
										if(wg_ack_times>=20)
									{qwl_sendstring("����̫�࣬��������");
									}
									
									wg_ack_times++;//������������
							if(wg_stor_pointer>=200)
								wg_stor_pointer=0;
							//�ݴ棬�ȴ�����
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[0];//ģ���ַ//�����ն˷��ͳɹ�
							wg_stor_pointer++;		
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[1];
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[2];//Ƶ��
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='S';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='E';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='N';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='D';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='_';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='O';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='K';
							wg_stor_pointer++;
						}
										else
										{
											qwl_sendstring("�����Լ�������ն�");
											qwl_sendchar(NO[PLC_ADR[0]>>4]);
											qwl_sendchar(NO[PLC_ADR[0]&0x0f]);
											qwl_sendchar(NO[PLC_ADR[1]>>4]);
											qwl_sendchar(NO[PLC_ADR[1]&0x0f]);
											qwl_sendchar(NO[PLC_ADR[2]>>4]);
											qwl_sendchar(NO[PLC_ADR[2]&0x0f]);
										}
					}
									else
										{
											qwl_sendstring("����ͬƵ���ڵ��ն�");
											qwl_sendchar(NO[PLC_ADR[0]>>4]);
											qwl_sendchar(NO[PLC_ADR[0]&0x0f]);
											qwl_sendchar(NO[PLC_ADR[1]>>4]);
											qwl_sendchar(NO[PLC_ADR[1]&0x0f]);
											qwl_sendchar(NO[PLC_ADR[2]>>4]);
											qwl_sendchar(NO[PLC_ADR[2]&0x0f]);
										}
								}
								else
								{	
									
									if(wg_ack_times>=20)
									{qwl_sendstring("����̫�࣬��������");
									}
									
									wg_ack_times++;//������������
							if(wg_stor_pointer>=200)
								wg_stor_pointer=0;
																		//�ݴ棬�ȴ�����
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[0];//ģ���ַ//�����ն˷��ͳɹ�
							wg_stor_pointer++;		
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[1];
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]=PLC_ADR[2];//Ƶ��
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='E';//�ն��ظ�
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='R';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='R';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='O';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='R';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='0';
							wg_stor_pointer++;
							wg_ack_buf[wg_stor_pointer]='2';
							wg_stor_pointer++;
								}
							}
						}break;
					case 0xc1:
					{
						qwl_sendstring("������������");

					}break;
					default:break;
					}
					

				

}
/*******************************************************************************
* �� �� ��         : ARMCHAIR_S_IQR()	  //���ж϶�ʱ���з���
* ��������		   : ��Ħ�����ݴ洢
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ARMCHAIR_S_IQR()	  //���ж϶�ʱ���з���
{
	
/*	time_flag++;
	if(time_flag>60)
	{
		amy_print_string("��Ħ�ο���");
		amy_print_char((ceshi>>4)+0x30);
		amy_print_char((ceshi&0x0f)+0x30);
	}*/
	PLC_time_flag++;
	armchair_time_flag++;
	GATEWAY_heart_time_flag++;//������ʱ������־
	GATEWAY_list_time_flag++;//�б�ʱ������־
	key_delay_out++;//������ʱδ�ж����Զ��˳�����
	zd_overtime_flag++;//�ն˳�ʱ��ʶ����ʱû���յ����������б�־��λ
	suipian_delay++;//Ϊ�˱��ֿ��ٷ�Ӧ���ӳ�ʱ�䷢��00����
	GATEWAY_off_time_flag++;
	CSQ_DELAY++;//CSQ��ȡ��ÿ10S��
	//nblot_read_flag1_delay++;
	
	if(amy_cotrol_flag==1)
	{
		
	amy_cotrol_time_flag++;
	if(amy_cotrol_time_flag>=amy_cotrol_time)
			{amy_cotrol_flag=0;
			LED=1;
			qwl_sendstring("��Ħ�ι���ֹͣ");
				armchair_switch=1;//��Ħ�γ�ʼ������ֹͣ��������
				getwayoff_send_flag=getwayoff_send_flag|0x01;//��Ħ��ֹͣ�����ϱ�
				if(Shared_Module.Module == oxygen_plant)
				Shared_Module_switch(0x01);//0���� 1�ر�
				}
			else
			{
				if(getwayoff_send_flag==0)
				{
					u8 i;
					i=armchair_io_state_1();
					if(i==0)
					{
						getwayoff_send_flag=getwayoff_send_flag|0x04;//�����а�Ħ��״̬����
					}
				}
							armchair_switch=0;;//��Ħ�γ�ʼ������ֹͣ��������
				if(Shared_Module.Module == oxygen_plant)
				Shared_Module_switch(0x00);//0���� 1�ر�
						LED=0;
			}
			
	}
	
}
/*******************************************************************************
* �� �� ��         : Amy_Record_Store
* ��������		   : ��Ħ�����ݴ洢
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Amy_Record_Store(u8 ch)
{

	if(ch==0xff)
	{
			amy_rec_flag=1;
	}
		
	if((amy_config&0x01)==0x01)//433���ڣ�����������
	{
		switch (amy_rec_flag)
		{

			case 1:
			{				
					amy_rec_flag=2;//������־λ����ʼ�洢
				 amy_rec_flag_1=0;//��ʼ������ȡ	
			}break;
				case 2:
			{u8 i;

				if(ch==0xfe)//������β����
				{
					if(amy_record_buf_1[0]==amy_rec_flag_1)//��ѯ�봫�������У����Ƿ�һ�£�һ�¾ʹ洢����Ȼ����
					{	qwl_sendstring("S2����������");
	//							qwl_sendchar(NO[config_recwait_time>>12]);//ԭ�����ڲ���433����ʱ��
//						qwl_sendchar(NO[(config_recwait_time>>8)&0x000f]);
///						qwl_sendchar(NO[(config_recwait_time>>4)&0x000f]);
//						qwl_sendchar(NO[(config_recwait_time)&0x000f]);
						if((amy_config&0x02)==0x02)//������
						{
							
							wg_rec_deal();
						}
						else
						{
							zd_rec_deal();
						}
						amy_rec_flag=0;
					}	
					else
					{
						amy_rec_flag=0;//
						qwl_sendstring("S2�����������⣬����,�������ݣ�");
						for(i=0;i<amy_rec_flag_1;i++)
						qwl_sendchar(amy_record_buf_1[i]);
					}
				}
				else
				{amy_record_buf_1[amy_rec_flag_1]=ch;//������ʱ���������ȴ�����
					amy_rec_flag_1++;
				}
				
			}break;
			default: break;
		}
	}
	

}




