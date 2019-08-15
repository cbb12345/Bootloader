/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��main.c
 * ����    :ѭ������LED��    
 * ��汾  ��V1.3.0
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-12-20
 * ���Է�ʽ��J-Link-OB
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
	
  for(i = 0; i < 48; i++)//�Ƚ���Ӧ�ó��򡰵��ж�����������SRAM��
  {
    *((uint32_t*)(0x20000000 + (i << 2)))=*(__IO uint32_t*)(APPLICATION_ADDRESS + (i<<2));
	}

  /* Enable the SYSCFG peripheral clock*/ 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
	
	/* Remap SRAM at 0x00000000 */
	
	/*Ȼ��������SYSCFG�Ĵ����������λ����SRMAӳ������ַ0���������ڷ����жϺ�CPU�ӵ�ַ0ȡ�ж�������
	����ʵ���Ͼ��Ǵ�SRAM��ȡ���ж���������SRAM�е��ж�����������֮ǰ�ӡ�Ӧ�ó��򡰵��ж�����������
	���ģ����ԣ�������ʵ��ȡ�ġ�Ӧ�ó��򡰵��ж�������*/
  SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
}	

//��������

void Delay_ms( uint16_t time_ms );
/**
  * @file   main
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*��ʼ��LED�˿�*/
	IAP_Set();
  LED_GPIO_Config();
	USART1_Init(57600);
	LED1_ONOFF(Bit_SET);
  while(1)
  {	
		if(updata_pro==1)
		{
			updata_pro = 0;
			//дflash��λ��־,д��0xAAAA
			printf("�յ������ļ���ʼ����.....\r\n");
		  STMFLASH_Write_NoCheck(0x8004A38,0xAAAA);
			Delay_ms(100);
			SCB->AIRCR = 0X05FA0000|(unsigned int)0x04; //��λ�ص�bootloader   
		}
    LED1_ONOFF(Bit_SET);
    Delay_ms(100);
    LED1_ONOFF(Bit_RESET);
    Delay_ms(100);
  }		
}

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
