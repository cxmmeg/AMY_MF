#include "config.h"


//#define    	POST_VER		   		0x20 //Э��汾
#define    	SYS_DEVICE_TYPE		   	0xD0 //�豸����

uchar  		RTN = 0; 

uchar ChkPostDataSum(uchar *DaPtr,uint16 len);
void PostSendDefault(USART_FUNCTION uf);
void DealPostPro(USART_FUNCTION uf,uchar *DaPtr,uint16 len);

//������������������������������������������������������������������������������
// ��������  : PostSendByte
// ��������  : ����һ�ֽ�����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendByte(USART_FUNCTION uf,uchar ChDa)
{
	API_AddProtocolBuffer(uf,(uchar *)&ChDa, 1);
}

//������������������������������������������������������������������������������
// ��������  : PostSendBytes
// ��������  : ����n���ֽڣ���n��ASCII��
// �������  : pchar �������� count ����.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendBytes(USART_FUNCTION uf,uchar *pchar,uchar count)
{
	if(count){
		while(count--)	
			PostSendByte(uf,*pchar++);
	}
}

//������������������������������������������������������������������������������
// ��������  : PostSendOneByte
// ��������  : ����һ�ֽ�����,2�ֽ�ASC
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendByteASC(USART_FUNCTION uf,uchar ChDa)
{
	uchar TempH;
	uchar TempL;
	TempH = hi_hex2asc(ChDa);
	TempL = low_hex2asc(ChDa);

	PostSendByte(uf,TempH);
	PostSendByte(uf,TempL);
}

//������������������������������������������������������������������������������
// ��������  : PostSend_HexStr
// ��������  : �����ַ���(��ȡ���ò�����)
// �������  : None .
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSend_HexStr(USART_FUNCTION uf,uchar *P,uchar Cnt)  
{	
	uchar  i;
	P += Cnt;
	for(i=0; i<Cnt; i++)            
	{	
		P--;
		PostSendByteASC(uf,hi_hex2asc(*P));
		PostSendByteASC(uf,low_hex2asc(*P));
	}
	PostSendByteASC(uf,0x2C);
}

//������������������������������������������������������������������������������
// ��������  : PostSend_Word
// ��������  : �������ַ���
// �������  : TemInt ��������.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSend_Word(USART_FUNCTION uf,uint TemInt)  
{	uchar  Te;
	Te = (uchar )(TemInt>>8);
	PostSendByteASC(uf,Te);
	PostSendByteASC(uf,TemInt);
}

//������������������������������������������������������������������������������
// ��������  : PostSend_Str
// ��������  : �����ַ���
// �������  : PS1�ַ���
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSend_Str(USART_FUNCTION uf,char *PS1)
{
	while(*PS1 != 0x00)
	{
		PostSendByteASC(uf,*PS1);
		PS1++;
	}
	PostSendByteASC(uf,0x2C);
}

//������������������������������������������������������������������������������
// ��������  : PostSendHead
// ��������  : �ʵ粿Э�鷢������֡ͷ����,Ӧ��ʹ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendHead(USART_FUNCTION uf)
{
	uchar len_H = 0x00;
	uchar len_L = 0x00;
	uchar PostVer = POST_VER;
	uchar DeviceType = SYS_DEVICE_TYPE;
	/*��λ�շ�����*/
	API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_TX);
	PostSendByte(uf,'~');
	PostSendByteASC(uf,PostVer);
	PostSendByteASC(uf,ModAddress);
	PostSendByteASC(uf,DeviceType);
	PostSendByteASC(uf,RTN);	    //RTN
	PostSendByteASC(uf,len_H);	   			//���ⷢ�������峤��
	PostSendByteASC(uf,len_L);	   			//���ⷢ�������峤��
}

//������������������������������������������������������������������������������
// ��������  : PostSendHeadCmd
// ��������  : �ʵ粿Э�鷢������֡ͷ���֣�����ʹ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendHeadCmd(USART_FUNCTION uf,uchar Addr,uchar cmd)	    
{
	uchar len_H = 0x00;
	uchar len_L = 0x00;
	uchar PostVer = POST_VER;
	uchar DeviceType = SYS_DEVICE_TYPE;
	/*��λ�շ�����*/
	API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_TX);
	PostSendByte(uf,'~');
	PostSendByteASC(uf,PostVer);
	PostSendByteASC(uf,Addr);
	PostSendByteASC(uf,DeviceType);
	PostSendByteASC(uf,cmd);	    //RTN
	PostSendByteASC(uf,len_H);	   			//���ⷢ�������峤��
	PostSendByteASC(uf,len_L);	   			//���ⷢ�������峤��
}

//������������������������������������������������������������������������������
// ��������  : PostSendTail
// ��������  : �ʵ粿Э�鷢��֡β
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendTail(USART_FUNCTION uf)
{	
	PostSendByte(uf,0x0D);
	API_StartSendBuffer(uf);
}

//������������������������������������������������������������������������������
// ��������  : SendPostChksum
// ��������  : �ʵ粿Э��ط���У��ͺ�����Ҫ��HEXת��ASC����У��ͣ�
// �������  : len У�����ݳ���.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendPostChksum(USART_FUNCTION uf)  
{
	uchar *TxBuf;
	uint16 *Len;
	uchar *p;
	uint i,ChksumData=0;
	uchar tempSumH;
	uchar tempSumL;

	if(API_GetProtocolBufferOrLen(uf,&TxBuf,&Len)>-1)
	{
		p = TxBuf+1;
		for(i=0;i<(*Len)-1;i++)
		{
			ChksumData+=*p++;
		}
		ChksumData=~ChksumData+1;
		tempSumH = (uchar)(ChksumData>>8);
		tempSumL = (uchar)ChksumData;

		PostSendByteASC(uf,tempSumH);
		PostSendByteASC(uf,tempSumL);
	}
}

