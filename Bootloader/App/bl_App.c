#include "Rte.h"
#include "Cos.h"
#include "CosTask.h"
#include "stm32f10x.h"

int main()
{
	Rte_Init();
	App_Task_Init();
	for(;;)
	{
		CosTask();
	}
	


}


