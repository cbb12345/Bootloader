/**
  ******************************************************************************
  * @file    STM32F0xx_IAP/src/ymodem.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "ymodem.h"
#include "string.h"
#include "flash_if.h"
#include "aes.h"


/** @addtogroup STM32F0xx_IAP
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t FileName[];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Receive byte from sender
  * @param  c: Character
  * @param  timeout: Timeout
  * @retval 0: Byte received
  *         -1: Timeout
  */
static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
  while (timeout-- > 0)
  {
    if (SerialKeyPressed(c) == 1)//如果收到串口数据
    {
      return 0;
    }
  }
  return -1;
}

/**
  * @brief  Send a byte
  * @param  c: Character
  * @retval 0: Byte sent
  */
static uint32_t Send_Byte (uint8_t c)
{
  SerialPutChar(c);
  return 0;
}

/**
  * @brief  Update CRC16 for input byte
  * @param  CRC input value 
  * @param  input byte
  * @retval Updated CRC value
  */
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
  uint32_t crc = crcIn;
  uint32_t in = byte|0x100;

  do
  {
    crc <<= 1;
    in <<= 1;

    if(in&0x100)
    {
      ++crc;
    }
    
    if(crc&0x10000)
    {
      crc ^= 0x1021;
    }
 } while(!(in&0x10000));

 return (crc&0xffffu);
}

/**
  * @brief  Cal CRC16 for YModem Packet
  * @param  data
  * @param  length
  * @retval CRC value
  */
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
  uint32_t crc = 0;
  const uint8_t* dataEnd = data+size;
  
  while(data<dataEnd)
  {
    crc = UpdateCRC16(crc,*data++);
  }
  crc = UpdateCRC16(crc,0);
  crc = UpdateCRC16(crc,0);

  return (crc&0xffffu);
}

/**
  * @brief  Cal Check sum for YModem Packet
  * @param  data
  * @param  length
  * @retval None
  */
uint8_t CalChecksum(const uint8_t* data, uint32_t size)
{
  uint32_t sum = 0;
  const uint8_t* dataEnd = data+size;
 
  while(data < dataEnd)
  {
    sum += *data++;
  }

 return (sum&0xffu);
}

/**
  * @brief  Receive a packet from sender
  * @param  data
  * @param  length
  * @param  timeout
  *          0: end of transmission
  *          -1: abort by sender
  *          >0: packet length
  * @retval 0: normally return
  *         -1: timeout or packet error
  *         1: abort by user
  */
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
  uint16_t i, packet_size, computedcrc;
  uint8_t c;
  *length = 0;
  if (Receive_Byte(&c, timeout) != 0)//返回0成功收取数据,-1为没收到数据
  {
    return -1;
  }
  switch (c)
  {
    case SOH:
      packet_size = PACKET_SIZE;
      break;
    case STX:
      packet_size = PACKET_1K_SIZE;
      break;
    case EOT:
      return 0;
    case CA:
      if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
      {
        *length = -1;
        return 0;
      }
      else
      {
        return -1;
      }
    case ABORT1:
    case ABORT2:
      return 1;
    default:
      return -1;
  }
  *data = c;//放到1024+5字节的包里面
  for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
  {
    if (Receive_Byte(data + i, timeout) != 0)//不断的接收1024字节放入
    {
      return -1;
    }
  }
	//检测序号补码是是否正确
  if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
  {
    return -1;
  }

  /* Compute the CRC */
  computedcrc = Cal_CRC16(&data[PACKET_HEADER], (uint32_t)packet_size);//计算CRC16码
  /* Check that received CRC match the already computed CRC value
     data[packet_size+3]<<8) | data[packet_size+4] contains the received CRC 
     computedcrc contains the computed CRC value */
  if (computedcrc != (uint16_t)((data[packet_size+3]<<8) | data[packet_size+4]))//检测CRC16校验码
  {
    /* CRC error */
    return -1;
  }

  *length = packet_size;
  return 0;
}

