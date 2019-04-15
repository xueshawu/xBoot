
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Cos.h"







/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/



#define	CosDisableAllInterrupt() do {__asm volatile ("cpsid i" : : : "memory");} while(0)

#define	CosEnableAllInterrupt()	do {__asm volatile ("cpsie i" : : : "memory");}while(0)


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
		pCosStructure->CosEn = 0;
		pCosStructure->CosCtr = 0xFFFFFFFF;
		pCosStructure->CosRdy = 0;
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
		pCosStructure->CosEn = 1;
		if(PosPeriod == 0)
		{
			pCosStructure->CosRdy = 1;
		}
		else
		{
			pCosStructure->CosRdy = 0;
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
		pCosStructure->CosEn = 0;
		pCosStructure->CosRdy = 0;
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
		if (1 == pCosStructure->CosEn)
		{
			if(pCosStructure->CosCtr<= CosCounter)
			{
				pCosStructure->CosRdy =1;
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
	CosDisableAllInterrupt();
	for (i =0; i<COS_MAX_NUM; i++)
	{
		if(1 == pCosStructure->CosRdy)
		{
			if(1== pCosStructure->CosEn)
			{
				pCosStructure->CosEn = 2;
				pCosStructure->CosRdy =0;
				CosEnableAllInterrupt();
				if(pCosStructure->CosFnct != (void (*)(void *))0 )
				{
					(*pCosStructure->CosFnct)(pCosStructure->CosFnctArg);
				}
				CosDisableAllInterrupt();
				if(pCosStructure->CosEn ==2)
				{
					pCosStructure->CosEn = 0;
					CosEnableAllInterrupt();
					return;
				}
			}
		}
		pCosStructure++;
	}
	CosEnableAllInterrupt();
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
		CosDisableAllInterrupt();
		pCosStructure->CosFnct = Fnct;
		pCosStructure->CosFnctArg = FnctArg;
		CosEnableAllInterrupt();
	}
}







