#include "usart.h"
#include "flash_if.h"
#include "led.h"
#include "menu.h"
#include "aes.h"

//1.���APP����Bootlaoder,ȫ�ֱ����Ƿ�ͨ�ã�
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;

 /**
  * @file   Delay_ms
  * @brief  ������ʱtime_ms ms
  * @param   time_ms ��ʱʱ��
  * @retval ��
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

		updata_byte = STMFLASH_ReadHalfWord(0x8004A38);//��ȡָ����ַ��ֵ
		if(updata_byte == 0xAAAA)//�жϴ˵�ַ��ֵ�Ƿ�Ϊ0xAAAA,�����������
		{
			Main_Menu();
		}
		else  //��������ת��APP,ע��F0������ϵ�е���ת��Щ��һ��,��ת���ֿɲο�ԭ�Ӹ����
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








