#include "config.h"

u8 q2x1278_config_buf[6];//433���ò���
u8 q2x1278_config_buf1=0;//���ڰ����ڵ���ģ���ַʱ��ʱ�洢�����㰴����ʱ�˳�ʹ��

u8 config_recwait_buf[256]={0};//���÷����ȴ�����
u16 config_recwait_time=0;//���÷����ȴ���ʱ

u8 sx1278_fb_flag=0;


u8 Uart_RecData_buf[MAX_RX_BUF];
u8 rx_pointer=0;


u8 Uart2_RecData_buf[MAX_RX2_BUF];
u8 rx2_pointer=0;

/*******************************************************************************
* �� �� ��         : USART2_sendchar(u8 ch)
* ��������		   : ����2��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void USART2_sendchar(u8 ch)
{
			USART_SendData(USART2,ch);//ͨ������USARTx���͵�������
		
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);

}

/*******************************************************************************
* �� �� ��         : USART2_sendstring(u8 *p)
* ��������		   : ����2��������λ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void USART2_sendstring(u8 *p)
{
  	while(*p)
  	{
  		USART2_sendchar(*p);
  		p++;
	}

}
/*******************************************************************************
* �� �� ��         : q2x1278_fb
* ��������		   : 433����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void q2x1278_fb()
{u8 i;
	if(sx1278_fb_flag)
	{
		sx1278_fb_flag=0;
		TM1618_DISPLAY_NO[0]=0;	
			amy_config=amy_config&0xf7;//��������״̬
	
				i=q2x1278_set();//���в�������
			if(i==0)
			qwl_sendstring("433���óɹ�");
			else
				qwl_sendstring("433����ʧ��");
			
			if(q2x1278_config_buf[2]!=0xff)
			PLC_time_tiqian=1;//���ٷ�������//���㰲װ��Ա����֪����Ħ��״̬	
	}
}
/*******************************************************************************
* �� �� ��         : q2x1278_init
* ��������		   : 433��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void q2x1278_init()	   //
{
	GPIO_InitTypeDef GPIO_InitStructure;	  //����һ���ṹ�������������ʼ��GPIO

	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=q2x1278_M0|q2x1278_M1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	;	//�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
				
}


/*******************************************************************************
* �� �� ��         : q2x1278_rec_cofig
* ��������		   : 433������ȡ
* ��    ��         : ��
* ��    ��         : 0��������1��������ȫ�ֺ�����433����
*******************************************************************************/
u8 q2x1278_rec_cofig()	   //
{u16 j=0;
u8 read_cofig_command[3]= {0xc1,0xc1,0xc1};

	GPIO_SetBits(GPIOE,q2x1278_M0);	//��M0���1
	GPIO_SetBits(GPIOE,q2x1278_M1);	//��M1���1,��������״̬
	rx2_pointer=0;
	//Tm618_Smg_clear();//�������
	while(1)//�ȴ���������
	{

		u8 i;
		u8 k;
		u8 w[2]={0xc0,0x00};
		u8 n=0;
		USART2_sendstring(read_cofig_command);
			Delay_MS(500);
		API_WatchDog_Feed();
	//Tm618_Smg_Line(n);//�˶�ֻ�����������ʾ����---
		n++;
	if(n>4)
		n=0;
	
		if(rx2_pointer>5)
		{
			
			


			i=strsearch(w,Uart2_RecData_buf,rx2_pointer);

		
 			if(i>0)
			{
				for(k=0;k<6;k++)
				{
					q2x1278_config_buf[k]=Uart2_RecData_buf[i+k-1];
					qwl_sendchar(NO[Uart2_RecData_buf[k]>>4]);
				qwl_sendchar(NO[Uart2_RecData_buf[k]&0x0f]);
				}
				
				break;
			}
		}
 
		j++;
		
		if(j>5)
		{	GPIO_ResetBits(GPIOE,q2x1278_M0);	//��M0���0
			GPIO_ResetBits(GPIOE,q2x1278_M1);	//��M1���0,���뷢��״̬
			return 1;
		}
		
	}
	
	GPIO_ResetBits(GPIOE,q2x1278_M0);	//��M0���0
	GPIO_ResetBits(GPIOE,q2x1278_M1);	//��M1���0,���뷢��״̬
return 0;	
}
/*******************************************************************************
* �� �� ��         : q2x1278_set()
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : 0��������1��������ȫ�ֺ�����433����
*******************************************************************************/
u8 q2x1278_set()	   //
{u16 j=0;
		u8 k;
u8 i;
	u8 rec_no=0;
	GPIO_SetBits(GPIOE,q2x1278_M0);	//��M0���1
	GPIO_SetBits(GPIOE,q2x1278_M1);	//��M1���1,��������״̬
	rx2_pointer=0;
	Tm618_Smg_clear();//�������

	while(1)//�ȴ���������
	{
		u8 n=0;
		

		for(k=0;k<6;k++)
		{USART2_sendchar(q2x1278_config_buf[k]);
//			qwl_sendchar(NO[q2x1278_config_buf[k]>>4]);
//			qwl_sendchar(NO[q2x1278_config_buf[k]&0x0f]);
		}
		
			Delay_MS(500);
		API_WatchDog_Feed();
	Tm618_Smg_Line(n);//�˶�ֻ�����������ʾ����---
		n++;
	if(n>2)
		n=0;
	
		if(rx2_pointer>5)
		{
						
			i=strsearch("OK",Uart2_RecData_buf,rx2_pointer);

 			if(i>0)
			{
				qwl_sendstring("���÷���OK");
				break;
			}
		}
 
		j++;
		
		if(j>5)
		{GPIO_ResetBits(GPIOE,q2x1278_M0);	//��M0���0
	GPIO_ResetBits(GPIOE,q2x1278_M1);	//��M1���0,���뷢��״̬
			
			rec_no=1;
			break;
		}
		
	}
	
	amy_config=0;
	amy_config1=0;
	
	i=q2x1278_rec_cofig();//433ģ���飬��ȡ���ò�������ʾ
	if(i==0)
		{

		qwl_sendstring("433����");
		

			TM1618_DISPLAY_NO[0]=0x00;
			TM1618_DISPLAY_NO[2]=(q2x1278_config_buf[2]&0x0f);//�������ʾ��ֵ
			
			TM1618_DISPLAY_NO[1]=(q2x1278_config_buf[2]>>4);//�������ʾ��ֵ
			
			amy_config=amy_config|0x01;//���ش��ڷ�
			
			if(q2x1278_config_buf[2]==0xff)//
			amy_config=amy_config|0x02;//�ն˻�������	
					amy_config=amy_config|0x10;//Ĭ���������
		}
	else
		{
			qwl_sendstring("433������");
			amy_config=amy_config&0xfe;//���ش��ڷ�
			amy_config=amy_config&0xfc;//�ն˻�������
		}	
	GPIO_ResetBits(GPIOE,q2x1278_M0);	//��M0���0
	GPIO_ResetBits(GPIOE,q2x1278_M1);	//��M1���0,���뷢��״̬
		

	
return rec_no;	
}

