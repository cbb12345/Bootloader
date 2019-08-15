/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��led.c
 * ����    ��LED����������     
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2015-12-20
 * Ӳ������: D1->PC13;D2->PB0;D4->PB1
 * ���Է�ʽ��J-Link-OB
**********************************************************************************/	


//ͷ�ļ�
#include "led.h"

 /**
  * @file   LED_GPIO_Config
  * @brief  LED����������
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{	
	//����һ��GPIO_InitTypeDef ���͵Ľṹ��
	GPIO_InitTypeDef  GPIO_InitStructure;
	//ʹ��GPIOC������ʱ��
	RCC_AHBPeriphClockCmd(LED1_GPIO_RCC,ENABLE);//ʹ��GPIO������ʱ��
	
	GPIO_InitStructure.GPIO_Pin =LED1_GPIO_PIN;	//ѡ��Ҫ�õ�GPIO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//��������Ϊ��ͨ���ģʽ				
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//��������Ϊ�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //��������Ϊ����		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���������ٶ�Ϊ50MHZ
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);//���ÿ⺯������ʼ��GPIO
	

	
}

 /**
  * @file   LEDXToggle
  * @brief  LED����ת
  * @param  ledx:1--PC13;2--PB0;1--PB1
  * @retval ��
  */
void LEDXToggle(uint8_t ledx)
{
  if(ledx==1)
  {
    LED1_GPIO_PORT->ODR^=LED1_GPIO_PIN;
  }
  
}
