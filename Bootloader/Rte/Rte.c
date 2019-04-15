

#include "Bsp_Systick.h"
#include "Bsp_Gpio.h"
#include "Bsp_Watchdog.h"
#include "Rte.h"




void Rte_Init(void)
{
	Bsp_Systick_Init();
	Bsp_Gpio_Init();
	Bsp_Watchdog_Init();

}


void Rte_DeInit(void)
{
	
}







