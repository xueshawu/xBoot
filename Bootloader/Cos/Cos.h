#ifndef __COS_H
#define __COS_H



/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Rte_PlatformType.h"








/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */
 
 #ifndef APP_TASK_INIT_PRIORITY	
	#define		APP_TASK_INIT_PRIORITY				0
#else
	#error "APP_TASK_INIT_PRIORITY	 has been define"
#endif

#ifndef APP_TASK_PRIORITY_1
	#define		APP_TASK_PRIORITY_1					1
#else
	#error	"APP_TASK_PRIORITY_1 has been define"
#endif

#ifndef APP_TASK_PRIORITY_2
	#define		APP_TASK_PRIORITY_2					2
#else
	#error	"APP_TASK_PRIORITY_2 has been define"
#endif

#ifndef	APP_TASK_PRIORITY_3
	#define		APP_TASK_PRIORITY_3					3
#else
	#error	"APP_TASK_PRIORITY_3 has been define"
#endif

#ifndef APP_TASK_PRIORITY_4	
	#define 	APP_TASK_PRIORITY					4
#else
	#error	"APP_TASK_PRIORITY_4"
#endif	

#ifndef APP_TASK_PRIORITY_5
	#define		APP_TASK_PRIORITY_5					5
#else
	#error	"APP_TASK_PRIORITY_5 has been define"
#endif	
	
#ifndef APP_TASK_PRIORITY_6	
	#define		APP_TASK_PRIORITY_6					6
#else
	#error	"APP_TASK_PRIORITY_6 has been define"	
#endif	
	
#ifndef APP_TASK_PRIORITY_7	
	#define		APP_TASK_PRIORITY_7					7
#else
	#error	"APP_TASK_PRIORITY_7 has been define"
#endif

#ifndef APP_TASK_PRIORITY_8
	#define APP_TASK_PRIORITY_8						8
#else
	#error "APP_TASK_PRIORITY_8 has been define"
#endif	

#ifndef APP_TASK_PRIORITY_9
	#define	APP_TASK_PRIORITY_9						9
#else
	#error	"APP_TASK_PRIORITY_9 has been define"
#endif

#ifndef APP_TASK_PRIORITY_10
	#define	APP_TASK_PRIORITY_10
#else
	#error	"APP_TASK_PRIORITY_10 has been define"
#endif

#define		APP_TASK_MIN_PRIORITY					10


#define		TASK_1MS_OFFSET							1
#define		TASK_5MS_OFFSET							5
#define		TASK_10MS_OFFSET						10
#define		TASK_50MS_OFFSET						50
#define		TASK_100MS_OFFSET						100
#define		TASK_500MS_OFFSET						500
#define		TASK_1000MS_OFFSET						1000




 
/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef struct
{
	unsigned short CosEn;
	unsigned short CosRdy;
	unsigned int   CosCtr;
	void (*CosFnct)(void *);
	void * CosFnctArg;
}CosStruct;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

extern void CosInit(void);
extern unsigned int CosGetCounter(void);
extern void CosCofigFnct(uint8 Priority, void(* Fnct)(void *),void *FnctArg);
extern void CosStart(uint8 Priority,uint16 PosPeriod);
extern void CosStop(uint16 Priority);
extern void CosRefresh(void);
extern void CosTask(void);















#else
	#error "Cos.h has been define"

#endif
/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/

