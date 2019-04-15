
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "Rte_Queque.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
Queque_StdRet Queque_Insert(Queque_T *queque,Queque_EleType element)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Queque_IsFull(queque))
	{
		return QUEQUE_FULL;
	}
	queque->rear = (queque->rear+1)&QUEQUE_SIZE;
	queque->quequeBuf[queque->rear] = element;
	return QUEQUE_OK;
	
}
//获取队列的头元素，并且插入新元素
Queque_StdRet Queque_Push(Queque_T *queque,Queque_EleType *element)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(element == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Queque_IsEmpty(queque))
	{
		return QUEQUE_EMPTY;
	}
	Queque_Delete(queque);
	Queque_Insert(queque,*element);
	return QUEQUE_OK;
}


Queque_StdRet Queque_Delete(Queque_T *queque)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Queque_IsEmpty(queque))
	{
		return QUEQUE_EMPTY;
	}
	queque->front = (queque->front+1)%QUEQUE_SIZE;
	return QUEQUE_OK;
}



Queque_StdRet Queque_Clear(Queque_T *queque)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	queque->front = 1;
	queque->rear = 0;
	return QUEQUE_OK;
	
}



Queque_Bool  Queque_IsEmpty(Queque_T *queque)
{
	return (queque->rear + 1)%QUEQUE_SIZE == queque->front;
}



Queque_Bool  Queque_IsFull(Queque_T *queque)
{
	return (queque->rear + 2)%QUEQUE_SIZE == queque->front;
}



Queque_StdRet Queque_Getength(Queque_T *queque)
{
	return (queque->front );
}



Queque_StdRet Queque_GetHead(Queque_T *queque,Queque_EleType *element)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Queque_IsEmpty(queque))
	{
			return QUEQUE_EMPTY;
	}
	return queque->quequeBuf[queque->front];
}


Queque_StdRet Queque_TransmitToBuffer(Queque_T *queque,Queque_EleType *elementBuf)
{
	uint8 front_index = queque->front;
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(elementBuf == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	while(front_index != (queque->rear + 2)%QUEQUE_SIZE)//检查空
	{
		*elementBuf = queque->quequeBuf[front_index];
		elementBuf++;
		front_index = ((front_index+1)%QUEQUE_SIZE);
		
	}
	return QUEQUE_OK;
}


