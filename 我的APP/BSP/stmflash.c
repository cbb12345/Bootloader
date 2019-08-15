#include "stmflash.h"


//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(unsigned int WriteAddr,unsigned short int pBuffer)   
{ 			 		 
	FLASH_Unlock();						//解锁
	FLASH_ErasePage(WriteAddr);//擦除这个扇区
	FLASH_ProgramHalfWord(WriteAddr,pBuffer);
	FLASH_Lock();//上锁
}


//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
unsigned short int STMFLASH_ReadHalfWord(unsigned int faddr)
{
	return *(volatile unsigned short int*)faddr; 
}









