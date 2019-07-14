
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

Queque_StdRet Uart_Queque_Init(QuequeStructType *queque)
{
	uint8 loopcnt;
	queque->front = 1;
	queque->rear = 0;
	for (loopcnt=0;loopcnt<ARRAY_SIZE;loopcnt++)
	{
		queque->quequeBuf[loopcnt] = 0x0;
	}

	
}


Queque_StdRet Uart_Queque_Insert(QuequeStructType *queque,Queque_EleType element)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queque_IsFull(queque))
	{
		return QUEQUE_FULL;
	}
	queque->rear = (queque->rear+1)&QUEQUE_SIZE;
	queque->quequeBuf[queque->rear] = element;
	return QUEQUE_OK;
	
}

Queque_StdRet Uart_Queque_Push(QuequeStructType *queque,Queque_EleType *element)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(element == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queque_IsEmpty(queque))
	{
		return QUEQUE_EMPTY;
	}
	Uart_Queque_Delete(queque);
	Uart_Queque_Insert(queque,*element);
	return QUEQUE_OK;
}


Queque_StdRet Uart_Queque_Delete(QuequeStructType *queque)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queque_IsEmpty(queque))
	{
		return QUEQUE_EMPTY;
	}
	queque->front = (queque->front+1)%QUEQUE_SIZE;
	return QUEQUE_OK;
}



Queque_StdRet Uart_Queque_Clear(QuequeStructType *queque)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	queque->front = 1;
	queque->rear = 0;
	return QUEQUE_OK;
	
}



Queque_Bool  Uart_Queque_IsEmpty(QuequeStructType *queque)
{
	return (queque->rear + 1)%QUEQUE_SIZE == queque->front;
}



Queque_Bool  Uart_Queque_IsFull(QuequeStructType *queque)
{
	return (queque->rear + 2)%QUEQUE_SIZE == queque->front;
}



Queque_StdRet Uart_Queque_Getength(QuequeStructType *queque)
{
	return (queque->front );
}



Queque_StdRet Uart_Queque_GetHead(QuequeStructType *queque,Queque_EleType *element)
{
	if(queque == QUEQUE_NULL_PTR)
	{
		return QUEQUE_PARAM;
	}
	if(Uart_Queque_IsEmpty(queque))
	{
			return QUEQUE_EMPTY;
	}
	return queque->quequeBuf[queque->front];
}


Queque_StdRet Uart_Queque_TransmitToBuffer(QuequeStructType *queque,Queque_EleType *elementBuf)
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