/**
  * @brief  Receive a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */

int32_t Ymodem_Receive (uint8_t *buf)
{                    //1024 + 5
	uint8_t bufferOut[16]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
  uint8_t *BufferIn;
	int32_t i, j, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;
  uint32_t flashdestination, ramsource;

	aesDecInit();//AES解密初始化
	
  /* Initialize flashdestination variable */
  flashdestination = APPLICATION_ADDRESS; //APP代码的起始地址,APPLICATION_ADDRESS = 0x8005000,可在target界面根据情况设置
                                           //这些都在ymodem.h里面的宏进行设置

  
  for (session_done = 0, errors = 0, session_begin = 0; ;)//死循环直至文件数据包全部发送完成
  {
    for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
    {
			
      switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
      {
        case 0://成功接收到1K
          errors = 0;
          switch (packet_length)
          {
            /* Abort by sender */
            case - 1:  //接收失败
              Send_Byte(ACK);  //回复
              return 0;
            /* End of transmission */
            case 0:
              Send_Byte(ACK);//回复
              file_done = 1;
              break;
            /* Normal packet */
            default:   //接收成功
              if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
              {//序号00(文件名)
                Send_Byte(NAK);
								
              }
              else
              {
                if (packets_received == 0)//文件名(首包)
                {
                  /* Filename packet */
                  if (packet_data[PACKET_HEADER] != 0)//文件名字
                  {
                    /* Filename packet has valid data */
                    for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                    {
                      FileName[i++] = *file_ptr++;//保存文件名
                    }
                    FileName[i++] = '\0';//字符串形式
                    for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < (FILE_SIZE_LENGTH - 1));)
                    {
                      file_size[i++] = *file_ptr++;//文件大小
                    }
                    file_size[i++] = '\0';
                    Str2Int(file_size, &size);//Convert a string to an integer

                    /* Test the size of the image to be sent */
                    /* Image size is greater than Flash size */
                    if (size > (USER_FLASH_SIZE + 1))
                    {
                      /* End session */
                      Send_Byte(CA);
                      Send_Byte(CA);
                      return -1;
                    }
                    /* erase user application area */
										FLASH_Unlock();						//解锁
                    FLASH_If_Erase(APPLICATION_ADDRESS);//This function does an erase of all user flash area
										FLASH_Lock();             //上锁
										
										Send_Byte(ACK);
										Send_Byte(CRC16);
                  }
                  /* Filename packet is empty, end session */
                  else
                  {
                    Send_Byte(ACK);
                    file_done = 1;
                    session_done = 1;
                    break;
                  }
                }
                /* Data packet */
                else  //文件信息保存完后开始接收数据
                {
                  memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
									
									/*----------------------------------------------------------------------------------------------*/
									BufferIn=buf;
									for (j = 0; j < packet_length; j += 16) //每次解密16字节
									{
										//解密数据包
										aesDecrypt(BufferIn,bufferOut); //由于参数使用的是指针,所以解密后依旧存在buf里面
										BufferIn+=16;
									}
									/*----------------------------------------------------------------------------------------------*/
								
                  ramsource = (uint32_t)buf;
                  
                  /* Write received data in Flash */                                   //这个size参数是自己加进入的,便于判断文件传输完成 
                  if (FLASH_If_Write(&flashdestination, (uint32_t*) ramsource, (uint16_t) packet_length/4, size)  == 0)
                  {//写入FLASH  
                    Send_Byte(ACK);
                  }
                  else /* An error occurred while writing to Flash memory */
                  {
                    /* End session */
                    Send_Byte(CA);
                    Send_Byte(CA);
                    return -2;
                  }
                }
                packets_received ++;
                session_begin = 1;
              }
          }
          break;
        case 1:
          Send_Byte(CA);
          Send_Byte(CA);
          return -3;
        default://检验错误
          if (session_begin > 0)
          {
            errors ++;
          }
          if (errors > MAX_ERRORS)
          {
            Send_Byte(CA);
            Send_Byte(CA);
            return 0;
          }
          Send_Byte(CRC16); //发送校验值
          break;
      }
			
      if (file_done != 0)
      {
        break;
      }
    }
    if (session_done != 0) //文件发送完成
    {
      break;
    }
  }
  return (int32_t)size;
}

