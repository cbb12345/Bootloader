#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

#define LED1_GPIO_RCC           RCC_AHBPeriph_GPIOC
#define LED1_GPIO_PORT          GPIOC
#define LED1_GPIO_PIN      			GPIO_Pin_13
#define LED1_ONOFF(x)     			GPIO_WriteBit(GPIOC,GPIO_Pin_13,x);

void LED_GPIO_Config(void);	

#endif
