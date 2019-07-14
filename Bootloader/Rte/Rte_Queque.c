
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
Queque_StdRet Queque_Insert(QuequeStructType *queque,Queque_EleType element)
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
//��ȡ���е�ͷԪ�أ����Ҳ�����Ԫ��
Queque_StdRet Queque_Push(QuequeStructType *queque,Queque_EleType *element)
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


Queque_StdRet Queque_Delete(QuequeStructType *queque)
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



Queque_StdRet Queque_Clear(QuequeStructType *queque)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	queque->front = 1;
	queque->rear = 0;
	return QUEQUE_OK;
	
}



Queque_Bool  Queque_IsEmpty(QuequeStructType *queque)
{
	return (queque->rear + 1)%QUEQUE_SIZE == queque->front;
}



Queque_Bool  Queque_IsFull(QuequeStructType *queque)
{
	return (queque->rear + 2)%QUEQUE_SIZE == queque->front;
}



Queque_StdRet Queque_Getength(QuequeStructType *queque)
{
	return (queque->front );
}



Queque_StdRet Queque_GetHead(QuequeStructType *queque,Queque_EleType *element)
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


Queque_StdRet Queque_TransmitToBuffer(QuequeStructType *queque,Queque_EleType *elementBuf)
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
	while(front_index != (queque->rear + 2)%QUEQUE_SIZE)//����
	{
		*elementBuf = queque->quequeBuf[front_index];
		elementBuf++;
		front_index = ((front_index+1)%QUEQUE_SIZE);
		
	}
	return QUEQUE_OK;
}