/*******************************************************************************
* �� �� ��         : q2x1278_check_send()
* ��������		   : �ն˷��͵������Ƿ�ɹ������ж�ȡ
* ��    ��         : ��
* ��    ��         : 
*******************************************************************************/
u8 q2x1278_check_send()
{u8 i;
 u8 j=3;

	if(rx2_pointer>5)
		{

			i=strsearch("SEND_OK",Uart2_RecData_buf,rx2_pointer);
 			if(i>0)
			{
				j=0;
				rx2_pointer=0;
			}
			
			i=strsearch("ERROR01",Uart2_RecData_buf,rx2_pointer);
 			if(i>0)
			{
				j=1;//Ƶ����ͬ
				rx2_pointer=0;
			}
			
			i=strsearch("ERROR02",Uart2_RecData_buf,rx2_pointer);
			
 			if(i>0)
			{
				j=2;//ID��ͬ
				rx2_pointer=0;
			}			
		}
 	

return j;
}
/*******************************************************************************
* �� �� ��         : q2x1278_plc_send()
* ��������		   : �ն�������������
* ��    ��         : ��
* ��    ��         : 
*******************************************************************************/
void q2x1278_plc_send()
{
	u32 flash_adr=0x0803e800;//flash�洢��ַ
	u8 i;
	u8 send_buf[28]={0};
	u8 *read_flash;
			read_flash=(u8*)(flash_adr);
			send_buf[0]=0xff;//��ͷ
			send_buf[1]=26;			//�����Ȳ�����ͷ��β�������ݳ��ȿ�ʼ��
			send_buf[2]=plc_state;//����״̬���
			
		send_buf[3]=(q2x1278_config_buf[1]>>4)+0x30;//ģ���ַ��λ��ת��ASCII����
		send_buf[4]=(q2x1278_config_buf[1]&0x0f)+0x30;//ģ���ַ��λ��ת��ASCII����
	
		send_buf[5]=(q2x1278_config_buf[2]>>4)+0x30;//ģ���ַ��λ��ת��ASCII����
		send_buf[6]=(q2x1278_config_buf[2]&0x0f)+0x30;//ģ���ַ��λ��ת��ASCII����
	
		send_buf[7]=(q2x1278_config_buf[4]>>4)+0x30;//Ƶ�ε�ַ��ת��ASCII����
		send_buf[8]=(q2x1278_config_buf[4]&0x0f)+0x30;//Ƶ�ε�ַ��ת��ASCII����
			
	
			for(i=0;i<16;i++)
				{send_buf[i+9]=*read_flash;
				 read_flash++;
				}				
		send_buf[25]=(amy_config>>4)+0x30;//״̬��ת��ASCII����
		send_buf[26]=(amy_config&0x0f)+0x30;//״̬��ת��ASCII����
		send_buf[27]=0xfe;//��β����
				
				

				USART2_sendchar(0x00);//����ģ���ַ
				USART2_sendchar(0xff);//
				USART2_sendchar(q2x1278_config_buf[4]);//Ƶ��
				for(i=0;i<28;i++)//���͸���������
				USART2_sendchar(send_buf[i]);
				
				qwl_sendstring("�ն�ID");
				for(i=9;i<27;i++)//���͸���������
				qwl_sendchar(send_buf[i]);
		
		
}

