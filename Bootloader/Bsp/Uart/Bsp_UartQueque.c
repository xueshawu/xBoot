
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "Bsp_UartQueque.h"



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

Queque_StdRet Uart_Queque_Init(QuequeStructType *queue)
{
	uint8 loopcnt;
	queue->front = 1;
	queue->rear = 0;
	for (loopcnt=0;loopcnt<ARRAY_SIZE;loopcnt++)
	{
		queue->quequeBuf[loopcnt] = 0x0;
	}

	
}


Queque_StdRet Uart_Queque_Insert(QuequeStructType *queue,Queque_EleType element)
{
	if(queue == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queque_IsFull(queue))
	{
		return QUEQUE_FULL;
	}
	queue->rear = (queue->rear+1)&QUEQUE_SIZE;
	queue->quequeBuf[queue->rear] = element;
	return QUEQUE_OK;
	
}

Queque_StdRet Uart_Queque_Push(QuequeStructType *queue,Queque_EleType *element)
{
	if(queue == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(element == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queue_IsEmpty(queue))
	{
		return QUEQUE_EMPTY;
	}
	Uart_Queque_Delete(queue);
	Uart_Queque_Insert(queue,*element);
	return QUEQUE_OK;
}


Queque_StdRet Uart_Queque_Delete(QuequeStructType *queue)
{
	if(queue == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queue_IsEmpty(queue))
	{
		return QUEQUE_EMPTY;
	}
	queue->front = (queue->front+1)%QUEQUE_SIZE;
	return QUEQUE_OK;
}



Queque_StdRet Uart_Queque_Clear(QuequeStructType *queue)
{
	if(queue == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	queue->front = 1;
	queue->rear = 0;
	return QUEQUE_OK;
	
}



Queque_Bool  Uart_Queue_IsEmpty(QuequeStructType *queue)
{
	return (queue->rear + 1)%QUEQUE_SIZE == queue->front;
}



Queque_Bool  Uart_Queque_IsFull(QuequeStructType *queue)
{
	return (queue->rear + 2)%QUEQUE_SIZE == queue->front;
}



Queque_StdRet Uart_Queque_Getength(QuequeStructType *queue)
{
	return (queue->front );
}



Queque_StdRet Uart_Queque_GetHead(QuequeStructType *queue,Queque_EleType *element)
{
	if(queue == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queue_IsEmpty(queue))
	{
			return QUEQUE_EMPTY;
	}
	return queue->quequeBuf[queue->front];
}


Queque_StdRet Uart_Queque_TransmitToBuffer(QuequeStructType *queue,Queque_EleType *elementBuf)
{
	uint8 front_index = queue->front;
	if(queue == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(elementBuf == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	while(front_index != (queue->rear + 2)%QUEQUE_SIZE)//����
	{
		*elementBuf = queue->quequeBuf[front_index];
		elementBuf++;
		front_index = ((front_index+1)%QUEQUE_SIZE);
		
	}
	return QUEQUE_OK;
}