/**
  * @brief  check response using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int32_t Ymodem_CheckResponse(uint8_t c)
{
  return 0;
}

/**
  * @brief  Prepare the first block
  * @param  timeout
  * @retval None
  */
void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length)
{
  uint16_t i, j;
  uint8_t file_ptr[10];
  
  /* Make first three packet */
  data[0] = SOH;
  data[1] = 0x00;
  data[2] = 0xff;
  
  /* Filename packet has valid data */
  for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH);i++)
  {
     data[i + PACKET_HEADER] = fileName[i];
  }

  data[i + PACKET_HEADER] = 0x00;
  
  Int2Str (file_ptr, *length);
  for (j =0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0' ; )
  {
     data[i++] = file_ptr[j++];
  }
  
  for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
  {
    data[j] = 0;
  }
}

/**
  * @brief  Prepare the data packet
  * @param  timeout
  * @retval None
  */
void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk)
{
  uint16_t i, size, packetSize;
  uint8_t* file_ptr;
  
  /* Make first three packet */
  packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
  size = sizeBlk < packetSize ? sizeBlk :packetSize;
  if (packetSize == PACKET_1K_SIZE)
  {
     data[0] = STX;
  }
  else
  {
     data[0] = SOH;
  }
  data[1] = pktNo;
  data[2] = (~pktNo);
  file_ptr = SourceBuf;
  
  /* Filename packet has valid data */
  for (i = PACKET_HEADER; i < size + PACKET_HEADER;i++)
  {
     data[i] = *file_ptr++;
  }
  if ( size  <= packetSize)
  {
    for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
    {
      data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
    }
  }
}

/**
  * @brief  Transmit a data packet using the ymodem protocol
  * @param  data
  * @param  length
  * @retval None
  */
void Ymodem_SendPacket(uint8_t *data, uint16_t length)
{
  uint16_t i;
  i = 0;
  while (i < length)
  {
    Send_Byte(data[i]);
    i++;
  }
}

