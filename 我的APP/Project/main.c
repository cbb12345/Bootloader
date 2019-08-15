/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：main.c
 * 描述    :循环点亮LED灯    
 * 库版本  ：V1.3.0
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2015-12-20
 * 调试方式：J-Link-OB
**********************************************************************************/	

#include "led.h"
#include "usart1.h"
#include "stmflash.h"

#define APPLICATION_ADDRESS     ((uint32_t)0x08005000)

void IAP_Set()
{
   uint32_t i = 0;

/* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  

  /* Copy the vector table from the Flash (mapped at the base of the application
     load address 0x08005000) to the base address of the SRAM at 0x20000000. */
	
  for(i = 0; i < 48; i++)//先将”应用程序“的中断向量表拷贝至SRAM中
  {
    *((uint32_t*)(0x20000000 + (i << 2)))=*(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}

  /* Enable the SYSCFG peripheral clock*/ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
	
	/* Remap SRAM at 0x00000000 */
	
	/*然后在配置SYSCFG寄存器的最低两位，将SRMA映射至地址0，这样，在发生中断后，CPU从地址0取中断向量，
	这样实际上就是从SRAM中取的中断向量，而SRAM中的中断向量表又是之前从”应用程序“的中断向量表拷贝过
	来的，所以，最终其实是取的”应用程序“的中断向量表。*/
  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
}	

//函数声明

void Delay_ms( uint16_t time_ms );
/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*初始化LED端口*/
	IAP_Set();
  LED_GPIO_Config();
	USART1_Init(57600);
	LED1_ONOFF(Bit_SET);
  while(1)
  {	
		if(updata_pro==1)
		{
			updata_pro = 0;
			//写flash置位标志,写入0xAAAA
			printf("收到加密文件开始升级.....\r\n");
		  STMFLASH_Write_NoCheck(0x8004A38,0xAAAA);
			Delay_ms(100);
			SCB->AIRCR = 0X05FA0000|(unsigned int)0x04; //软复位回到bootloader   
		}
    LED1_ONOFF(Bit_SET);
    Delay_ms(100);
    LED1_ONOFF(Bit_RESET);
    Delay_ms(100);
  }		
}

 /**
  * @file   Delay_ms
  * @brief  毫秒延时time_ms ms
  * @param   time_ms 延时时间
  * @retval 无
  */
void Delay_ms( uint16_t time_ms )
{
  uint16_t i,j;
  for( i=0;i<time_ms;i++ )
  {
		for( j=0;j<4784;j++ );
  }
}
