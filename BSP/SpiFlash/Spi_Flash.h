#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_  

#define FLASH_CHREAD 		0x0B
#define FLASH_CLREAD 		0x03
#define FLASH_PREAD			0xD2

#define FLASH_BUFWRITE1 	0x84
#define FLASH_IDREAD 		0x9F
#define FLASH_STATUS 		0xD7
#define PAGE_ERASE 			0x81
#define PAGE_READ 			0xD2
#define MM_PAGE_TO_B1_XFER  0x53				// �����洢����ָ��ҳ���ݼ��ص���һ������
#define BUFFER_2_WRITE 		0x87				// д��ڶ�������
#define B2_TO_MM_PAGE_PROG_WITH_ERASE 0x86	    // ���ڶ�������������д�����洢��������ģʽ��

#define Dummy_Byte 			0xA5

#define Select_Flash()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define NotSelect_Flash()    GPIO_SetBits(GPIOA, GPIO_Pin_4)

extern unsigned char fac_id;		    //BFH: ������SST
extern unsigned char dev_id;	        //41H: �����ͺ�SST25VF016B 

void SPI_Flash_Init(void);	            //SPI��ʼ��
unsigned char FlashReadID(void);		//��ȡflashID�ĸ��ֽ�
void sect_clr(unsigned long a1); 	
void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
void SST25_W_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
void SST25_W_BLOCKQ(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
int SST25_clr_256_BLOCK(void);
void checktest(void);
#endif


