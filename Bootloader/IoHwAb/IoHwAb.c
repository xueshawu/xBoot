
/*
 *********************************************************************************************************
 *                                            INCLUDE FILES
 *********************************************************************************************************
*/
#include "IoHwAb.h"
#include "Bsp_Gpio.h"




void IoHwAb_Init(void)
{
		
}


void IoHwAb_RunningLamp_MainFunction(void)
{
	static Dio_LevelType Level = STD_HIGH;
	Bsp_Gpio_Dio_Wrtie(RUNNING_LAMP,Level);
	Level = !Level;
}
void IoHwAb_FaultLamp_MainFunction(void)
{
	static Dio_LevelType Level = STD_HIGH;
	Bsp_Gpio_Dio_Wrtie(FAULT_LAMP,Level);
	Level = !Level;
}


