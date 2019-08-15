#ifndef _USART_H
#define _USART_H

#include "stm32f0xx.h"
#include <stdio.h>


/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)             ((c >= 'A') && (c <= 'F'))
#define IS_af(c)             ((c >= 'a') && (c <= 'f'))
#define IS_09(c)             ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)        IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)        IS_09(c)
#define CONVERTDEC(c)        (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)        (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))


#define SerialPutString(x)   Serial_PutString((uint8_t*)(x))

void USART1_Init(uint16_t baud);
void SerialPutChar(uint8_t c);
uint32_t SerialKeyPressed(uint8_t *key);
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);
void Int2Str(uint8_t* str, int32_t intnum);
void Serial_PutString(uint8_t *s);
uint8_t GetKey(void);


#endif


