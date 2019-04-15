#ifndef __COS_H
#define __COS_H






/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */

#define 	COS_MAX_NUM						10



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

void CosInit(void);
void CosCofigFnct(unsigned short Priority, void(* Fnct)(void *),void *FnctArg);
void CosStart(unsigned short Priority,unsigned int PosPeriod);
void CosStop(unsigned short Priority);
void CosRefresh(void);
void PosInit(void);




















#else
	#error 



#endif
/*
 ******************************************************************************
 *
 *end of the file
 ******************************************************************************
 */