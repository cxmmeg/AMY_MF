#include "config.h"


u8 post_buf[1024];   // ����ݴ���
u16 post_len=0;				// �������
u8 NO[16]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};



/***************************************************** 
 ������CRC8У���ӳ��� X^8+X^5+X^4+X^0
 ��ڲ�����ָ������ָ�룬У���ֽڸ���
 ���ڲ�����8λCRCУ����
******************************************************/  
  
const char CRC8Table[]={  
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,  
  157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,  
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,  
  190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,  
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,  
  219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,  
  101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,  
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,  
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,  
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,  
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,  
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,  
  202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,  
  87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,  
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,  
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53  
};  
  
  
unsigned char CRC8_Table(unsigned char *p, char counter)  
{  
    unsigned char crc8 = 0;  
  
    for( ; counter > 0; counter--){  
        crc8 = CRC8Table[crc8^*p];  
        p++;  
    }  
    return(crc8);  
  
}  
 

/*******************************************************************************
* �� �� ��         : post_group
* ��������		   : ����ʵ粿ͨ�ú�����
* ��    ��         : 
* ��    ��         : ��
*	��ע							��Ҫ�����ȫ�ֺ����ppost_buf��1024�� u16 post_len
*******************************************************************************/
void post_group(u8 *post_buf2,u16 post_buf1_len,u8 command)
{
	u8 i;
	u16 LENGTH;
	u16 j;
	u16 post_CHKSUM=0;
	u16 info_len=0;
	u8 post_buf1[512]={0};

	
	post_buf[0]='~';//��ʼ��־
	post_buf1[0]=POST_VER;//ͨ��Э��汾��
	post_buf1[1]=POST_ADR;//ͨ��Э���ַ
	post_buf1[2]=POST_CID1;//���Ʊ�ʶ�룬��Ħ�ι̶�0XD0
	post_buf1[3]=command;//��Ħ�������ϴ�����
	
		info_len=post_buf1_len;//INFO�ڵ����ݳ���
	j=post_buf1_len*2;
	LENGTH=((j<<4)&0xf000)+((j<<8)&0xf000)+((j<<12)&0xf000);
	LENGTH=((~LENGTH)&0xf000)+0x1000;
	LENGTH=LENGTH+j;
	post_buf1[4]=(LENGTH>>8);
	post_buf1[5]=LENGTH;
		
	for(i=0;i<info_len;i++)//�洢16λ����ID
	{
		post_buf1[6+i]=post_buf2[i];
	}
	j=info_len+6;
			for(i=0;i<j;i++)//ת������Ҫ���͵����ݣ��ʵ粿Э��
		{
		post_buf[i*2+1]=NO[post_buf1[i]>>4];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+1];
			
		post_buf[i*2+2]=NO[post_buf1[i]&0x0f];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+2];
		}	
			
		post_CHKSUM=(~post_CHKSUM)+1;
		post_len=j*2+1;
		post_buf[post_len]=NO[(post_CHKSUM>>12)];//����У����
		post_len++;
		post_buf[post_len]=NO[(post_CHKSUM>>8)&0x0f];
		post_len++;
		post_buf[post_len]=NO[(post_CHKSUM>>4)&0x0f];
		post_len++;
		post_buf[post_len]=NO[post_CHKSUM&0x0f];
		post_len++;
		post_buf[post_len]=0x0d;//������
		post_len++;
}

