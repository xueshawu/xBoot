/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_Gpio.h"
#include "stm32f10x_gpio.h"


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
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
	Dio_ChannelType chn = GPIOFSM_INIT;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);
	while(chn != GPIOFSM_PENDING )
	{
		switch(chn)
		{
			case GPIOFSM_INIT :
				chn = RUNNING_LAMP;
			
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
void Bsp_Gpio_Dio_Wrtie(Dio_ChannelType Channel,Dio_LevelType Level)
{
	switch(Channel)
	{
		case FAULT_LAMP:
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)Level);
			break;
		case RUNNING_LAMP:
			GPIO_WriteBit(GPIOE, GPIO_Pin_5, (BitAction)Level);
			break;
		default :
			break;
		
	}

}



Dio_LevelType Bsp_Gpio_Dio_Read(Dio_ChannelType Channel)
{
	/*
	switch(Channel)
	{
		case 
	}*/
	return STD_HIGH;
}










