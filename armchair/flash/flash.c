#include "config.h"




u8 software_version[2]={0};//����汾
u8 Hardware_version[2]={0};//Ӳ���汾
u8 Upgrade_ip[22]={0};//����IP
u8 carrieroperator=0;//��Ӫ��
u8	module_name=0;//ģ���ͺ�
u8 amy_max_total=100;//��Ħ��������������
//u8 SIM_ID[20]={0};//SIM��ID
u8 zd_sl_flag=0;//�ն�����ˢ���־
/*******************************************************************************
* �� �� ��         : Write_Flash
* ��������		   : дSTM32Flash,8λ�洢,һ�����1024k
* ��    ��         : 16λ���ݣ�flash ��ַ
* ��    ��         : 0/1
*******************************************************************************/
void Write_Flash(u8 *flash_data,u32 flash_adr,u16 flash_len)
{ 
	u16 flash_16data=0;
	u16 flash_adr_point=0;
//	u8 *read_flash;
	u16 i=0;
	FLASH_Unlock();//����	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ	

	FLASH_ErasePage(flash_adr);//Ҫ����ҳ����ʼ��ַ
	while(i<flash_len)
 	{
		flash_16data=0;
			if((i+1)<flash_len)
			{flash_16data=(*(flash_data+1)<<8)+*flash_data;
				i=i+2;
			}
			else
			{i=i+1;
			flash_16data=*flash_data;	
			}
  		FLASH_ProgramHalfWord((flash_adr+flash_adr_point),flash_16data);//д����;32λ����FLASH_ProgramWord��16λ����FLASH_ProgramHalfWord
			flash_adr_point=flash_adr_point+2;
  		flash_data++;
				flash_data++;
		}


	//FLASH_ProgramWord(FLASH_ADR+4,data);//��������д���ݸ�flash��ÿдһ����һ���ּ��ĸ��ֽ�	
	FLASH_Lock();//����

//read_flash=(u8*)(flash_adr);
//sendstring20(read_flash);



}

/*******************************************************************************
* �� �� ��         : Add_Flash
* ��������		   : дSTM32Flash,8λ�洢,һ�����2048
* ��    ��         : 16λ���ݣ�flash ��ַ
* ��    ��         : 0/1
*******************************************************************************/
void Add_Flash(u8 *flash_data,u32 flash_adr,u16 flash_len)
{ 
	u16 flash_16data=0;
	u16 flash_adr_point=0;
//	u8 *read_flash;
	u16 i=0;
//	FLASH_Unlock();//����	
//	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
//					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ	

	while(i<flash_len)
 	{
		flash_16data=0;
			if((i+1)<flash_len)
			{flash_16data=(*(flash_data+1)<<8)+*flash_data;
				i=i+2;
			}
			else
			{i=i+1;
			flash_16data=*flash_data;	
			}
  		FLASH_ProgramHalfWord((flash_adr+flash_adr_point),flash_16data);//д����;32λ����FLASH_ProgramWord��16λ����FLASH_ProgramHalfWord
			flash_adr_point=flash_adr_point+2;
  		flash_data++;
				flash_data++;
		}


	//FLASH_ProgramWord(FLASH_ADR+4,data);//��������д���ݸ�flash��ÿдһ����һ���ּ��ĸ��ֽ�	
	FLASH_Lock();//����

//read_flash=(u8*)(flash_adr);
//sendstring20(read_flash);



}
/*******************************************************************************
* �� �� ��         : Write_Flash_ALLAMY
* ��������		   : дSTM32Flash
* ��    ��         : 8λ���ݣ�flash ��ַ
* ��    ��         : 0/1
*******************************************************************************/
void Write_Flash_ALLAMY()
{ //u16 i;
	//u8 *read_flash;
	//for(i=0;i<2;i++)
	//{Write_Flash(&amy_record_buf[i*1024],(flash_AMY_adr1+i*1024),1024);
	//}
	
	Write_Flash(&amy_record_buf[0],flash_AMY_adr1,2048);
Write_Flash(&amy_record_buf[2048],(flash_AMY_adr1+2048),2048);
	Write_Flash(&amy_record_buf[4096],(flash_AMY_adr1+4096),1154);
	
	
//qwl_sendstring("����FLASH1");
//						read_flash=(u8*)(flash_AMY_adr1);
//						for(i=0;i<5250;i++)
//						{
//							qwl_sendchar(read_flash[i]);
//						}

}