/*******************************************************************************
* �� �� ��         : GATEWAY_peizhi_pack()
* ��������		   : �����������
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void GATEWAY_peizhi_pack()
{  

		

	u8 i;

	u8 post_buf1[26];
	u16 post_buf1_len=0;

	
   

		
			for(i=0;i<16;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=AmyID[i];
			}
			i=CRC8_Table(AmyID, 16);  
		post_buf1[post_buf1_len]=i;
			post_buf1_len++;
	
			post_group(post_buf1,post_buf1_len,AMY_HEART);
   
} 

								
/*******************************************************************************
* �� �� ��         : GATEWAY_heart_send_pack
* ��������		   : �����������
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void GATEWAY_heart_send_pack()
{  

		

	u8 i;
	u8 j;
	u8 post_buf1[200];
	u16 post_buf1_len=0;
	char SoftWareVer[7];
			j=q2x1278_config_buf[4];
	if(poll_zd.list_please_flag==0)
		j=j|0x80;

			for(i=0;i<16;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=AmyID[i];
			}
			i=CRC8_Table(AmyID, 16);  
		post_buf1[post_buf1_len]=i;
			post_buf1_len++;
		post_buf1[post_buf1_len]=0x00;
			post_buf1_len++;
		post_buf1[post_buf1_len]=amy_total;
			post_buf1_len++;
		post_buf1[post_buf1_len]=amy_CHKSUM;//�ն��ܺ͵�У����
			post_buf1_len++;
		post_buf1[post_buf1_len]=j;//Ƶ���ϴ���Ϊ�˷����Ժ���
			post_buf1_len++;
		post_buf1[post_buf1_len]=CSQ;
			post_buf1_len++;
			sprintf(SoftWareVer,"V%4.2f",SOFTWARE_VER);
							for(i=0;i<6;i++)
						{
							post_buf1[post_buf1_len]=SoftWareVer[i];
							post_buf1_len++;
						}
							for(i=0;i<20;i++)
						{
							post_buf1[post_buf1_len]=SIM_ID[i];
							post_buf1_len++;
						}
						post_buf1[post_buf1_len]=shebei_flag;
							post_buf1_len++;
						
							for(i=0;i<19;i++)
						{
							post_buf1[post_buf1_len]=0x00;
							post_buf1_len++;
						}
			post_group(post_buf1,post_buf1_len,AMY_HEART);
   
}  

/*******************************************************************************
* �� �� ��         : void SIM_ID_send_pack()
* ��������		   : SIM�����ϴ�
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void SIM_ID_send_pack()
{  

		

	u8 i;
	u8 j;
	u8 post_buf1[50];
	u16 post_buf1_len=0;
			j=q2x1278_config_buf[4];
	if(poll_zd.list_please_flag==0)
		j=j|0x80;

			for(i=0;i<16;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=AmyID[i];
			}
			for(i=0;i<20;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=SIM_ID[i];
			}
			
			post_group(post_buf1,post_buf1_len,0x00);
   
}  
/*******************************************************************************
* �� �� ��         : poll_list_send_pack
* ��������		   : ��ѯ�б�״̬�ϴ�
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void poll_list_send_pack()
{  

		

	u8 i;

	u8 post_buf1[50];
	u16 post_buf1_len=0;
			
			for(i=0;i<16;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=AmyID[i];
			}
			i=CRC8_Table(AmyID, 16);  
		post_buf1[post_buf1_len]=i;
			post_buf1_len++;
		post_buf1[post_buf1_len]=0x01;//�б�״̬�ϴ�
			post_buf1_len++;
		for(i=0;i<25;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=poll_zd.zd_state[i];
			}
			post_group(post_buf1,post_buf1_len,AMY_list_state);
   
}  
/*******************************************************************************
* �� �� ��         : poll_list_send_pack
* ��������		   : ��ѯ�б�״̬�ϴ�
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void amy_off_send_pack()
{  

		

	u8 i;

	u8 post_buf1[50];
	u16 post_buf1_len=0;
			
			for(i=0;i<16;i++,post_buf1_len++)//�洢16λ����ID
			{
				post_buf1[post_buf1_len]=AmyID[i];
			}
			i=CRC8_Table(AmyID, 16);  
		post_buf1[post_buf1_len]=i;
			post_buf1_len++;
		post_buf1[post_buf1_len]=0x02;//���н�������
			post_buf1_len++;
			post_buf1[post_buf1_len]=amy_cotrol_time>>8;
			post_buf1_len++;
			post_buf1[post_buf1_len]=(amy_cotrol_time&0xff);
			post_buf1_len++;
			post_buf1[post_buf1_len]=getwayoff_send_flag;
			post_buf1_len++;
			post_group(post_buf1,post_buf1_len,AMY_list_state);
   
} 
/*******************************************************************************
* �� �� ��         : GATEWAY_list_send_pack
* ��������		   : �����б����
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void GATEWAY_list_send_pack()
{

	u16 i;
	u16 j;
	u8 n=0;
	u8 w=0;
	u8 q=0;
	u16 post_CHKSUM=0;
	u8 post_buf1[512];
	u16 h=0;
	u16 info_len=0;
	u8 pack_total;//������
	u8 pack_no;//������
	h=post_len;

	post_buf[0]='~';//��ʼ��־
	post_buf1[0]=POST_VER;//ͨ��Э��汾��
	post_buf1[1]=POST_ADR;//ͨ��Э���ַ
	post_buf1[2]=POST_CID1;//���Ʊ�ʶ�룬��Ħ�ι̶�0XD0
	post_buf1[3]=AMY_LIST ;//��Ħ���б��ϴ�����
	
	pack_total=amy_total/13;
	if(amy_total%13)
	{pack_total++;
	}
	if(getwaylist_send_times>0)
		getwaylist_send_times--;
	pack_no=pack_total-getwaylist_send_times;
	post_buf1[6]=pack_total;//������
	post_buf1[7]=pack_no;//��Ħ�ΰ����
	h=8;
	info_len=2;

	for(i=0;i<16;i++)//�洢16λ����ID
	{
		post_buf1[8+i]=AmyID[i];
		
	}

		i=CRC8_Table(AmyID, 16);  
		post_buf1[24]=i;
			
		post_buf1[25]=0x00;
		post_buf1[26]=amy_total;
		post_buf1[27]=amy_CHKSUM;//�ն��ܺ͵�У����
	info_len=22;
	h=28;
			if(amy_total!=0)
			{
					w=pack_no-1;
					w=w*13;
					for(i=0;i<250;i++)//һ���߼�����ҪĿ�ķֶη����б�
					{
							if(amy_record_buf[i*21]>0)
							{
									n++;
								if(n>w)
								{
										for(q=0;q<17;q++)
										{
											

										post_buf1[h]=amy_record_buf[(i*21)+4+q];
											

											if(q==16)//��״̬��ת����ƽ̨��ʶ��ģ�0������1����.�ն˰���������״̬���Ժ���չ
											{if(post_buf1[h]&0x04)//
													{post_buf1[h]=1;
													}
												
												else if(1)
												{post_buf1[h]=0;
												}
											}
											//qwl_sendchar(post_buf1[h]);
											h++;
											info_len++;
										}	
										post_buf1[h]=amy_record_buf[(i*21)+2];
										h++;
											info_len++;
								}
								if(n>=(13+w))
									break;
							}	
					}
			}
			info_len=info_len*2;
	j=((info_len<<4)&0xf000)+((info_len<<8)&0xf000)+((info_len<<12)&0xf000);
	j=((~j)&0xf000)+0x1000+info_len;
	post_buf1[4]=(j>>8);
	post_buf1[5]=j;
//			qwl_sendchar(0x38);
//			qwl_sendchar(0x38);
	for(i=0;i<h;i++)
	{post_buf[i*2+1]=NO[post_buf1[i]>>4];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+1];
//		qwl_sendchar(post_buf[i*2+1]);
		post_buf[i*2+2]=NO[post_buf1[i]&0x0f];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+2];
//		qwl_sendchar(post_buf[i*2+2]);
	}
//	qwl_sendchar(0x38);
//	qwl_sendchar(0x38);
	h=(h)*2+1;
		post_CHKSUM=(~post_CHKSUM)+1;
	
		post_buf[h]=NO[(post_CHKSUM>>12)];//����У����
		h++;
		post_buf[h]=NO[(post_CHKSUM>>8)&0x0f];
		h++;
		post_buf[h]=NO[(post_CHKSUM>>4)&0x0f];
		h++;
		post_buf[h]=NO[post_CHKSUM&0x0f];
		h++;
		post_buf[h]=0x0d;//������
	h++;
	post_len=h;

	//	amy_total
}
/*******************************************************************************
* �� �� ��         : amystate_send_pack
* ��������		   : ��Ħ��״̬���
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void amystate_send_pack(u8 state,u8 addr)
{u32 flash_adr=0x0803e800;//flash�洢��ַ
	u8 *read_flash;
		u8 i;
	u16 j;

	u16 post_CHKSUM=0;
	u8 post_buf1[100];
	u16 info_len=0;

	post_buf[0]='~';//��ʼ��־
	post_buf1[0]=POST_VER;//ͨ��Э��汾��
	post_buf1[1]=POST_ADR;//ͨ��Э���ַ
	post_buf1[2]=POST_CID1;//���Ʊ�ʶ�룬��Ħ�ι̶�0XD0
	post_buf1[3]=0x00 ;//��Ħ��״̬�ϴ�����
	
	info_len=70;//INFO�ڵ����ݳ���
	j=((info_len<<4)&0xf000)+((info_len<<8)&0xf000)+((info_len<<12)&0xf000);
	j=((~j)&0xf000)+0x1000+info_len;
	post_buf1[4]=(j>>8);
	post_buf1[5]=j;
	
	read_flash=(u8*)(flash_adr);//��ȡ16λ����ID
	for(i=0;i<16;i++)//�洢16λ����ID
	{
		post_buf1[6+i]=read_flash[i];
	}

for(i=0;i<16;i++)//�洢16λ����ID
	{
		post_buf1[i+22]=amy_check_ID[i];
	}
	
		post_buf1[38]=addr;//�ն˵�ַ
		post_buf1[39]=state;	//�ն�״̬

		post_buf1[40]=0;//ģʽ����
	
	for(i=0;i<41;i++)
	{
		post_buf[i*2+1]=NO[post_buf1[i]>>4];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+1];
		post_buf[i*2+2]=NO[post_buf1[i]&0x0f];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+2];
	}
		post_CHKSUM=(~post_CHKSUM)+1;
	
				post_buf[83]=NO[(post_CHKSUM>>12)];//����У����
		post_buf[84]=NO[(post_CHKSUM>>8)&0x0f];
		post_buf[85]=NO[(post_CHKSUM>>4)&0x0f];
		post_buf[86]=NO[post_CHKSUM&0x0f];
		post_buf[87]=0x0d;//������

	
	post_len=88;
}

/*******************************************************************************
* �� �� ��         : amyack_send_pack
* ��������		   : ��Ħ��״̬���
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void amyack_send_pack(u8 state)
{
		u8 i;

	u16 post_CHKSUM=0;
	u8 post_buf1[25];

	post_buf[0]='~';//��ʼ��־
	post_buf1[0]=POST_VER;//ͨ��Э��汾��
	post_buf1[1]=POST_ADR;//ͨ��Э���ַ
	post_buf1[2]=POST_CID1;//���Ʊ�ʶ�룬��Ħ�ι̶�0XD0
	
	post_buf1[3]=state ;//��Ħ��״̬����
	
	post_buf1[4]=0x00;//info����
	post_buf1[5]=0x00;
	
	
	
	for(i=0;i<6;i++)
	{
		post_buf[i*2+1]=NO[post_buf1[i]>>4];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+1];
		post_buf[i*2+2]=NO[post_buf1[i]&0x0f];
		post_CHKSUM=post_CHKSUM+post_buf[i*2+2];
	}
		post_CHKSUM=(~post_CHKSUM)+1;
	
				post_buf[13]=NO[(post_CHKSUM>>12)];//����У����
		post_buf[14]=NO[(post_CHKSUM>>8)&0x0f];
		post_buf[15]=NO[(post_CHKSUM>>4)&0x0f];
		post_buf[16]=NO[post_CHKSUM&0x0f];
		post_buf[17]=0x0d;//������

	
	post_len=18;
}



