#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "iap.h"

iapfun jump2app;
u16 iapbuf[1024];
//appxaddr:Ӧ�ó������ʼ��ַ  flash ��һ��ֻ��д��16λ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
void iap_write_appbin(u32 appxaddr, u8 *appbuf, u32 appsize)
{
	u16 t;
	u16 i = 0;
	u16 temp;
	u32 fwaddr = appxaddr;//��ǰд��ĵ�ַ
	u8 *dfu = appbuf;
	for (t = 0; t < appsize; t += 2)
	{
		temp = (u16)dfu[1] << 8;
		temp += (u16)dfu[0];
		dfu += 2;//ƫ��2���ֽ�
		iapbuf[i++] = temp;
		if (i == 1024)
		{
			i = 0;
			STMFLASH_Write(fwaddr, iapbuf, 1024);
			fwaddr += 2048;//ƫ��2048  16=2*8.����Ҫ����2.
//			printf("start download app\r\n");
		}
	}
	if (i){
		STMFLASH_Write(fwaddr, iapbuf, i);//������һЩ�����ֽ�д��ȥ.  
//		printf("start download app\r\n");
	}
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
//	u32 data = (*(vu32*)appxaddr);
//	printf("%d",data);
//		printf("start load app\r\n");
	if (((*(vu32*)appxaddr) & 0x2FFE0000) == 0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{
		jump2app = (iapfun)*(vu32*)(appxaddr + 4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
//	else{
//		printf("unstart load app\r\n");
//	}
}
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr;
}
#if STM32_FLASH_WREN	//���ʹ����д   
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
	u16 i;
	for (i = 0; i < NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
		WriteAddr += 2;//��ַ����2.
	}
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
u16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];//�����2K�ֽ�

void STMFLASH_Write(u32 WriteAddr, u16 *pBuffer, u16 NumToWrite)
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
	u16 i;
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr = WriteAddr - STM32_FLASH_BASE;			//ʵ��ƫ�Ƶ�ַ.
	secpos = offaddr / STM_SECTOR_SIZE;					//������ַ  0~127 for STM32F103RBT6
	secoff = (offaddr%STM_SECTOR_SIZE) / 2;			//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain = STM_SECTOR_SIZE / 2 - secoff;		//����ʣ��ռ��С   
	if (NumToWrite <= secremain)secremain = NumToWrite;//�����ڸ�������Χ
	while (1)
	{
		STMFLASH_Read(secpos*STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);//������������������
		for (i = 0; i < secremain; i++)//У������
		{
			if (STMFLASH_BUF[secoff + i] != 0XFFFF)break;//��Ҫ����  	  
		}
		if (i < secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE + STM32_FLASH_BASE);//�����������
			for (i = 0; i<secremain; i++)//����
			{
				STMFLASH_BUF[i + secoff] = pBuffer[i];
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE + STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE / 2);//д����������  
//			printf("download flash\r\n");
		}
		else STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if (NumToWrite == secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff = 0;				//ƫ��λ��Ϊ0 	 
			pBuffer += secremain;  	//ָ��ƫ��
			WriteAddr += secremain*2;	//д��ַƫ��	secremain = STM_SECTOR_SIZE / 2 - secoff;	�������һ��2
			NumToWrite -= secremain;	//�ֽ�(16λ)���ݼ�
			if (NumToWrite>(STM_SECTOR_SIZE / 2))secremain = STM_SECTOR_SIZE / 2;//��һ����������д����
			else secremain = NumToWrite;//��һ����������д����
		}
	};
	FLASH_Lock();//����
}
#endif

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr, u16 *pBuffer, u16 NumToRead)
{
	u16 i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr += 2;//ƫ��2���ֽ�.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr, u16 WriteData)
{
	STMFLASH_Write(WriteAddr, &WriteData, 1);//д��һ���� 
}