/*******************************************************************************
* �� �� ��         : Write_light_idip()
* ��������		   : дSTM32Flash
* ��    ��         : 8λ���ݣ�flash ��ַ
* ��    ��         : 0/1
*******************************************************************************/
void Write_light_idip()
{ 
	u8 write_flash_buf[111];
	u8 i=0;
	for(i=0;i<16;i++)
	{
		write_flash_buf[i]=AmyID[i];
	}
	for(i=0;i<22;i++)
	{
		write_flash_buf[i+16]=light_TCP[i];
	}
		for(i=0;i<22;i++)
	{
		write_flash_buf[i+38]=light_UDP[i];
	}
			for(i=0;i<22;i++)
	{
		write_flash_buf[i+60]=Upgrade_ip[i];
		
	}
	for(i=0;i<2;i++)
	{
		write_flash_buf[i+82]=software_version[i];
	}
	for(i=0;i<2;i++)
	{
		write_flash_buf[i+84]=Hardware_version[i];
	}

	write_flash_buf[86]=carrieroperator;
	
	write_flash_buf[87]=module_name;
	
	write_flash_buf[88]=amy_max_total;
	
	for(i=0;i<20;i++)
	{
		write_flash_buf[i+89]=SIM_ID[i];
	}
	write_flash_buf[109]=zd_sl_flag;
	Write_Flash(write_flash_buf,AMYIDIP_adr,110);



}

