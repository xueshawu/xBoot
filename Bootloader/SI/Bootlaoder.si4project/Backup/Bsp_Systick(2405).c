#include "Bsp_Systick.h"
#include "Rte_PlatformType.h"
#include "stm32f10x.h"




void Bsp_Systick_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}


static void Bsp_Systick_Cbk(void)
{
	static boolean stLED = TRUE;
	GPIO_WriteBit(GPIOB, GPIO_Pin_5, stLED);
	stLED = !stLED;
}



/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	Bsp_Systick_Cbk();
}