/**
  * @brief  Transmit a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
uint8_t Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile)
{
  uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
  uint8_t FileName[FILE_NAME_LENGTH];
  uint8_t *buf_ptr, tempCheckSum ;
  uint16_t tempCRC, blkNumber;
  uint8_t receivedC[2], CRC16_F = 0, i;
  uint32_t errors = 0, ackReceived = 0, size = 0, pktSize;

  for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
  {
    FileName[i] = sendFileName[i];
  }
  CRC16_F = 1;
  
  /* Prepare first block */
  Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);
  
  do 
  {
    /* Send Packet */
    Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
    
    /* Send CRC or Check Sum based on CRC16_F */
    if (CRC16_F)
    {
       tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
       Send_Byte(tempCRC >> 8);
       Send_Byte(tempCRC & 0xFF);
    }
    else
    {
       tempCheckSum = CalChecksum (&packet_data[3], PACKET_SIZE);
       Send_Byte(tempCheckSum);
    }
  
    /* Wait for Ack and 'C' */
    if (Receive_Byte(&receivedC[0], 1000000) == 0)  
    {
      if (receivedC[0] == ACK)
      { 
        /* Packet transfered correctly */
        ackReceived = 1;
      }
    }
    else
    {
        errors++;
    }
  }while (!ackReceived && (errors < 0x0A));//校验错误重发
  
  if (errors >=  0x0A)
  {
    return errors;
  }
  buf_ptr = buf;
  size = sizeFile;
  blkNumber = 0x01;

  /* Here 1024 bytes package is used to send the packets */
  while (size)
  {
    /* Prepare next packet */
    Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
    ackReceived = 0;
    receivedC[0]= 0;
    errors = 0;
    do
    {
      /* Send next packet */
      if (size >= PACKET_1K_SIZE)
      {
        pktSize = PACKET_1K_SIZE;
       
      }
      else
      {
        pktSize = PACKET_SIZE;
      }
      Ymodem_SendPacket(packet_data, pktSize + PACKET_HEADER);
      /* Send CRC or Check Sum based on CRC16_F */
      if (CRC16_F)
      {
         tempCRC = Cal_CRC16(&packet_data[3], pktSize);
         Send_Byte(tempCRC >> 8);
         Send_Byte(tempCRC & 0xFF);
      }
      else
      {
        tempCheckSum = CalChecksum (&packet_data[3], pktSize);
        Send_Byte(tempCheckSum);
      }
      
      /* Wait for Ack */
      if (Receive_Byte(&receivedC[0], 1000000) == 0)  
      {    if (receivedC[0] == ACK)
      {
        ackReceived = 1;  
        if (size > pktSize)
        {
           buf_ptr += pktSize;  
           size -= pktSize;
           if (blkNumber == (USER_FLASH_SIZE/1024))
           {
             return 0xFF; /*  error */
           }
           else
           {
              blkNumber++;
           }
        }
        else
        {
           buf_ptr += pktSize;
           size = 0;
        }
      }
      }
      else
      {
        errors++;
      }
    }while(!ackReceived && (errors < 0x0A));
    
    /* Resend packet if NAK  for a count of 10 else end of commuincation */
    if (errors >=  0x0A)
    {
      return errors;
    }
    
  }
  ackReceived = 0;
  receivedC[0] = 0x00;
  receivedC[1] = 0x00;
  errors = 0;
  do 
  { 
    Send_Byte(EOT);   
    /* Send (EOT); */
    /* Wait for Ack */
    receivedC[0] = USART_ReceiveData(USART1);
    if (receivedC[0] == ACK)
    {
      ackReceived = 1;
      }
    
    else
    {
      errors++;
    }
    /* Clear Overrun flag of the USART2 */
    USART_ClearFlag(USART1, USART_FLAG_ORE);
  }while (!ackReceived && (errors < 0x0A));
    
  if (errors >=  0x0A)
  {
    return errors;
  }
  
  /* Last packet preparation */
  ackReceived = 0;
  receivedC[0] = 0x00;
  receivedC[1] = 0x00;
  errors = 0;

  packet_data[0] = SOH;
  packet_data[1] = 0;
  packet_data [2] = 0xFF;

  for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
  {
     packet_data [i] = 0x00;
  }
  
  do 
  {
    /* Send Packet */
    Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);

    /* Send CRC or Check Sum based on CRC16_F */
    tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
    Send_Byte(tempCRC >> 8);
    Send_Byte(tempCRC & 0xFF);
  
    /* Wait for Ack and 'C' */
    if (Receive_Byte(&receivedC[1], 1000000) == 0)  
    {
      if (receivedC[1] == ACK)
      { 
        /* Packet transfered correctly */
        ackReceived = 1;
      }
    }
    else
    {
      errors++;
    }
  }while (!ackReceived && (errors < 0x0A));
  
  /* Resend packet if NAK  for a count of 10  else end of commuincation */
  if (errors >=  0x0A)
  {
    return errors;
  }  
  receivedC[0] = 0x00;
  do 
  { 
    Send_Byte(EOT);   
    /* Send (EOT); */
    /* Wait for Ack */
    if ((Receive_Byte(&receivedC[0], 1000000) == 0)  && receivedC[0] == ACK)
    {
      ackReceived = 1;  
    }
    
    else
    {
      errors++;
    }
    /* Clear Overrun flag of the USART2 */
    USART_ClearFlag(USART1, USART_FLAG_ORE);
  }while (!ackReceived && (errors < 0x0A));
    
  if (errors >=  0x0A)
  {
    return errors;
  }
  return 0; /* file trasmitted successfully */
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