/*******************************************************************************
* �� �� ��         : Init_Amy_Id()
* ��������		   	: ��ʼ����Ħ���ն����ص�ID��Ĭ��STM32оƬID�������޸ġ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Init_Amy_Id()
{u32 STM32ID_adr=0x1ffff7e8;//STM32ΨһID�洢��ַ
u8 UDP_init_word[]={"123.159.193.22,7272"};//112.74.98.39,8647����Ĭ��IP
u8 TCP_init_word[]={"123.159.193.22,7271"};//112.74.98.39,8647����Ĭ��IP
u8 *read_flash;	
char SoftWareVer[7];
	u8 CpuID[12];
	u8 i;
	u8 n=0;
	u8 ff_flag=0;//����ff��־
	u8 oo_flag=0;//����00��־
		qwl_sendstring("��ȡȫ��ΨһID");

	for(i=0;i<12;i++)
	{CpuID[i]=*(vu32*)(STM32ID_adr+i);
		qwl_sendchar(NO[CpuID[i]>>4]);
		qwl_sendchar(NO[CpuID[i]&0x0f]);
	}


				qwl_sendstring("��ȡ��Ħ��id");						
				read_flash=(u8*)(AMYIDIP_adr);

						for(i=0;i<16;i++)
						{
							AmyID[i]=read_flash[i];
							qwl_sendchar(NO[AmyID[i]>>4]);
							qwl_sendchar(NO[AmyID[i]&0x0f]);
							if(AmyID[i]==0xff)//����ԭ����ʼ��
								ff_flag++;
							if(AmyID[i]==0x00)
								oo_flag++;
						}
						qwl_sendchar(0x0d);
						qwl_sendchar(0x0a);
						for(i=0;i<16;i++)
						{
							qwl_sendchar(AmyID[i]);
						}
						qwl_sendchar(0x0d);
						qwl_sendchar(0x0a);
						
						if(ff_flag==16)//���ڵ�����ff,��־λ��1���ָ�ԭ��ID
							n=n|0x01;
						if(oo_flag==16)//���ڵ�����0,��־λ��1���ָ�ԭ��ID
							n=n|0x02;
						switch(AmyID[3])
						{
							
							case 0x32:
							{qwl_sendstring("������");
								Shared_Module.Module=oxygen_plant;//ʶ���������͵�ģ��,������
							}break;
							case 0x33:
							{qwl_sendstring("Ͷ�һ�");
								Shared_Module.Module=slot_machines;//ʶ���������͵�ģ��,Ͷ�һ�
							}break;
							default:
							{
								qwl_sendstring("������˹");
								Shared_Module.Module=ruilaikesi;//ʶ���������͵�ģ��,������˹
							}break;
						}
						
						
						if(n>0)
						{qwl_sendstring("ˢ��ԭ��ID");//ԭ��IDΪSTM32��ǰ8λΨһID
							
								for(i=0;i<12;i++)
										{		
											AmyID[i]=CpuID[i];
											
											qwl_sendchar(NO[AmyID[i]>>4]);
											qwl_sendchar(NO[AmyID[i]&0x0f]);
										}	
								for(i=12;i<16;i++)
										{
											AmyID[i]=0;
											qwl_sendchar(NO[AmyID[i]>>4]);
											qwl_sendchar(NO[AmyID[i]&0x0f]);
										}											
//										Write_Flash_ALLAMY();//����FLASH��
						}
						
						qwl_sendstring("��ȡTCP");	
						for(i=0;i<22;i++)
						{
							light_TCP[i]=read_flash[i+16];
							if(light_TCP[i])
							qwl_sendchar(light_TCP[i]);
							if(light_TCP[i]==0xff)
								ff_flag++;
							if(light_TCP[i]==0x00)
								oo_flag++;
						}
						if(ff_flag==22)//���ڵ�����ff,��־λ��1���ָ�ԭ��ID
							n=n|0x04;
						if(oo_flag==22)//���ڵ�����0,��־λ��1���ָ�ԭ��ID
							n=n|0x08;
						ff_flag=0;
						oo_flag=0;
						
						qwl_sendstring("��ȡUDP");	
								for(i=0;i<22;i++)
						{
							light_UDP[i]=read_flash[i+38];
							if(light_UDP[i])
							qwl_sendchar(light_UDP[i]);
							if(light_UDP[i]==0xff)
								ff_flag++;
							if(light_UDP[i]==0x00)
								oo_flag++;
						}
						if(ff_flag==22)//���ڵ�����ff,��־λ��1���ָ�ԭ��ID
							n=n|0x04;
						if(oo_flag==22)//���ڵ�����0,��־λ��1���ָ�ԭ��ID
							n=n|0x08;
						
						if(n&0x0c)
						{qwl_sendstring("ˢ��ԭ��IP");
							for(i=0;i<22;i++)
										{		
											light_TCP[i]=TCP_init_word[i];
											qwl_sendchar(light_TCP[i]);
										}	
							for(i=0;i<22;i++)
										{		
											light_UDP[i]=UDP_init_word[i];
											qwl_sendchar(light_UDP[i]);
										}		
							
						}
							qwl_sendstring("��ȡ�ն�����");	
							zd_sl_flag=read_flash[109];					
							amy_max_total=read_flash[88];
							qwl_sendchar(NO[amy_max_total>>4]);
							qwl_sendchar(NO[amy_max_total&0x0f]);
						if((amy_max_total==0xff)||(amy_max_total==0x00)||(zd_sl_flag!=0xcc))
						{
							amy_max_total=100;
							zd_sl_flag=0xcc;
							n=1;
						}
						amy_max_total=100;
						if(n>0)
						{qwl_sendstring("д��flashIDIP");
						Write_light_idip();
						}
						qwl_sendstring("��ȡ����IP");
							for(i=0;i<22;i++)
						{
							Upgrade_ip[i]=read_flash[i+60];
							if(Upgrade_ip[i])
							qwl_sendchar(Upgrade_ip[i]);

						}
						qwl_sendstring(" ");
						qwl_sendstring("��ȡ����汾");
						sprintf(SoftWareVer,"V%4.2f",SOFTWARE_VER);
							for(i=0;i<6;i++)
						{
							//software_version[i]=read_flash[i+82];
							qwl_sendchar(SoftWareVer[i]);
						}
						
						qwl_sendstring("��ȡӲ���汾");						
							for(i=0;i<2;i++)
						{
							Hardware_version[i]=read_flash[i+84];
							qwl_sendchar(NO[Hardware_version[i]]);
						}
						
							carrieroperator=read_flash[86];
							if(carrieroperator==0)
							{
								qwl_sendstring("��Ӫ��=����");	
							}
							else if(carrieroperator==1)
							{
								qwl_sendstring("��Ӫ��=�ƶ�");	
							}
							else if(1)
							{
								qwl_sendstring("��Ӫ��=��ͨ");	
							}
						
								module_name=read_flash[87];
							if(module_name==0)
							{
								qwl_sendstring("ģ��=2G");	
							}
							else if(module_name==1)
							{
								qwl_sendstring("ģ��=3G");	
							}
							else if(1)
							{
								qwl_sendstring("ģ��=4G");	
							}
														

							
							qwl_sendstring("��ȡSIM��ID");
								for(i=0;i<20;i++)
						{							
							SIM_ID[i]=read_flash[i+89];
							qwl_sendchar(NO[SIM_ID[i]>>4]);
							qwl_sendchar(NO[SIM_ID[i]&0x0f]);
						}
}


