#include "menu.h"
#include "ymodem.h"
#include "usart.h"

pFunction Jump_To_Application;
uint32_t JumpAddress;

uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] =
  {
    0
  };

uint8_t FileName[FILE_NAME_LENGTH];
	
/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
unsigned char SerialDownload(void)
{
  uint8_t Number[10] = {0};
  int32_t Size = 0;

  Size = Ymodem_Receive(&tab_1024[0]);//开始接收文件,成功会返回文件的大小
  if (Size > 0)  //返回成功
  {
		SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    SerialPutString(FileName);
    Int2Str(Number, Size);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
    SerialPutString("-------------------\n");
		return 0;
  }
  else if (Size == -1)
  {
    //SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
		return 1;
	}
  else if (Size == -2)
  {
    //SerialPutString("\n\n\rVerification failed!\n\r");
		return 2;
	}
  else if (Size == -3)
  {
    //SerialPutString("\r\n\nAborted by user.\n\r");
		return 3;
  }
  else
  {
   // SerialPutString("\n\rFailed to receive the file!\n\r");
		return 4;
	}
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
  uint8_t status = 0 ; 

  SerialPutString("\n\n\rSelect Receive File\n\r");

  if (GetKey() == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);

    if (status != 0) 
    {
      SerialPutString("\n\rError Occurred while Transmitting File\n\r");
    }
    else
    {
      SerialPutString("\n\rFile uploaded successfully \n\r");
    }
  }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
	
void Main_Menu(void)
{
  uint8_t key = 0;

  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n=              (C) COPYRIGHT 2012 STMicroelectronics                 =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=  STM32F0xx In-Application Programming Application  (Version 1.0.0) =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=                                   By MCD Application Team          =");
  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n\r\n");

  while (1)
  {
    SerialPutString("\r\n================== Main Menu ============================\r\n\n");
    SerialPutString("  Download Image To the STM32F0xx Internal Flash ------- 1\r\n\n");
    SerialPutString("  Upload Image From the STM32F0xx Internal Flash ------- 2\r\n\n");
    SerialPutString("  Execute The New Program ------------------------------ 3\r\n\n");

    if(FlashProtection != 0)//如果有写保护
    {
      SerialPutString("  Disable the write protection ------------------------- 4\r\n\n");
    }

    SerialPutString("==========================================================\r\n\n");

    /* Receive key */
    key = GetKey();//while一直等待接收到指令
    if (key == 0x31)//这里我从APP跳到Bootloader用的是字符'1',而官方也是字符'1'才进行升级,所以实际上用超级终端需要
    {			          //发送两次字符'1'才会开始等待升级
      /* Download user application in the Flash */
			if(SerialDownload()==0)//如果传输完成跳回APP
			{
				//标志清除
				FLASH_Unlock();						 //解锁
				FLASH_ErasePage(0x8004A38);//擦除这个扇区,即擦除0xAAAA
				FLASH_Lock();              //上锁
				
			  if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
				{ 
					/* Jump to user application */
					JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
					Jump_To_Application = (pFunction) JumpAddress;
					
					/* Initialize user application's Stack Pointer */
					__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
					
					/* Jump to application */
					Jump_To_Application();
				}
				
				return;
			}
			else
			{
				return;
			}
    }
		//官方的东西,我觉得多余就去掉了
//    else if (key == 0x32)
//    {
//      /* Upload user application from the Flash */
//      SerialUpload();
//    }
//		else if (key == 0x33) /* execute the new program */
//    {
//      /* Jump to user application */
//      JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
//      Jump_To_Application = (pFunction) JumpAddress;

//      /* Initialize user application's Stack Pointer */
//      __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
//      
//      /* Jump to application */
//      Jump_To_Application();
//    }
//    else if ((key == 0x34) && (FlashProtection == 1))
//    {
//      /* Disable the write protection */
//      writeprotect = FLASH_If_DisableWriteProtection();

//      switch (writeprotect)
//      {
//        case 0:
//        {
//          SerialPutString("Write Protection disabled...\r\n");
//          SerialPutString("...and a System Reset will be generated to reload the new option bytes\r\n");
//          
//          /* Launch loading new option bytes */
//          FLASH_OB_Launch();
//          
//          break;
//        }
//        case 1:
//        {
//          SerialPutString("Error: Flash write unprotection failed...\r\n");
//          break;
//        }
//        case 2:
//        {
//          SerialPutString("Flash memory not write protected\r\n");
//          break;
//        }
//        default:
//        {
//        }
//      }
//    }
//    else
//    {
//      if (FlashProtection == 0)
//      {
//        SerialPutString("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
//      }
//      else
//      {
//        SerialPutString("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
//      }
//    }
  }
}








