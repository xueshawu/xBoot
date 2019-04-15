#include "Bsp_Gpio.h"
#include "stm32f10x_gpio.h"






static void  Gpio_FaultLamp_Init(void)
{
	GPIO_InitTypeDef	gpio_InitConfig;
	gpio_InitConfig.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_InitConfig.GPIO_Pin = GPIO_Pin_5;
	gpio_InitConfig.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_InitConfig);
}


static void Gpio_RunningLamp_Init(void)
{
	GPIO_InitTypeDef	gpio_InitConfig;
	gpio_InitConfig.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_InitConfig.GPIO_Pin = GPIO_Pin_5;
	gpio_InitConfig.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpio_InitConfig);	
}













void Bsp_Gpio_Init(void)
{
	DioChannelType chn = GPIOFSM_INIT;

	RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	
	while(chn != GPIOFSM_PENDING )
	{
		switch(chn)
		{
			case FAULT_LAMP :
				Gpio_FaultLamp_Init();
				chn = RUNNING_LAMP;
				break;
			case RUNNING_LAMP :
				Gpio_RunningLamp_Init();
				chn = GPIOFSM_PENDING;
				break;
			default : 
				chn = GPIOFSM_PENDING;
				break;
			
		}
	}
}
void Bsp_Gpio_DioWrtie(DioChannelType Channel)
{
	switch(Channel)
	{
		case FAULT_LAMP:
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, 1);
			break;
		case RUNNING_LAMP:
			GPIO_WriteBit(GPIOE, GPIO_Pin_5, 1);
			break;
		default :
			break;
		
	}

}



void Bsp_Gpio_DioRead(DioChannelType Channel)
{
	
}










