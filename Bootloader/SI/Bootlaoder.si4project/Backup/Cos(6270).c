
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Cos.h"







/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define 	COS_MAX_NUM						10		/* 最大任务数 */

#define 	TASK_PENDING					0
#define 	TASK_READY						1
#define		TASK_RUNNING					2

#define		TASK_DISABLE					0
#define		TASK_ENABLE						1




#define	CosDisableAllInterrupt() do {__set_PRIMASK(0);} while(0)

#define	CosEnableAllInterrupt()	do {__set_PRIMASK(1);}while(0)


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
	unsigned short i = 0;
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
void CosStart(unsigned short Priority, unsigned int PosPeriod)
{
	CosStruct *pCosStructure;
	if(Priority <COS_MAX_NUM)
	{
		pCosStructure = &CosStructTab[0];
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
void CosStop(unsigned short Priority)
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
	unsigned short i;
	CosStruct *pCosStructure;
	pCosStructure = &CosStructTab[0];
	CosDisableAllInterrupt();
	CosCounter++;
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
	unsigned short i=0;
	CosStruct *pCosStructure;
	pCosStructure = &CosStructTab[0];
	CosDisableAllInterrupt(); /* 进入临界区 */
	for (i =0; i<COS_MAX_NUM; i++)
	{
		if(TASK_ENABLE == pCosStructure->CosEn)
		{
			if(TASK_READY== pCosStructure->CosRdy) .
			{
				pCosStructure->CosEn = 2;
				pCosStructure->CosRdy = TASK_RUNNING;
				CosEnableAllInterrupt();  /* 退出临界区 */
				if(pCosStructure->CosFnct != (void (*)(void *))0 )
				{
					(*pCosStructure->CosFnct)(pCosStructure->CosFnctArg);
				}
				CosDisableAllInterrupt();	/* 进入临界区 */
				if(pCosStructure-> == TASK_ENABLE)
				{
					pCosStructure->CosEn = TSAK_;

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
void CosCofigFnct(unsigned short Priority, void(* Fnct)(void *),void *FnctArg)
{
	CosStruct *pCosStructure;
	if(Priority<COS_MAX_NUM)
	{
		pCosStructure = &CosStructTab[0];
		CosDisableAllInterrupt();   /* 进入临界区 */
		pCosStructure->CosFnct = Fnct;
		pCosStructure->CosFnctArg = FnctArg;
		CosEnableAllInterrupt();	/* 退出临界区 */
	}
}







