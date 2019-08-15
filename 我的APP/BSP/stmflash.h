#ifndef _STM32FLASH_H
#define _STM32FLASH_H

#include "stm32f0xx.h"


void STMFLASH_Write_NoCheck(unsigned int WriteAddr,unsigned short int pBuffer);
unsigned short int STMFLASH_ReadHalfWord(unsigned int faddr);


#endif







