#include "config.h"

//��������undstand�в���
#define BUILD_PROJECT BOOT_HD

//������������������������������������������������������������������������������
// ��������  : TimingDelay_Decrement
// ��������  : ʹ��SysTick����ȷ��ʱ,TimingDelay������Ϊ1ms�������׼����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static __IO uint8_t KeySacnTimeNum = 0;

void TimingDelay_Decrement(void)
{
    if(KeySacnTimeNum++ >= 5)		//5ms����ɨ��
	{
		KeySacnTimeNum = 0;
	}
}

//������������������������������������������������������������������������������
// ��������  : InitChip
// ��������  : STM32оƬ�ĳ�ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void InitChip(void)			
{

	/*��ʼ��оƬ*/
    SystemInit();


    #ifdef STM32_BOOT
    	/* Set the Vector Table base location at 0x08000000*/  
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000);   
	
    #endif 


}

//������������������������������������������������������������������������������
// ��������  : InitHardWare
// ��������  : �������ڡ�������Դ��Ӳ����ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void InitHardWare(void)			
{   
	/*STM32оƬ�ĳ�ʼ��*/
    InitChip();
	
		/*led��ʼ��*/  
    API_Led_Init();
	
	/*��Ħ��IO��ʼ��*/  
	API_Armchair_IO_Init();

	/*flash��ʼ��*/  
	API_SPI_Flash_Init();

	/*Э���ʼ��*/
	API_Protocols_Init();


	/*��ʱ��ģ���ʼ��*/
	API_Timers_Init();
	
	

}

//������������������������������������������������������������������������������
// ��������  : InitSoftSystem
// ��������  : ʹ�õĹ���ģ���������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void InitSoftSystem(void)		
{

	/*ѡ������û�����������*/
	API_SetTimer(2000,1,TIMER_TYPE_IAP_SELECT_PROGRAM,API_Select_Program_run,NULL);

	/*�����ʼ��*/
	API_qwl_net_init();
	
	/*�������ݷ��ͳ�ʼ��*/
	API_net_data_send_init();
	
	/*������ʱ����ʼ����*/
    API_StartRunTimer();
	
	/*��Ħ�γ�ʼ��*/  
	API_Armchair_Init();
}

//������������������������������������������������������������������������������
// ��������  : main
// ��������  : ϵͳ������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int main(void)  
{ 
	/*�������ڡ�������Դ��Ӳ����ʼ��*/		    
    InitHardWare();		    
	/*ʹ�õĹ���ģ���������*/
    InitSoftSystem();  
	DEBUG("time:%s\n",__TIME__); 
  mofanSDK();
	while (1)
	{	
		
	/*	u8 buf[1024]={0};
		//u8 buf1[1024]={0};
		u16 i=0;
		SST25_clr_256_BLOCK();
		//checktest();
		for(i=0;i<1024;i++)
		{buf[i]=0x44;
		}
		//Delay_MS(1000);
		//SST25_W_BLOCK(0x0,buf,1024);
		Delay_MS(1000);
		for(i=0;i<1024;i++)
		{buf[i]=0x55;
		}
		//SST25_W_BLOCK(2048,buf,1024);
		iap_data.copy_long = 0;
		Save_Iap_Config();
		SST25_R_BLOCK(0x00,buf,1024);
		for(i=0;i<1024;i++)
		qwl_sendchar(buf[i]);
		SST25_R_BLOCK(1024,buf,1024);
		for(i=0;i<1024;i++)
		qwl_sendchar(buf[i]);
		SST25_R_BLOCK(2048,buf,1024);
		for(i=0;i<1024;i++)
		qwl_sendchar(buf[i]);
		SST25_R_BLOCK(4096,buf,1024);
		for(i=0;i<1024;i++)
		qwl_sendchar(buf[i]);

		while(1);*/
		
		/*�������û������������־*/
		API_Check_Run_User_Program_Falg();
		API_WatchDog_Feed();

		/*��ʱ��ģ�鴦��*/
		DoTimer();
		API_WatchDog_Feed();
		
		/*���ڷ������յ�������*/
		API_DoProtocols();
		API_WatchDog_Feed();
		
		/*�Զ���������*/
		API_net_state();
		API_WatchDog_Feed();
		
		//#if CONF_ARMCHAIR_EN
		//if((amy_config&0x02)==0x02)//���簴Ħ��������
		//#endif
		{
		/*���ڷ�������*/
		API_net_data_send();
		API_WatchDog_Feed();
		}
		
		
		
		/*���ڰ�Ħ�δ���*/
		API_ARMCHAIR_DEAL();
		API_WatchDog_Feed();

	}
}

/***********************************end of line ********************************/

