#include "usart1.h"
#include "string.h"
#include "led.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
};

/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout; 

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=ch;
  return (ch);
}
#endif 


/*************************************************************************
* 描  述: 蓝牙串口初始化函数
* 输  入: 串口波特率
* 输  出：无
**************************************************************************/
void USART1_Init(uint16_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽方式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = baud;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
	USART_Cmd(USART1, ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);				
}


/*************************************************************************
* 描  述: 蓝牙串口处理函数
* 输  入: 无
* 输  出：无
**************************************************************************/
volatile unsigned char updata_pro = 0;

void USART1_IRQHandler(void)
{
	unsigned char buffer = 0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		buffer = USART_ReceiveData(USART1);
		
		if(buffer=='1')
		{
			updata_pro = 1;
		}
	}		
}


