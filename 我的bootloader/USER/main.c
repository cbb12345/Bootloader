#include "usart.h"
#include "flash_if.h"
#include "led.h"
#include "menu.h"
#include "aes.h"

//1.解决APP回跳Bootlaoder,全局变量是否通用？
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

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

int main(void)
{
	unsigned short int updata_byte = 0;
	
	unsigned char i=0;
	
	LED_GPIO_Config();
	USART1_Init(57600);
	LED1_ONOFF(Bit_RESET);

	while(1)
	{

		updata_byte = STMFLASH_ReadHalfWord(0x8004A38);//读取指定地址的值
		if(updata_byte == 0xAAAA)//判断此地址的值是否为0xAAAA,如果是则升级
		{
			Main_Menu();
		}
		else  //不是则跳转到APP,注意F0和其他系列的跳转有些不一样,跳转部分可参考原子哥代码
		{	
			Delay_ms(5000);
			/* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
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
		}
	}
}








