#include "Bsp_Systick.h"
#include "Cos.h"
#include "stm32f10x.h"





void Bsp_Systick_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
}


static void Bsp_Systick_Cbk(void)
{
	CosRefresh();

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