//������������������������������������������������������������������������������
// ��������  : PostSendFixupByte
// ��������  : �������ͻ���̶�λ�ò���
// �������  : FixupNo λ�� valus ����.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendFixupByte(USART_FUNCTION uf,uchar *TxBuf,uchar FixupNo,uchar valus)
{
	TxBuf[FixupNo] = hi_hex2asc(valus);
	TxBuf[FixupNo+1] = low_hex2asc(valus);
}

//������������������������������������������������������������������������������
// ��������  : PostSendDataLen
// ��������  : �ʵ粿Э�鷢�����ݳ���
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendDataLen(USART_FUNCTION uf)
{
	uchar *TxBuf;
	uint16 *Len;
	Taglchksum  tmplen = {0};
	unsigned int word;
	unsigned char tempH;
	unsigned char tempL;

	if(API_GetProtocolBufferOrLen(uf,&TxBuf,&Len)>-1)
	{
		tmplen.word = *Len - 13;
		tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
		tmplen.div.lChksum = ~tmplen.div.lChksum+1;
		word = tmplen.word;
		tempH = word>>8;
		tempL =	(uchar)word;

		PostSendFixupByte(uf,TxBuf,9,tempH);	   
		PostSendFixupByte(uf,TxBuf,11,tempL);
	}
}

//������������������������������������������������������������������������������
// ��������  : ChkPostDataSum
// ��������  : �ʵ粿Э�����ݳ���У��
// �������  : None.
// �������  : None
// ���ز���  : �ɹ�����0  ʧ�ܷ���1
//������������������������������������������������������������������������������
uchar ChkPostDataSum(uchar *DaPtr,uint16 len)
{
	Taglchksum  tmplen  = {0};
	unsigned int  word;

	word = TwocharToInt(DaPtr[5],DaPtr[6]);	//�յ�����Ϣ�峤��
	tmplen.word = len - 18;					//ʵ���յ�����Ϣ���ۼӵĳ���,�ٶ�Щ����У�� 
	tmplen.div.lChksum = tmplen.div.hLenth+tmplen.div.mLenth+tmplen.div.lLenth;
	tmplen.div.lChksum=~tmplen.div.lChksum+1;

	if(word!=tmplen.word)
		return 1;	
	else
		return 0;
}

//������������������������������������������������������������������������������
// ��������  : SendDefault
// ��������  : �ʵ粿Э��Ӧ��������������
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void PostSendDefault(USART_FUNCTION uf)
{	
	PostSendHead(uf);
	PostSendByteASC(uf,1);   					// DataFlag
	PostSendByteASC(uf,1);   					// ��״̬ΪЭ��ת������PC��ͨ��״̬
	PostSendDataLen(uf);
	PostSendPostChksum(uf);
	PostSendTail(uf);
}

//������������������������������������������������������������������������������
// ��������  : DealPostProtocol
// ��������  : �ʵ粿Э�����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void DealPostProtocol(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{ 		  
	unsigned char CallAddr;
	unsigned char utemp;	
	unsigned char Var;
	u16 i;
	for(i=0;i<len;i++)
	{
		qwl_sendchar(DaPtr[i]);
	}
	Var=twoascii2byte(DaPtr[1],DaPtr[2]);//Э��汾	
	CallAddr=twoascii2byte(DaPtr[3],DaPtr[4]);				/*��λ�����е�ַ*/
		
	if(CallAddr==ModAddress ||CallAddr==0xff||CallAddr==0x01)
	{	//�жϳ����б��� 6=SOI+EOI+SUM==4 ASCII��
		utemp=chk_post_chksum((uchar *)&DaPtr[1],len-6);	

		//��У����Ƿ���ȷ
		if(!utemp)
		{  	 
            // ��ASCIIת����HEX
    		AscToHexFun(&DaPtr[1],len-2); 
            if(DaPtr[4]==0x50)	//��ȡ��ַ �㲥ֱ����ͨ��
			{	
                RTN=0;		
				DealPostPro(uf,DaPtr,len);	
			}  	
            else
            {		 				    
    			if((Var==0x20)||(Var==0x21)||(Var==0x22))
    			{	 
    				if(1)//!ChkPostDataSum(DaPtr,len))
    				{  
    					RTN=0;								
    					DealPostPro(uf,DaPtr,len); 
    				}
    				else
    				{	//���������ۼӺͼ�鲻��
    					RTN=0x03;
    				}									
    			}
    			else
    			{
                    RTN=0x01;	
    			}
            }
		}
		else
		{
            RTN=0x02;//�ۼӺͼ�鲻��
		}										
	}
	else
	{
		API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_ALL);
	}

	if(RTN)
	{	//��Ϣ�����
		PostSendDefault(uf);
	}
}

//������������������������������������������������������������������������������
// ��������  : DealPostPro
// ��������  : �ʵ粿Э�鴦��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������ 
void DealPostPro(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{	
    switch(uf)
	{
		case USART_FUNCTION_WCDMA:
			Uploading_Pro(uf,DaPtr,len);
			break;
		case USART_FUNCTION_MOUNT + 0:
			break;
		case USART_FUNCTION_MOUNT + 1:
			break;
		case USART_FUNCTION_MSG:
			Uploading_Pro(uf,DaPtr,len);
			break;
		case USART_FUNCTION_DEBUG:
			Uploading_Pro(uf,DaPtr,len);
			break;
	}
}



//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������


