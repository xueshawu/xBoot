
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32f10x.h"   /* must include frist for include core_cm3.h */
#include "Cos.h"











/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#ifndef COS_MAX_NUM
	#define 	COS_MAX_NUM						10		/* 最大任务数 */
#else
	#error	"COS_MAX_NUM has been define"
#endif


#ifndef TASK_PEDNDING
	#define 	TASK_PENDING					0		/* Task is pending */
#else
	#error 	"TASK_PEDDING has been define"					
#endif

#ifndef TASK_READY
	#define 	TASK_READY						1		/* Task is Ready*/
#else
	#error	"TASK_READY has been define"
#endif

#ifndef TASK_RUNNING
	#define		TASK_RUNNING					2
#else
	#error "TASK_RUNNING has been define"				/* Task is Running */

#endif


#ifndef TASK_DISABLE
	#define		TASK_DISABLE					0
#else
	#error	"TASK_DISABLE has been define"				/* Task is Disable*/
#endif	

#ifndef TASK_ENABLE
	#define		TASK_ENABLE						1
#else
	#error	"TASK_ENABLE has been define"
#endif







/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static CosStruct	CosStructTab[COS_MAX_NUM];


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

unsigned int CosCounter = 0;
static unsigned int g_SysCounter =0;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*函数名  ： CosInit
*函数功能 ：初始化Cos配置参数
*函数返回值 ： none
*创建日期 ： 18/09/17
*修改日期 ：
*修改人 ：
*修改原因 ：
*版本 ：V1.0
*历史版本
*********************************************************************************************************
*/
void CosInit(void)
{
	uint8 i = 0;
	CosStruct	*pCosStructure;
	
	pCosStructure = &CosStructTab[0];
	CosCounter =0;
	for(i=0; i<COS_MAX_NUM; i++)
	{
		pCosStructure->CosEn = TASK_DISABLE;
		pCosStructure->CosCtr = 0xFFFFFFFF;
		pCosStructure->CosRdy = TASK_PENDING;
		pCosStructure++;
	}
	
}



 unsigned int CosGetCounter(void)
{
	return g_SysCounter;
}



/*
*********************************************************************************************************
*函数名  ： CosStart
*函数功能 ：设定任务优先级与执行周期
*函数返回值 ： none
*创建日期 ： 18/09/17
*修改日期 ：
*修改人 ：
*修改原因 ：
*版本 ：V1.0
*历史版本
*********************************************************************************************************
*/
void CosStart(uint8 Priority, uint16 PosPeriod)
{
	CosStruct *pCosStructure;
	if(Priority <COS_MAX_NUM)
	{
		pCosStructure = &CosStructTab[Priority];//根据优先级查找对应的函数指针
		CosDisableAllInterrupt();
		pCosStructure->CosCtr = PosPeriod+CosCounter;
		pCosStructure->CosEn = TASK_ENABLE;
		if(PosPeriod == 0)
		{
			pCosStructure->CosRdy = TASK_READY;
		}
		else
		{
			pCosStructure->CosRdy = TASK_PENDING;
		}
		CosEnableAllInterrupt();
	}
}




/*
*********************************************************************************************************
*函数名  ： CosStop
*函数功能 ：设置任务为挂起状态
*函数返回值 ： none
*创建日期 ： 18/09/17
*修改日期 ：
*修改人 ：
*修改原因 ：
*版本 ：V1.0
*历史版本
*********************************************************************************************************
*/
void CosStop(uint16 Priority)
{
	CosStruct *pCosStructure;
	if(Priority< COS_MAX_NUM)
	{
		pCosStructure = &CosStructTab[0];
		CosDisableAllInterrupt();
		pCosStructure->CosEn = TASK_DISABLE;
		pCosStructure->CosRdy = TASK_PENDING;
		pCosStructure->CosCtr = 0xFFFFFFFF;
		CosEnableAllInterrupt();
		
	}
}


/*
*********************************************************************************************************
*函数名  ： CosRefresh
*函数功能 ：设定任务优先级与执行周期
*函数返回值 ： none
*创建日期 ： 18/09/17
*修改日期 ：
*修改人 ：
*修改原因 ：
*版本 ：V1.0
*历史版本
*********************************************************************************************************
*/

void CosRefresh(void)
{
	uint8 i;
	CosStruct *pCosStructure;
	pCosStructure = &CosStructTab[0];
	CosDisableAllInterrupt();
	CosCounter++;
	g_SysCounter++;
	for(i=0; i<COS_MAX_NUM; i++)
	{
		if (TASK_ENABLE == pCosStructure->CosEn)
		{
			if(pCosStructure->CosCtr<= CosCounter)
			{
				pCosStructure->CosRdy =TASK_READY;
			}
		}
		pCosStructure++;
	}
	CosEnableAllInterrupt();
}


/*
*********************************************************************************************************
*函数名  ： CosTask
*函数功能 ：按优先级执行任务
*函数返回值 ： none
*创建日期 ： 18/09/17
*修改日期 ：
*修改人 ：
*修改原因 ：
*版本 ：V1.0
*历史版本
*********************************************************************************************************
*/
void CosTask(void)
{
	uint8 i=0;
	CosStruct *pCosStructure;
	pCosStructure = &CosStructTab[0];
	CosDisableAllInterrupt(); /* 进入临界区 */
	for (i =0; i<COS_MAX_NUM; i++)
	{
		if(TASK_ENABLE == pCosStructure->CosEn)
		{
			if(TASK_READY== pCosStructure->CosRdy)
			{
				pCosStructure->CosEn = TASK_ENABLE ;
				pCosStructure->CosRdy = TASK_RUNNING;
				CosEnableAllInterrupt();  /* 退出临界区 */
				if(pCosStructure->CosFnct != (void (*)(void *))0 )
				{
					(*pCosStructure->CosFnct)(pCosStructure->CosFnctArg);
				}
				CosDisableAllInterrupt();	/* 进入临界区 */
				if(pCosStructure->CosRdy == TASK_RUNNING)
				{
					pCosStructure->CosEn = TASK_PENDING;

				}
			}
		}
		pCosStructure++;
	}
	CosEnableAllInterrupt(); /* 退出临界区 */
}


/*
*********************************************************************************************************
*函数名  ： CosRefresh
*函数功能 ：设定任务优先级与执行周期
*函数返回值 ： none
*创建日期 ： 18/09/17
*修改日期 ：
*修改人 ：
*修改原因 ：
*版本 ：V1.0
*历史版本
*********************************************************************************************************
*/
void CosCofigFnct(uint8 Priority, void(* Fnct)(void *),void *FnctArg)
{
	CosStruct *pCosStructure;
	if(Priority<COS_MAX_NUM)
	{
		pCosStructure = &CosStructTab[Priority];
		CosDisableAllInterrupt();   /* 进入临界区 */
		pCosStructure->CosFnct = Fnct;
		pCosStructure->CosFnctArg = FnctArg;
		CosEnableAllInterrupt();	/* 退出临界区 */
	}
}







