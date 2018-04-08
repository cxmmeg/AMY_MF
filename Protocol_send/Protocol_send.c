#include "config.h"


PROTOCOL_SEND_TYPE Net_Data_Send={5,0,5,SEND_READY,WAIT_REC};



/*******************************************************************************
* �� �� ��         : void process_F8()
* ��������		   : ����12�����
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void process_F8(u8 RTN)
{  


	u8 i;

	u8 post_buf1[200];
	u16 post_buf1_len=0;

				for(i=0;i<16;i++)
				{post_buf1[post_buf1_len]=AmyID[i];
					post_buf1_len++;
				}
				for(i=0;i<2;i++)
				{post_buf1[post_buf1_len]=software_version[i];
					post_buf1_len++;
				}
				for(i=0;i<22;i++)
				{post_buf1[post_buf1_len]=Upgrade_ip[i];
					post_buf1_len++;
				}

	
			post_group(post_buf1,post_buf1_len,RTN);
   net_send(post_buf,post_len);
} 
//������������������������������������������������������������������������������
// ��������  : upload_link_send()
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void upload_link_send_POST()
{
			
	process_F8(0xf8);

}
//������������������������������������������������������������������������������
// ��������  : upload_link_send()
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void upload_link_send()
{
								switch(Net_Data_Send.upload_handshake.send_state )
											{
												case SEND_READY:
												{
													if(Net_Data_Send.upload_handshake.TIMES>0)
													{
														if(Net_Data_Send.upload_handshake.TIMES!=0xff)
															Net_Data_Send.upload_handshake.TIMES--;
														
														Net_Data_Send.upload_handshake.delays=0;
														Net_Data_Send.upload_handshake.send_state=WAIT_ACK;
													upload_link_send_POST();
													}
													
												}break;
												case WAIT_ACK:
												{
													if(Net_Data_Send.upload_handshake.delays>Net_Data_Send.upload_handshake.MAX_delays)
													{
														Net_Data_Send.upload_handshake.delays=0;
														Net_Data_Send.upload_handshake.send_state=SEND_READY;
														qwl_sendstring("�ȴ����ֳ�ʱ");
														
													}
													if(Net_Data_Send.upload_handshake.REC_state == REC_OK)
													{
														Net_Data_Send.upload_handshake.TIMES=5;
														Net_Data_Send.upload_handshake.send_state=STOP_SEND;
														Net_Data_Send.upload_handshake.delays=0;

													}
												}break;
												case STOP_SEND:
												{
														//�����������ݳ�ʼ��
													Net_Data_Send.upload_handshake.delays =0;
													Net_Data_Send.upload_handshake.MAX_delays  =5;
													Net_Data_Send.upload_handshake.TIMES  =5;
													//Net_Data_Send.upload_handshake.send_state  =SEND_READY;
													Net_Data_Send.upload_handshake.REC_state = WAIT_REC;
												}break;
		
												default:break;
											}	 
}											
//������������������������������������������������������������������������������
// ��������  : net_data_send_init()
// ��������  : �������ݷ��ͳ�ʼ������
// �������  : None
// �������  : None
// ���ز���  :��
//������������������������������������������������������������������������������
void net_data_send_init()
{
	//�����������ݳ�ʼ��
	Net_Data_Send.upload_handshake.delays =0;
	Net_Data_Send.upload_handshake.MAX_delays  =5;
	Net_Data_Send.upload_handshake.TIMES  =5;
	Net_Data_Send.upload_handshake.send_state  =SEND_READY;
	Net_Data_Send.upload_handshake.REC_state = WAIT_REC;	
	
}
//������������������������������������������������������������������������������
// ��������  : net_data_send_delay
// ��������  : �������ݷ�����ʱ
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void net_data_send_delay()
{
	Net_Data_Send.upload_handshake.delays++;
}
//������������������������������������������������������������������������������
// ��������  : net_data_send()
// ��������  : �������ݷ���
// �������  : None
// �������  : None
// ���ز���  : ��
//������������������������������������������������������������������������������
void net_data_send()
{
	if(net.reconnect_setp==LINK_OK)
	{

		upload_link_send();
										
	}
}



