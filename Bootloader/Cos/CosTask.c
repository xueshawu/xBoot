
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_Gpio.h"
#include "Bsp_Watchdog.h"
#include "Cos.h"
#include "CosTask.h"
#include "IoHwAb.h"




/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/







void App_Task_Init(void)
{
	CosInit();
	CosCofigFnct(APP_TASK_PRIORITY_1,App_Task_10Ms,0);
	CosCofigFnct(APP_TASK_PRIORITY_2,App_Task_50Ms,0);
	CosCofigFnct(APP_TASK_PRIORITY_3,App_Task_100Ms,0);
	
	CosStart(APP_TASK_PRIORITY_1,TASK_10MS_OFFSET);
	CosStart(APP_TASK_PRIORITY_2,TASK_50MS_OFFSET);
	CosStart(APP_TASK_PRIORITY_3,TASK_100MS_OFFSET);
}



void App_Task_10Ms(void *param)
{

	(void)param;
	IoHwAb_RunningLamp_MainFunction();
	CosStart(APP_TASK_PRIORITY_1,TASK_500MS_OFFSET);
	
}


void App_Task_50Ms(void *param)
{
	(void)param;
	Bsp_Watchdog_Feed();
	CosStart(APP_TASK_PRIORITY_2,TASK_50MS_OFFSET);	
}
void App_Task_100Ms(void *param)
{


	(void)param;
	IoHwAb_FaultLamp_MainFunction();
	CosStart(APP_TASK_PRIORITY_3,TASK_100MS_OFFSET);
	
}






