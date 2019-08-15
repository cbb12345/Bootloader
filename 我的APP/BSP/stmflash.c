#include "stmflash.h"


//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(unsigned int WriteAddr,unsigned short int pBuffer)   
{ 			 		 
	FLASH_Unlock();						//����
	FLASH_ErasePage(WriteAddr);//�����������
	FLASH_ProgramHalfWord(WriteAddr,pBuffer);
	FLASH_Lock();//����
}


//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
unsigned short int STMFLASH_ReadHalfWord(unsigned int faddr)
{
	return *(volatile unsigned short int*)faddr; 
}









