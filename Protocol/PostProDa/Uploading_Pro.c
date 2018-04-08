#include "config.h"
extern u8 amy_check_pointer;
//������������������������������������������������������������������������������
// ��������  : Uploading_Send_50
// ��������  : ��ȡ��ַ
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void Uploading_Send_50(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{	
	PostSendHead(uf);
	PostSend_Word(uf,ModAddress);     // ��Э���ַ		
	PostSendDataLen(uf);
	PostSendPostChksum(uf);
	PostSendTail(uf);
}
//������������������������������������������������������������������������������
// ��������  : Uploading_Send_F8
// ��������  : ��ȡ��ַ
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void Uploading_Send_F8(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{	uchar  Cmd = DaPtr[4];
	
			Run_User_Program_Falg = true;
	switch (uf)
	{
		case USART_FUNCTION_DEBUG:
		{
			#ifdef STM32_BOOT
			iap_data.copyflag = 1;
			iap_data.Address = ModAddress;
            API_goto_iap();
			#else
			PostSendHead(uf);
			PostSendByteASC(uf,Cmd);		
			PostSendDataLen(uf);
			PostSendPostChksum(uf);
			PostSendTail(uf);
			#endif
		}break;
		case USART_FUNCTION_WCDMA:
		{u16 i;
			software_version[0]=DaPtr[23];
				software_version[1]=DaPtr[24];
			process_F8(0x00);
			if(exchange_platform.Platform_Choice ==PLATFORM_CHOICE_PUBLIC)
			{
				qwl_sendstring1("�л�������ƽ̨");
							for(i=0;i<22;i++)
			{ 
				update_ip[i]=DaPtr[25+i];
				if(update_ip[i]!=0)
				qwl_sendchar(update_ip[i]);
			}

			qwl_sendchar(0x0d);
			qwl_sendchar(0x0a);
				exchange_platform.UPDATE_link_overtime =0;
				exchange_platform.UPDATE_link_times =0;
			exchange_platform.Platform_Choice =PLATFORM_CHOICE_UPDATE;//�л�������ƽ̨
			API_nblot_init_0();//��ʼ��ģ�飬��λ��ʼ
			net_data_send_init();//������ƽ̨����Ҫ����������Ϣ�����г�ʼ��
			Delay_MS(1000);
			}
			
			
		}break;
		default:break;
	}

}
/*******************************************************************************
* �� �� ��         : OUT_AMY_addr_please_set(u8 *ch)
* ��������		   : ���ڰ�Ħ�ε�ַ����
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void OUT_AMY_addr_please_set(u8 *ch)
{

	u8 send_buf[13];
	u8 addr[3];
	u16 len=0;
	addr[0]=0;
	addr[1]=ch[8];
	addr[2]=q2x1278_config_buf[4];
			send_buf[len]=0;
		len++;
		send_buf[len]=ch[8];//Ŀ�ĵ�ַ��ƽ̨����
		len++;
		send_buf[len]=q2x1278_config_buf[4];
		len++;

													amy_amy_please(send_buf,len,0xc1,addr);

}
/*******************************************************************************
* �� �� ��         : AMY_addr_please_set(u8 *ch)
* ��������		   : ��Ħ�ε�ַ�����趨
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void AMY_addr_please_set(u8 *ch)
{
							u16 j;
						u16 i;

						u8 addr[3];
	switch (ch[0])
	{
		case 1://��ѯ�ն�״̬
		{									
											
											j=ch[1]*21;
												if(j>0)
												{qwl_sendstring("�ҵ��ն˵�ַ");
													j=(j/21);
												config_recwait_buf[j]=0x01;
													addr[0]=0;
													addr[1]=j;
													addr[2]=q2x1278_config_buf[4];
													amy_config_please(&addr[0]);
													amystate_send_flag[j]=1;//��Ħ��״̬���ͱ�־
												config_recwait_time=0;
												}
												if(j==0)
												{qwl_sendstring("û�ҵ��ն˵�ַ");

													amystate_send(0x06);
												}
		}
			break;
		case 2:
		{
			qwl_sendstring("��Ħ��ʱ���趨");

								for(i=0;i<3;i++)
								plc_cofig_buf_1[i]=ch[i+2];
								amy_config_rec_flag=1;

								amy_config_send_1(ch[1]);
		}
			break;
		case 3:
		{
			qwl_sendstring("����-��Ħ��״̬");
			wg_state_flag=1;
			
			
		}
			break;
		case 4:
		{
			qwl_sendstring("����-��Ħ������");
							amy_cotrol_time=(u16)(ch[2]<<8)+ch[3];
							if(amy_cotrol_time>0)//��������ʱ���������
							{
								if(amy_cotrol_flag==1)
							getwayoff_send_flag=getwayoff_send_flag|0x02;

								Shared_Module_switch(0x00);//0���� 1�ر�
								amy_cotrol_flag=1;
							amy_cotrol_time_flag=0;//��Ħ�μ�ʱ����
							armchair_switch=0;//��Ħ�γ�ʼ������ֹͣ��������
								
							qwl_sendstring("��Ħ�ο�ʼ����88");
							LED=0;
							}
						wg_state_flag=2;	
		}break;
		case 5:
		{
			qwl_sendstring("��ȡ�ն��б�");
							poll_zd.poll_max_time =(u16)(ch[1]<<8)+ch[2];//��ѯʱ��
							poll_zd.list_please_flag =1;
			for(i=0;i<13;i++)
			{
				poll_zd.zd_list[i]=ch[3+i];//��Ħ�������б�
				amy_check_pointer=0;
				qwl_sendchar(NO[poll_zd.zd_list[i]>>4]);
				qwl_sendchar(NO[poll_zd.zd_list[i]&0x0f]);
			}
			if(poll_zd.poll_max_time>0xfff0)
				poll_zd.poll_max_time=0xfff0;
			poll_zd.poll_time =poll_zd.poll_max_time;
		}
			break;
		case 6://��ѯSIM����
		{
			qwl_sendstring("��ȡ����");
			SIM_GET_FLAG=1;
		}break;
		
		default:
		{
		}
			break;
	}
}
/*******************************************************************************
* �� �� ��         : process_fb
* ��������		   : ��Ħ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void process_fb(u8 *n)
{
	u8 write_flash_fb=0;
	u16 j;
								qwl_sendstring("��������");
//								for(j=3;j<m;j++)
//								qwl_sendchar(n[j]);//������
							GATEWAY_peizhi_pack();
							net_send(post_buf,post_len);
								
								if(n[12]&0x01)//��ʾID��Ч
								{qwl_sendstring("����ID");
											for(j=0;j<16;j++)
												{AmyID[j]=n[j+14];
												qwl_sendchar(AmyID[j]);													
												}
									write_flash_fb=1;
								}
										if(n[12]&0x02)//����汾
								{qwl_sendstring("��������汾");
											for(j=0;j<2;j++)
												{software_version[j]=n[j+31];
												qwl_sendchar(NO[software_version[j]]);													
												}		
								write_flash_fb=1;												
								}
								
								if(n[12]&0x04)//��ʾӲ����Ч
								{qwl_sendstring("����Ӳ���汾");
											for(j=0;j<2;j++)
												{Hardware_version[j]=n[j+34];
												qwl_sendchar(NO[Hardware_version[j]]);													
												}	
									write_flash_fb=1;
								}
								if(n[12]&0x40)//���ð�Ħ������
								{qwl_sendstring("���ð�Ħ������");
									qwl_sendchar(NO[n[43]>>4]);
										qwl_sendchar(NO[n[43]&0x0f]);
								amy_max_total=n[43];
									zd_sl_flag=0xcc;
					
								}
								
								if(n[12]&0x80)//�޸�����IP
								{qwl_sendstring("��������IP");
											for(j=0;j<21;j++)
												{Upgrade_ip[j]=n[j+45];
													if(Upgrade_ip[j])
												qwl_sendchar(Upgrade_ip[j]);													
												}	
												
												Upgrade_ip[j]=0;
									write_flash_fb=1;
								}
									if(n[11]&0x01)//��ʾ����TCP
								{qwl_sendstring("����TCP");
											for(j=0;j<21;j++)
												{light_TCP[j]=n[j+67];
													if(light_TCP[j])
												qwl_sendchar(light_TCP[j]);													
												}	
												light_TCP[j]=0;
									write_flash_fb=1;
								}
								if(n[11]&0x02)//��ʾUDP��Ч
								{qwl_sendstring("����UDP");
											for(j=0;j<21;j++)
												{light_UDP[j]=n[j+89];
													if(light_UDP[j])
												qwl_sendchar(light_UDP[j]);													
												}	
												light_UDP[j]=0;
									write_flash_fb=1;
								}
								
								if(n[11]&0x10)//��ʾUDP��Ч
								{qwl_sendstring("����SIM����");
											for(j=0;j<20;j++)
												{SIM_ID[j]=n[j+115];
													if(SIM_ID[j])
												qwl_sendchar(SIM_ID[j]);													
												}	
									write_flash_fb=1;
								}
								
								if(n[11]&0x04)//��ʾ��Ӫ����Ч
								{

									qwl_sendstring("������Ӫ��");
									carrieroperator=n[111];
									qwl_sendchar(NO[carrieroperator]);													

									write_flash_fb=1;
								}
									if(n[11]&0x08)//��ʾģ��������Ч
								{qwl_sendstring("����ģ������");
									module_name=n[113];
									qwl_sendchar(NO[module_name>>4]);
									qwl_sendchar(NO[module_name&0x0f]);											

									write_flash_fb=1;
								}
								
								if(write_flash_fb)
								{
									Write_light_idip();	
								}
								
									if(n[12]&0x38)//����433��ַ
								{qwl_sendstring("����433��ַ");
										
			q2x1278_config_buf[1]=0x00;//��ַ��λ��ʼ��Ĭ��Ϊ00	
			q2x1278_config_buf[0]=0xc0;//433��������
			q2x1278_config_buf[3]=0x1A;//������
			q2x1278_config_buf[5]=0xc0;//���㷢��
			
									if(n[12]&0x20)//����������
									{qwl_sendstring("=����");
										q2x1278_config_buf[2]=0xff;
									}
									else
									{
										q2x1278_config_buf[2]=n[37];
										qwl_sendchar(NO[n[37]>>4]);
										qwl_sendchar(NO[n[37]&0x0f]);
										qwl_sendstring("=�ն�");
									}
									
										if(n[12]&0x10)//����433��ַ
								{qwl_sendstring("����433Ƶ��");
									qwl_sendchar(NO[n[39]>>4]);
										qwl_sendchar(NO[n[39]&0x0f]);
								q2x1278_config_buf[4]=n[39];
					
								}
									sx1278_fb_flag=1;
								}

}
/*******************************************************************************
* �� �� ��         : process_00
* ��������		   : ������ȷ
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void process_00(u8 *n)
{

									if(n[5]==0x06)
									{
											getwayheart_sendok_flag=1;

											if(n[6]==1)//����洢У���벻��ȷ����Ҫ�����ϴ�
											{
												getwaylist_send_flag=1;//�б�Ҫ�������ϴ���־
												getwaylist_send_times=0;
											}
											GATEWAY_heart_max_flag=(u16)(n[7]<<8)+n[8];//��������ʱ��
											qwl_sendstring("��������ʱ��888");
											qwl_sendchar(NO[(GATEWAY_heart_max_flag>>12)&0x0f]);
											qwl_sendchar(NO[(GATEWAY_heart_max_flag>>8)&0x0f]);
											qwl_sendchar(NO[(GATEWAY_heart_max_flag>>4)&0x0f]);
											qwl_sendchar(NO[GATEWAY_heart_max_flag&0x0f]);
											//amystate_send(0x00);//������ȷ
											net_error_flag=0;//���ܵ����ݣ��峬ʱ��������ֹģ������
									}
								else if(getwaylist_sendok_flag==2)
								{getwaylist_sendok_flag=1;
									getwayoff_sendok_flag=1;
									//amystate_send(0x00);//������ȷ
								}
								else //if(getwayoff_sendok_flag==2)
								{getwayoff_sendok_flag=1;
									//amystate_send(0x00);//������ȷ
								}
}
/*******************************************************************************
* �� �� ��         : void process_12()
* ��������		   : ����12�����
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void process_12()
{  

		

	u8 i;

	u8 post_buf1[200];
	u16 post_buf1_len=0;
char SoftWareVer[7];
	char HardWareVer[7];
	char Serial[30] = {0};
	char Content[20] = {0};
/*��˾����*/
u8 CompName[20] = COMPANY_NAME;

	
	sprintf(SoftWareVer,"V%4.2f",SOFTWARE_VER);
	sprintf(HardWareVer,"V%4.2f",HARDWARE_VER);
   
		
			sprintf(Content,"%s %s",__DATE__,__TIME__);
	
				post_buf1[post_buf1_len]=0x00;
				post_buf1_len++;
				post_buf1[post_buf1_len]=ModAddress;
				post_buf1_len++;
				for(i=0;i<6;i++)
				{post_buf1[post_buf1_len]=SoftWareVer[i];
					post_buf1_len++;
				}
				for(i=0;i<6;i++)
				{post_buf1[post_buf1_len]=HardWareVer[i];
					post_buf1_len++;
				}
				for(i=0;i<20;i++)
				{post_buf1[post_buf1_len]=CompName[i];
					post_buf1_len++;
				}
				for(i=0;i<16;i++)
				{post_buf1[post_buf1_len]=AmyID[i];
					post_buf1_len++;
				}
				for(i=0;i<4;i++)
				{post_buf1[post_buf1_len]=0x0;
					post_buf1_len++;
				}
				for(i=0;i<24;i++)
				{post_buf1[post_buf1_len]=Serial[i];
					post_buf1_len++;
				}
				for(i=0;i<20;i++)
				{post_buf1[post_buf1_len]=Content[i];
					post_buf1_len++;
				}
				

	
			post_group(post_buf1,post_buf1_len,0x00);
   net_send(post_buf,post_len);
} 
//������������������������������������������������������������������������������
// ��������  : Uploading_Pro
// ��������  : �ϱ���Э�鴦��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Uploading_Pro(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
	uchar  Cmd = DaPtr[4];
	switch(Cmd)
	{	
		case 0x50://��ȡ��ַ
			Run_User_Program_Falg = true;
			Uploading_Send_50(uf,DaPtr,len);
			break;
		case 0xF8:
		{	
			if(Net_Data_Send.upload_handshake.REC_state == WAIT_REC)//�ж��״����֣��ȴ���������
			{
				exchange_platform.UPDATE_link_times=0;
				Net_Data_Send.upload_handshake.REC_state=REC_OK;//
			}			
		Uploading_Send_F8(uf,DaPtr,len);
			
		}
			break;
		case 0x1b://��Ħ��״̬��ѯ
		{
			u16 i;
			u16 j;
			u8 addr[3];
			u8 zd_id[16]={0};
			for(i=0;i<16;i++)
									{
										
										zd_id[i]=DaPtr[i+7];//����ʱ�䵱״̬������
									}
								if(CRC8_Table(zd_id, 16)!=DaPtr[23])
								{
									qwl_sendstring("����У�����");
									break;
								}
									
								qwl_sendstring("����״̬");
								

								for(i=0;i<16;i++)
									{
										
										plc_cofig_buf[i]=DaPtr[i+7];//����ʱ�䵱״̬������
									}
									

										j=strsearch_qwl(plc_cofig_buf,amy_record_buf,5250,16);
												if(j>0)
												{qwl_sendstring("�ҵ��ն˵�ַ");
													j=(j/21);
												config_recwait_buf[j]=0x01;
													addr[0]=0;
													addr[1]=j;
													addr[2]=q2x1278_config_buf[4];
													amy_config_please(&addr[0]);
													amystate_send_flag[j]=1;//��Ħ��״̬���ͱ�־
																									config_recwait_time=0;
												}
												if(j==0)
												{qwl_sendstring("û�ҵ��ն˵�ַ");

													amystate_send(0x06);
												}
		}
			break;
		case 0x1e://��Ħ��ʱ������
		{
			u16 i;
			//u16 j;
			//u8 addr[3];
			u8 zd_id[16]={0};
											for(i=0;i<16;i++)
									{
										
										zd_id[i]=DaPtr[i+7];//����ʱ�䵱״̬������
									}
								if(CRC8_Table(zd_id, 16)!=DaPtr[23])
								{
									qwl_sendstring("����У�����");
									break;
								}
								qwl_sendstring("��Ħ��ʱ���趨");
								for(i=0;i<16;i++)
								{
								plc_cofig_buf[i]=DaPtr[i+7];
								
								}

								for(i=0;i<3;i++)
								plc_cofig_buf_1[i]=DaPtr[i+24];
								amy_config_rec_flag=1;

								amy_config_send();
								//amystate_send(0x00);
		}
			break;
		case 0xfb://��Ħ������
		{
			process_fb(&DaPtr[1]);
		}	
		case 0x12://��Ħ������
		{
			process_12();
		}	
			break;
		case 0x24://��Ħ��ֱ�ӵ�ַ��ѯ����ַ����ʱ�䣬����ֱ������������ֱ�Ӳ�ѯ��Ħ��״�
		{
			u16 i;
			u8 zd_id[16]={0};
											for(i=0;i<16;i++)
									{
										
										zd_id[i]=DaPtr[i+7];//����ʱ�䵱״̬������
									}
								if(CRC8_Table(zd_id, 16)!=DaPtr[23])
								{
									qwl_sendstring("����У�����");
									break;
								}
								
								for(i=0;i<16;i++)
								{
									if(AmyID[i]!=DaPtr[i+7])
									{
										break;
									}
								}
								if(i!=16)
								{
									qwl_sendstring("������ID��ƥ��");
									OUT_AMY_addr_please_set(&DaPtr[7]);//����ID����
									break;
								}
								else
								AMY_addr_please_set(&DaPtr[24]);
		}
			break;
		
				case 0x25://��Ħ��ֱ�ӵ�ַ��ѯ����ַ����ʱ�䣬����ֱ������������ֱ�Ӳ�ѯ��Ħ��״�
		{
			u16 i;
			u8 zd_id[16]={0};
											for(i=0;i<16;i++)
									{
										
										zd_id[i]=DaPtr[i+7];//����ʱ�䵱״̬������
									}
								if(CRC8_Table(zd_id, 16)!=DaPtr[23])
								{
									qwl_sendstring("����У�����");
									break;
								}
								
								for(i=0;i<16;i++)
								{
									if(AmyID[i]!=DaPtr[i+7])
									{
										break;
									}
								}
								if(i!=16)
								{
									qwl_sendstring("������ID��ƥ��");
									break;
								}
								else
								AMY_addr_please_set(&DaPtr[24]);
		}
			break;
		case 0x00:
			process_00(&DaPtr[1]);
			break;
		case 0x01:
			qwl_sendstring("VER����");
			break;
		case 0x04:
			qwl_sendstring("CID2����");
			break;
		case 0x05:
			qwl_sendstring("�����ʽ����");
			break;
		case 0x06:
			qwl_sendstring("��Ч����");
			break;
		default:
            RTN= 0x05;//��Ч����
			PostSendDefault(uf);
            break;
	}
}

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������


