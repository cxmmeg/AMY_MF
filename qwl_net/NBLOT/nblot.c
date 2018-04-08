#include "config.h"


u8 nblot_CFUN[]="AT+CFUN=1";
u8 nblot_NSOCL[]="AT+NSOCL=0"; 
u8 nblot_CEREG[]="AT+CEREG=1";
u8 nblot_CGDCONT[]="AT+CGDCONT=1,\"IP\",\"CTNB\"";
u8 nblot_COPS[]="AT+COPS=1, 2,\"46011\"";
u8 nblot_CGATT[]="AT+CGATT? ";
u8 nblot_NSOST[]="AT+NSOST=0,";
u8 nblot_NSOCR[]="AT+NSOCR= DGRAM,17,56852,1";
u8 nblot_NSORF[]="AT+NSORF=0,";
u8 nblot_NCCID[]="AT+NCCID";

u8 nblot_NBAND[]="AT+NBAND?";
u8 nblot_NBAND1[]="AT+NBAND=5";
u8 nblot_NCONFIG[]="AT+NCONFIG?";

u8 nblot_AUTOCONNECT[]="AT+NCONFIG=AUTOCONNECT,TRUE";

u8 nblot_SCRAMBL_true[]="AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE";
u8 nblot_SCRAMBL_false[]="AT+NCONFIG=CR_0354_0338_SCRAMBLING,FALSE";
u8 nblot_AVOID_true[]="AT+NCONFIG=CR_0859_SI_AVOID,TRUE";
u8 nblot_AVOID_false[]="AT+NCONFIG=CR_0859_SI_AVOID,FALSE";
u8 nblot_rec_flag=0;
u8 nblot_read_flag=0;//nb������Ҫ���ܵ����ݣ���Ҫ������


u8 nblot_read_flag1=0;
//u8 nblot_read_flag1_delay=0;

//u8 nblot_rec_buf[500];
//u16 nblot_rec_times=0;

u8 nblot_RESET_flag=0;


u8 nblot_overtime_flag=0;




u8 light_UDP[22]={"39.108.148.248,26002"};;

u8 NBIOT_SET_FALG=0;

u8 NB_SET_FLAG=0;
/*********************************************************************
 ** ��������: strsearch_hq ()
 ** ��������: ��ָ���������������ҵ���ͬ������
 ** ��ڲ���: ptr2Ҫ���ҵ�����, ptr1��ǰ����
 ** ���ڲ���: 0-û�� �ҵ�   >1 ���ҵ�
 *********************************************************************/
/*u16 strsearch_hq(u8 *ptr2,u8 *ptr1_at)//���ַ���*ptr2��*ptr1�е�λ��
//����������������ַ���*ptr2�Ƿ���ȫ������*ptr1��
//����:  0  û���ҵ�
//65535 �ӵ�N���ַ���ʼ��ͬ
{
u16 i,j,k;
        
      
	if(nblot_rec_times==0) return(0);
	
	for(i=0,j=0;i<nblot_rec_times;i++)
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
	return(0);
}
*/
//*******************************************************
//TP����
//******************************************************
void nblot_TP_GPRS(u8 *p,u8 *q)
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
}

//��������
//*********************************************************
/*******************************************************************************
* �� �� ��         :qwl_net_sendstring1(u8 *p,u16 len)
* ��������		   : ����3��������λ
* ��    ��         : ��
* ��    ��         : ��
********************************************************************************/
	
u8 nblot_send(u8 *p,u16 len)
{	u16 j=0;


clear_SystemBuf();

if(exchange_platform.Platform_Choice == PLATFORM_CHOICE_PUBLIC)
nblot_TP_GPRS(nblot_NSOST,light_UDP);
else
nblot_TP_GPRS(nblot_NSOST,update_ip);//��������IP

	qwl_net_sendchar(0x2c);
	qwl_sendchar(0x2c);

///	sprintf(i, "%d", (u8)len);
//	for(k=0;k<strlen(i);k++)
	j=len;
	if(len>99)
	{	j=len/100;
		qwl_net_sendchar(NO[j]);
		qwl_sendchar(NO[j]);
		j=len%100;
	}
	
	qwl_net_sendchar(NO[j/10]);
	qwl_sendchar(NO[j/10]);
	qwl_net_sendchar(NO[j%10]);
	qwl_sendchar(NO[j%10]);
	

	
	qwl_net_sendchar(0x2c);
	qwl_sendchar(0x2c);

for(j=0;j<len;j++)
  	{
  		qwl_sendchar(NO[p[j]>>4]);//������
			qwl_sendchar(NO[p[j]&0x0f]);//������
			//qwl_sendchar(p[j]);
			qwl_net_sendchar(NO[p[j]>>4]);
			qwl_net_sendchar(NO[p[j]&0x0f]);
	}
	
	
	qwl_sendchar(0x0D);
  	qwl_sendchar(0x0A);
	qwl_net_sendchar(0x0D);
  	qwl_net_sendchar(0x0A);
return 1;
} 



/*******************************************************************************
* �� �� ��         : cdma_init()
* ��������		   : cdma��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void nblot_init()
{
	
		switch (net.setp_setp)
	{

			case 0:
			{qwl_sendstring("��ѯƵ��");
				clear_SystemBuf();
				qwl_net_sendstring(nblot_NBAND);//��λ
				net.delay=0;
				net.setp_setp=1;
			}break;
			case 1:
			{
				u8 addr=0;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							qwl_sendstring("��ѯƵ�γ�ʱ");
									net.setp_setp=0;
								net.delay=0;
							net.delay_times++;
							if(net.delay_times>5)
							{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}	
							addr=strsearch_h("+NBAND:",SystemBuf);
							if(addr>0)
								{ if(SystemBuf[addr]=='5')
									{qwl_sendstring("Ƶ����ȷ");
										NB_SET_FLAG=0;
									}
									else if(SystemBuf[addr]=='8')
									{qwl_sendstring("Ƶ����ȷ");
										NB_SET_FLAG=0;
									}
									else
									{
										qwl_sendstring("Ƶ�δ���");
										NB_SET_FLAG=1;
									}
									net.delay_times=0;
								net.setp_setp=2;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;
								}
							
						
			}break;
			case 2:
			{qwl_sendstring("��ѯ����");
				clear_SystemBuf();
				qwl_net_sendstring(nblot_NCONFIG);//��λ
				net.delay=0;
				net.setp_setp=3;
			}break;
			case 3:
			{
				u8 addr=0;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							qwl_sendstring("��ѯ���ó�ʱ");
								net.setp_setp=2;
								net.delay=0;
							net.delay_times++;
							if(net.delay_times>5)
							{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}	
	
						}
						addr=strsearch_h("AVOID,FALSE",SystemBuf);
						if(addr==0)
						addr=strsearch_h("AVOID,TRUE",SystemBuf);
						if(addr)
						{
							NB_SET_FLAG=0;
							addr=strsearch_h("AUTOCONNECT,FALSE",SystemBuf);
							if(addr>0)
								{ NB_SET_FLAG=NB_SET_FLAG|0x02;
									qwl_sendstring("�Զ����Ӵ���");
								}
								
							addr=strsearch_h("SCRAMBLING,FALSE",SystemBuf);
							if(addr>0)
								{ NB_SET_FLAG=NB_SET_FLAG|0x04;
									qwl_sendstring("SCRAMB���Ӵ���");
								}
								
							addr=strsearch_h("AVOID,FALSE",SystemBuf);
							if(addr>0)
								{ NB_SET_FLAG=NB_SET_FLAG|0x08;
									qwl_sendstring("AVOID���Ӵ���");
								}
								qwl_sendstring("���������");
								qwl_sendchar(NO[NBIOT_SET_FALG>>4]);
								qwl_sendchar(NO[NBIOT_SET_FALG&0x0f]);
								if(NB_SET_FLAG&0x01)
								{
								net.delay_times=0;
								net.setp_setp=4;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;
								}
								else if(NB_SET_FLAG&0x02)
								{
								net.delay_times=0;
								net.setp_setp=6;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;
								}
								else if(1)
								{

										if(NBIOT_SET_FALG>3)//3�����Ӳ��ϣ��ͽ���ģʽ�л�
										{

									net.delay_times=0;
									net.setp_setp=8;//0;//net.setp_setp=8;//�����Զ�ʶ��ر�
									net.reconnect_setp=MODE_INIT;//2;//net_reconnection_flag=3;//����������������
									net.delay=0;											
										}
										else
										{
										NBIOT_SET_FALG++;
									net.delay_times=0;
									net.setp_setp=0;
									net.reconnect_setp=OPEN_GPRS;//����������������
									net.delay=0;		
										}
								}
							}

			}break;		
			
			
			case 4:
			{qwl_sendstring("���õ���Ƶ��");
				clear_SystemBuf();
				qwl_net_sendstring(nblot_NBAND1);//����Ƶ��Ϊ5
				net.delay=0;
				net.setp_setp=5;
			}break;
			case 5:
			{
				u8 addr=0;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							qwl_sendstring("����Ƶ�γ�ʱ");
								net.setp_setp=4;
								net.delay=0;
							net.delay_times++;
							if(net.delay_times>5)
							{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}
							
							addr=strsearch_h("OK",SystemBuf);
							if(addr>0)
								{ 
									qwl_sendstring("����Ƶ�γɹ�");
									if(NB_SET_FLAG&0x02)
									{
								net.delay_times=0;
								net.setp_setp=6;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;		
									}
									else if(NBIOT_SET_FALG>3)
									{
								net.delay_times=0;
								net.setp_setp=8;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;	
									}
									else if(1)
									{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//���������������ӣ�������Ƶ�α�������
								net.delay=0;	
									}
									
								}
		
			}break;

			case 6:
			{qwl_sendstring("�����Զ�����ģʽ");
				clear_SystemBuf();
				qwl_net_sendstring(nblot_AUTOCONNECT);//����Ƶ��Ϊ5
				net.delay=0;
				net.setp_setp=7;
			}break;
			case 7:
			{
				u8 addr=0;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							qwl_sendstring("�����Զ�������ʱ");
								net.setp_setp=6;
								net.delay=0;
							net.delay_times++;
							if(net.delay_times>5)
							{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}
							
							addr=strsearch_h("OK",SystemBuf);
							if(addr>0)
								{ 
									qwl_sendstring("�����Զ������ɹ�");
									if(NBIOT_SET_FALG>3)
									{
								net.delay_times=0;
								net.setp_setp=8;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;	
									}
									else if(1)
									{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;	
									}
									
								}
		
			}break;	

			case 8:
			{
				if((NB_SET_FLAG&0x0c)>0)
				qwl_sendstring("����SCRAMBL=true");
				else
				qwl_sendstring("����SCRAMBL=false");	
				clear_SystemBuf();
				if((NB_SET_FLAG&0x0c)>0)
				qwl_net_sendstring(nblot_SCRAMBL_true);//����Ϊtrue
				else
				qwl_net_sendstring(nblot_SCRAMBL_false);//����Ϊfalse
				net.delay=0;
				net.setp_setp=9;
			}break;
			case 9:
			{
				u8 addr=0;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							qwl_sendstring("����SCRAMBL��ʱ");
								net.setp_setp=8;
								net.delay=0;
							net.delay_times++;
							if(net.delay_times>5)
							{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}
							
							addr=strsearch_h("OK",SystemBuf);
							if(addr>0)
								{ 
									qwl_sendstring("����SCRAMBL�ɹ�");
								net.delay_times=0;
								net.setp_setp=10;
								net.reconnect_setp=MODE_INIT;//����������������
								net.delay=0;	
								}
		
			}break;	

					
				case 10:
			{
				if(NB_SET_FLAG&0x0c)
				qwl_sendstring("����AVOID=true");
				else
				qwl_sendstring("����AVOID=false");	
				clear_SystemBuf();
				if(NB_SET_FLAG&0x0c)
				qwl_net_sendstring(nblot_AVOID_true);//����Ϊtrue
				else
				qwl_net_sendstring(nblot_AVOID_false);//����Ϊfalse
				net.delay=0;
				net.setp_setp=11;
			}break;
			case 11:
			{
				u8 addr=0;
							if(net.delay>2)//�ȴ���ʱ������������
						{net.delay=0;
							qwl_sendstring("����AVOID��ʱ");
								net.setp_setp=10;
								net.delay=0;
							net.delay_times++;
							if(net.delay_times>5)
							{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
							}
						}
							
							addr=strsearch_h("OK",SystemBuf);
							if(addr>0)
								{ 
									NBIOT_SET_FALG=0;
									qwl_sendstring("����AVOID�ɹ�");
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������,��������Ҫ����
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
void nblot_reconnect()
{
	
		switch (net.setp_setp)
	{
		
		case 0:
		{
			clear_SystemBuf();			
			qwl_net_sendstring(nblot_NSOCR);
			net.setp_setp=1;
			net.delay=0;
			qwl_sendstring("������");
			nblot_RESET_flag++;
			if(nblot_RESET_flag>3)
		{
			net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_CHOICE;//����CDMA����
					net.delay=0;
			nblot_RESET_flag=0;
					qwl_sendstring("����NBLOT��λ");
	}
		}break;
		case 1:
		{
			if(net.delay>=6)//�ȴ���ʱ������������
			{net.delay=0;
				net.setp_setp=0;//�ȴ�ʧ��
				net.reconnect_setp=LINK_IP;
				
				
				qwl_sendstring("��������ʱ");
				net.delay_times++;
				if(net.delay_times>7)
				{
					net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=OPEN_GPRS;//����CDMA����
					net.delay=0;
					qwl_sendstring("����NBLOT����");
				}
			}
			

				if(strsearch_h("OK",SystemBuf)>0)
				{net.reconnect_setp=LINK_OK;//����ϣ���������
					net.setp_setp=0;
					net.delay_times=0;
					net.delay=0;
					deal_process=0;
					qwl_sendstring("�������ɹ�");
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
void nblot_open_GPRS()
{
	switch (net.setp_setp)
	{
		case 0:
		{
			clear_SystemBuf();			
			qwl_net_sendstring(nblot_NCCID);
			net.setp_setp=1;
			net.delay=0;
			qwl_sendstring("��ȡNB����");
		}break;
			case 1:
		{u8 i;
			u16 j;
						if(net.delay>3)//�ȴ���ʱ������������
			{net.delay=0;
				net.setp_setp=0;//�ȴ�ʧ��
				net.reconnect_setp=OPEN_GPRS;
				
				
				qwl_sendstring("������ʱ");
				net.delay_times++;
				if(net.delay_times>3)
				{
					net.setp_setp=2;
					net.delay_times=0;
					net.delay=0;
				}
			}
			

				j=strsearch_h("+NCCID:",SystemBuf);
				if(j>0)
				{
					
					qwl_sendstring("��NB���ųɹ�");
									for(i=0;i<20;i++)
									{SIM_ID[i]=SystemBuf[j+i];
										qwl_sendchar(SIM_ID[i]);
									}
					net.setp_setp=2;
					net.delay_times=0;
					net.delay=0;
				}
				
			
		}break;

				case 2:
		{
			clear_SystemBuf();			
			qwl_net_sendstring(nblot_CGATT);
			net.setp_setp=3;
			net.delay=0;
			qwl_sendstring("���Ų�ѯ");
		}break;
		case 3:
		{
			if(net.delay>6)//�ȴ���ʱ������������
			{net.delay=0;
				net.setp_setp=2;//�ȴ�ʧ��
				net.reconnect_setp=OPEN_GPRS;
				
				
				qwl_sendstring("���ų�ʱ");
				net.delay_times++;
				if(net.delay_times>7)
				{
					net.delay_times=0;
					net.setp_setp=0;
					net.reconnect_setp=MODE_CHOICE;//����CDMA����
					net.delay=0;
					qwl_sendstring("����NBLOT����");
				}
			}
			

				if(strsearch_h("+CGATT:1",SystemBuf)>0)
				{
					net.reconnect_setp=LINK_IP;//�򿪰�
					net.setp_setp=0;
					net.delay_times=0;
					net.delay=0;
					qwl_sendstring("NBLOT���ųɹ�");
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
void nblot_init_0()
{
								net.delay_times=0;
								net.setp_setp=0;
								net.reconnect_setp=MODE_CHOICE;//����������������
								net.delay=0;
}



/*******************************************************************************
* �� �� ��         : void nblot_rec(void)
* ��������		   : ��������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void nblot_rec(u8 k)
{
	if(k=='+')
	{
		nblot_rec_flag=1;
	}
	switch(nblot_rec_flag)
	{
		case 1:
		{
			if(k=='N')
				nblot_rec_flag++;
		}
		break;
		case 2:
		{
			if(k=='S')
				nblot_rec_flag++;
		}
		break;
		case 3:
		{
			if(k=='O')
				nblot_rec_flag++;
		}
		break;
		case 4:
		{
			if(k=='N')
				nblot_rec_flag++;
		}
		break;
		case 5:
		{
			if(k=='M')
				nblot_rec_flag++;
		}
		break;
		case 6:
		{
			if(k=='I')
			{
				nblot_rec_flag=0;
				nblot_read_flag=1;
			}
		}
		break;
	
		default:break;
	}

}
/*******************************************************************************
* �� �� ��         : nblot_read()
* ��������		   : 
* ��    ��         : 
* ��    ��         : 
*******************************************************************************/
void nblot_read()
{

	if(nblot_read_flag>0)
	{
		
//		nblot_read_flag1_delay=0;
		nblot_read_flag1=1;
		nblot_RESET_flag=0;//�л�����������
//		nblot_rec_times=0;
	NBIOT_SET_FALG=0;
qwl_sendstring("AT+NSORF=0,512");//�������
	qwl_net_sendstring("AT+NSORF=0,512");//�������
		
				nblot_read_flag=0;
	}
}
/*******************************************************************************
* �� �� ��         : u8 nblot_group()
* ��������		   : 
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
/*u8 nblot_group(void)
{
	u16 i=0;
	u16 j=0;
	u16 len=0;
	u8 ch=0;
	u8 k;
	k=1;

		
		len=nblot_rec_times;
		Delay_MS(3);
		if(len==nblot_rec_times);
		{

	qwl_sendchar(NO[nblot_rec_times>>4]);
	qwl_sendchar(NO[nblot_rec_times&0x0f]);
			
			i=strsearch_hq("7E",nblot_rec_buf);
			if(i>1)
			{
				i=i-2;
				
							for(j=i;j<nblot_rec_times;j++)
			{
				qwl_sendchar(nblot_rec_buf[j]);
			}				
			for(j=i;j<nblot_rec_times;j++)
			{ch=(ASCII_hex[nblot_rec_buf[j]-0x30]<<4)+ASCII_hex[nblot_rec_buf[j+1]-0x30];
				j++;
				eth_rec_deal(ch);
//				qwl_sendchar(ch);


								
			}
									
	
			k=0;
			nblot_rec_times=0;
			
			
			}
				
		}
	
		//i=strsearch_h("+NSORF=0,",SystemBuf);
		//{
		//	len=
			//nblot_read_flag=0;
		//}
	
	return k;
}

*/