/*******************************************************************************
* �� �� ��         :amy_config_send_1()
* ��������		   	: �ն����÷���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 amy_config_send_1(u8 addr)
{
		u8 send_buf[13]={0};
		u16 i;
		u8 j;
if(amy_config_rec_flag==1)
{amy_config_rec_flag=0;
	addr=addr+1;
		i=addr*21-1;
	if(i>0)
	{qwl_sendstring("�ҵ��ն˵�ַ");
	}
	if(i==0)
	{qwl_sendstring("û�ҵ��ն˵�ַ");
		amystate_send(0x06);
		return 1;
	}
	j=(i/21);
	
	config_recwait_buf[j]=0x01;
amystate_send_flag[j]=1;//��Ħ��״̬���ͱ�־
			send_buf[0]=amy_record_buf[i-19];
		send_buf[1]=amy_record_buf[i-18];
		send_buf[2]=amy_record_buf[i-17];
					send_buf[3]=0xff;//��ͷ
				send_buf[4]=8 ;//����
		send_buf[5]=0xb0;// �������ͣ�����ʱ�䣬ģʽ
		
		for(j=0;j<3;j++)
		{send_buf[6+j*2]=((plc_cofig_buf_1[j]>>4)+0x30);
		send_buf[7+j*2]=((plc_cofig_buf_1[j]&0x0f)+0x30);
		}
		send_buf[12]=0xfe;//��β֡
		for(i=0;i<13;i++)//���͸���������
		USART2_sendchar(send_buf[i]);
		
		qwl_sendstring("������������888");
			for(i=0;i<13;i++)//���͸���������
		{qwl_sendchar(NO[send_buf[i]>>4]);
		qwl_sendchar(NO[send_buf[i]&0x0f]);
		}
		config_recwait_time=0;
}
return 0;
}
/*******************************************************************************
* �� �� ��         :amy_config_send()
* ��������		   	: �ն����÷���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 amy_config_send()
{
		u8 send_buf[13]={0};
		u16 i;
		u8 j;
if(amy_config_rec_flag==1)
{amy_config_rec_flag=0;
		i=strsearch_qwl(plc_cofig_buf,amy_record_buf,5250,16);
	if(i>0)
	{qwl_sendstring("�ҵ��ն˵�ַ");
	}
	if(i==0)
	{qwl_sendstring("û�ҵ��ն˵�ַ");
		amystate_send(0x06);
		return 1;
	}
	j=(i/21);
	
	config_recwait_buf[j]=0x01;
amystate_send_flag[j]=1;//��Ħ��״̬���ͱ�־
			send_buf[0]=amy_record_buf[i-19];
		send_buf[1]=amy_record_buf[i-18];
		send_buf[2]=amy_record_buf[i-17];
					send_buf[3]=0xff;//��ͷ
				send_buf[4]=8 ;//����
		send_buf[5]=0xb0;// �������ͣ�����ʱ�䣬ģʽ
		
		for(j=0;j<3;j++)
		{send_buf[6+j*2]=((plc_cofig_buf_1[j]>>4)+0x30);
		send_buf[7+j*2]=((plc_cofig_buf_1[j]&0x0f)+0x30);
		}
		send_buf[12]=0xfe;//��β֡
		for(i=0;i<13;i++)//���͸���������
		USART2_sendchar(send_buf[i]);
		
		qwl_sendstring("������������888");
			for(i=0;i<13;i++)//���͸���������
		qwl_sendchar(send_buf[i]);
		config_recwait_time=0;
}
return 0;
}
/*******************************************************************************
* �� �� ��         :amy_config_please()
* ��������		   	: ��Ħ����������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 amy_config_please(u8 *addr)
{

	u8 i;
	u8 send_buf[13];
	
	config_recwait_buf[addr[1]]=0x01;

			send_buf[0]=addr[0];
		send_buf[1]=addr[1];
		send_buf[2]=addr[2];
					send_buf[3]=0xff;//��ͷ
				send_buf[4]=2 ;//����
		send_buf[5]=0xb1;// �������ͣ�����ʱ�䣬ģʽ
		
		
		send_buf[6]=0xfe;//��β֡
		for(i=0;i<7;i++)//���͸���������
		USART2_sendchar(send_buf[i]);
		config_recwait_time=0;
		qwl_sendstring("����״̬");
//			for(i=0;i<7;i++)//���͸���������
//		{qwl_sendchar(NO[send_buf[i]>>4]);
//			qwl_sendchar(NO[send_buf[i]&0x0f]);
//		}
		

return 0;
}
/*******************************************************************************
* �� �� ��         :u8 amy_amy_please(u8 *send_buf,u8 len,u8 command);
* ��������		   	: ���ػ�����ͨ��rola
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 amy_amy_please(u8 *send_buf,u8 len,u8 command,u8 *addr)
{

	u8 i;
	u8 n=0;
	u8 checksums=0;//�ۼӺ�
u8 send_buf1[258];
			send_buf1[n]=addr[0];
				n++;
		send_buf1[n]=addr[1];
				n++;
		send_buf1[n]=addr[2];
				n++;
		send_buf1[n]=0xff;//��ͷ
	n++;
		send_buf1[n]=(len+4);//����
	n++;
		send_buf1[n]=command;// �������ͣ�����ʱ�䣬ģʽ
	n++;
	for(i=0;i<len;i++,n++)
	{
		send_buf1[n]=send_buf[i];
		checksums=checksums+send_buf[i];
	}
	send_buf1[n]=checksums;//У����
	n++;
		send_buf1[n]=0xfe;//��β֡
	n++;
		for(i=0;i<n;i++)//���͸���������
		USART2_sendchar(send_buf1[i]);

//			for(i=0;i<7;i++)//���͸���������
//		{qwl_sendchar(NO[send_buf[i]>>4]);
//			qwl_sendchar(NO[send_buf[i]&0x0f]);
//		}
		

return 0;
}
/*******************************************************************************
* �� �� ��         :amy_ack_send()
* ��������		   	: �ն����÷���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
u8 amy_ack_send()
{
u8 i;
u8 send_buf[10];
while(wg_ack_times>0)//������������Ҫ��������������봦��
{
	qwl_sendstring("���з�������");
//	qwl_sendchar(NO[(wg_ack_times>>4)]);
//	qwl_sendchar(NO[(wg_ack_times&0x0f)]);
//	qwl_sendchar(NO[(wg_deal_pointer>>4)]);
//	qwl_sendchar(NO[(wg_deal_pointer&0x0f)]);
	for(i=0;i<10;i++)
	{send_buf[i]=wg_ack_buf[wg_deal_pointer];

	wg_deal_pointer++;
	if(wg_deal_pointer>=200)
				wg_deal_pointer=0;
	}
	for(i=0;i<10;i++)
	{
			USART2_sendchar(send_buf[i]);
		
	}
		for(i=0;i<3;i++)
	{	
		qwl_sendchar(NO[send_buf[i]>>4]);
		qwl_sendchar(NO[send_buf[i]&0x0f]);
	}
			for(i=3;i<10;i++)
	{	
		qwl_sendchar(send_buf[i]);
	}
	Delay_MS(1);

	wg_ack_times--;
}
return 0;
}





