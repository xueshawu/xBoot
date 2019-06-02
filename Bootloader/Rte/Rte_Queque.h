#ifndef __RTE_QUEQUE_H
#define __RTE_QUEQUE_H
#include "Rte_PlatformType.h"


/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */
#define 	QUEQUE_SIZE			12
#define 	ARRAY_SIZE			(QUEQUE_SIZE+1)

#define 	QUEQUE_NULL_PTR		NULL_PTR


#define 	QUEQUE_OK			0
#define		QUEQUE_NOTOK		1
#define		QUEQUE_PARAM		0xE0
#define		QUEQUE_EMPTY		0xE1
#define		QUEQUE_FULL			0xE2

/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/
typedef struct
{
	uint8 front;
	uint8 rear;
	uint8 quequeBuf[ARRAY_SIZE];
}Queque_T;


typedef boolean Queque_Bool;
typedef uint8	Queque_StdRet;
typedef uint8	Queque_EleType;



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern Queque_StdRet Queque_Insert(Queque_T *queque,Queque_EleType element);
extern Queque_StdRet Queque_Push(Queque_T *queque,Queque_EleType *element);
extern Queque_StdRet Queque_Delete(Queque_T *queque);
extern Queque_StdRet Queque_Clear(Queque_T *queque);
extern Queque_StdRet Queque_IsEmpty(Queque_T *queque);
extern Queque_StdRet Queque_IsFull(Queque_T *queque);
extern Queque_StdRet Queque_GetLength(Queque_T *queque);
extern Queque_StdRet Queque_GetHead(Queque_T *queque,Queque_EleType *element);
extern Queque_StdRet Queque_TransmitToBuffer(Queque_T *queque,Queque_EleType *elementBuf);













#else
	#error "Rte_Queque.h has been define"

#endif
/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/







