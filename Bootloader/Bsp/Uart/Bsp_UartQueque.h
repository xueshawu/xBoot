#ifndef _BSP_UARTQUEQUE_H
#define _BSP_UARTQUEQUE_H

#include "Bsp_UartType.h"




/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */
#define 	QUEQUE_SIZE			2*sizeof(MsgStrcutType)
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
}QuequeStructType;


typedef boolean Queque_Bool;
typedef uint8	Queque_StdRet;
typedef uint16	Queque_EleType;



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern Queque_StdRet Uart_Queque_Init(QuequeStructType *queue);
extern Queque_StdRet Uart_Queque_Insert(QuequeStructType *queue,Queque_EleType element);
extern Queque_StdRet Uart_Queque_Push(QuequeStructType *queue,Queque_EleType *element);
extern Queque_StdRet Uart_Queque_Delete(QuequeStructType *queue);
extern Queque_StdRet Uart_Queque_Clear(QuequeStructType *queue);
extern Queque_StdRet Uart_Queue_IsEmpty(QuequeStructType *queue);
extern Queque_StdRet Uart_Queque_IsFull(QuequeStructType *queue);
extern Queque_StdRet Uart_Queque_GetLength(QuequeStructType *queue);
extern Queque_StdRet Uart_Queque_GetHead(QuequeStructType *queue,Queque_EleType *element);
extern Queque_StdRet Uart_Queque_TransmitToBuffer(QuequeStructType *queue,Queque_EleType *elementBuf);














#endif
/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/





