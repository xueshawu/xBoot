/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_Uart.h"
#include "Cos.h"
#include "Rte_PlatformType.h"
#include "Rte_Queque.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/




#define 	UART_BAUDRATE			115200
#define 	UART_MSGLENGTH			10


#define		UART_TX_LENGTH			12
#define		UART_RX_LENGTH			12



/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

#define 	UART_TX_CONFIRM_FLAG			0x01U
#define		UART_TX_TIMEOUT_FLAG			0x02U
#define 	UART_TX_FULL_FLAG				0x04U
#define		UART_TX_CRC_FLAG				0x08U


#define		UART_RX_CONFIRM_FLAG			0x01U
#define		UART_RX_TIMEOUT_FLAG			0x02U
#define		UART_RX_FULL_FLAG				0x03U
#define		UART_RX_CRC_FLAG				0x08U



#define 	UART_TIMEOUT_VAL				200U			//200ms

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

MsgStrcutType g_UartTxMsgBuf;
MsgStrcutType g_UartRxMsgBuf;

Uart_RxMsgInfoType	g_RxMsgInfo =
{
	0x00,
	&g_UartRxMsgBuf,
};
Uart_TxMsgInfoType	g_TxMsgInfo =
{
	0x00,
	&g_UartTxMsgBuf,
};



/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/





static void Uart_MemSet(void *buffer, uint8 c, uint16 count);
static void Uart_MemCpy(MsgStrcutType *SrcPtr,MsgStrcutType *DestPtr,uint8 Length);



static void Bsp_Uart_SetTxConfirm(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_CONFIRM_FLAG;
}

static void Bsp_Uart_SetRxConfirm(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_CONFIRM_FLAG;
}


static void Bsp_Uart_SetTxTimeout(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_TIMEOUT_FLAG;
}

static void Bsp_Uart_SetRxTimeout(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_TIMEOUT_FLAG;
}


static void Bsp_Uart_SetTxCrcErr(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_CRC_FLAG;
}


static void Bsp_Uart_SetRxCrcErr(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_CRC_FLAG;
}


static void Bsp_Uart_ClearTxConfirm(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_CONFIRM_FLAG);
}


static void Bsp_Uart_ClearRxConfirm(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_CONFIRM_FLAG);
}


static void Bsp_Uart_ClearTxTimeOut(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_TIMEOUT_FLAG);
}

static void Bsp_Uart_ClearRxTimeOut(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_TIMEOUT_FLAG);
}

static void Bsp_Uart_ClearTxStatus(void)
{
	g_TxMsgInfo.TxStatus = 0x00;
}

static void Bsp_Uart_ClearRxStatus(void)
{
	g_RxMsgInfo.RxStatus = 0x00;
}

static void Bsp_Uart_ClearTxCrcErr(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_CRC_FLAG);
}


static void Bsp_Uart_ClearRxCrcErr(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_CRC_FLAG);
}




static void Bsp_Uart_ClearTxBuf(void)
{
	Uart_MemSet(g_TxMsgInfo.TxMsgBuf,0x00,MESSAGE_LENGTH);
}


static void Bsp_Uart_ClearRxBuf(void)
{
	Uart_MemSet(g_RxMsgInfo.RxMsgBuf,0x00,MESSAGE_LENGTH);
}






static Uart_Bool Bsp_Uart_CheckRxBufIsEmpty(void)
{
	if(g_RxMsgInfo.RxStatus & UART_RX_FULL_FLAG)
	{
		return UART_FALSE;
	}
	else
	{
		return UART_TRUE;
	}
}

static Uart_Bool Bsp_Uart_CheckTxBufIsFull(void)
{
	if(g_TxMsgInfo.TxStatus & UART_TX_FULL_FLAG)
	{
		return UART_TRUE;
	}
	else
	{
		return UART_FALSE;
	}
}

static Uart_Bool Bsp_Uart_CheckTxIsTimeout(uint8 fristTime,uint8 lastTime)
{
	if((lastTime - fristTime) > TX_TIMEOUT) //TODO 这里可能会溢出，导致判断失败，如何规避？？？ g_SysCounter 为16位数据，节拍为1MS
	{
		return UART_TRUE;
	}
	else
	{
		return UART_FALSE;
	}
}


Uart_StdType Bsp_Uart_TransmitReq(MsgStrcutType *Message)
{
	Uart_StdType length = sizeof(MsgStrcutType);
	if(Message == UART_NULL_PTR)
	{
		return UART_PARAM;
	}
	if(Bsp_Uart_CheckTxBufIsFull())
	{
		return UART_TX_BUFISFULL;
	}
	Uart_MemCpy(Message,g_TxMsgInfo.TxMsgBuf,length);
	Bsp_Uart_SetTxConfirm();
	return UART_OK;
	
}


Uart_StdType Bsp_Uart_ReceiveReq(MsgStrcutType *Message)
{	
	Uart_StdType length = sizeof(MsgStrcutType);
	if(Message == UART_NULL_PTR)
	{
		return UART_PARAM;
	}
	if(Bsp_Uart_CheckRxBufIsEmpty())
	{
		return UART_RX_BUFISEMPTY;
	}
	Uart_MemCpy(g_RxMsgInfo.RxMsgBuf,Message,length);
	Bsp_Uart_SetRxConfirm();
	return UART_OK;
}




static void Bsp_Uart_SendMessage(void)
{
	const uint8 length = sizeof(MsgStrcutType);
	uint8 txbuf[length];
	uint8 MemCpyCnt = 0;
	uint8 SendCnt = 0;
	uint8 *BufPtr = (uint8 *)g_RxMsgInfo.RxMsgBuf;
	for(MemCpyCnt =0;MemCpyCnt<length;MemCpyCnt++)
	{
		txbuf[MemCpyCnt] = *BufPtr;
		BufPtr++;
	}
	for(SendCnt=0;SendCnt<length;SendCnt++ )
	{
		while((USART1->SR & USART_FLAG_TXE) == UART_FALSE)//Check TXE Flag 在将数据压入DataReg之后，FLAG_TXE 值为 TRUE 需要多久置位，重新置为FALSE需要多久
		{
			USART1->DR = txbuf[SendCnt];
		}
	}
}

// 采用队列的方式接收数据，帧的判定时间为200ms，
static void Bsp_Uart_ReceviceMessage(void)
{

}



static void Uart_MemSet(void *buffer, uint8 c, uint16 count)
{
	uint8 *temp = (uint8 *)buffer;
	while(count--)
	{
		*temp = c;
		temp++;
	}
	
}


static void Uart_MemCpy(MsgStrcutType *SrcPtr,MsgStrcutType *DestPtr,uint8 Length)
{
	uint8 *src = (uint8 *)SrcPtr;
	uint8 *dest = (uint8 *)DestPtr;
	while(Length--)
	{
		*dest = *src;
		dest++;
		src++;
	}
}


static void Uart_MemCpyMsg2Array(MsgStrcutType *SrcMsgPtr,uint8 *DestarrayPrt)
{
	uint8 length = sizeof(MsgStrcutType);
	uint8 *src = (uint8 *)SrcMsgPtr;
	uint8 *dest = DestarrayPrt;
	while(length--)
	{
		*src = *dest;
		src++;
		dest++;
	}
	
}

static void Uart_MemCpyArray2Msg(uint8 *SrcArrayPtr,MsgStrcutType *DestMsgPtr)
{
	uint8 length = sizeof(MsgStrcutType);
	uint8 *src = SrcArrayPtr;
	uint8 *dest = (uint8 *)DestMsgPtr;
	while(length--)
	{
		*src = *dest;
		src++;
		dest++;		
	}
}



void Bsp_Uart_Init(void)
{
	GPIO_InitTypeDef	gpio_InitConfig;
	USART_InitTypeDef	uart_InitConfig;
	NVIC_InitTypeDef	nvic_InitConfig;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


	gpio_InitConfig.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_InitConfig.GPIO_Pin = GPIO_Pin_9;
	gpio_InitConfig.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_InitConfig);

	gpio_InitConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_InitConfig.GPIO_Pin = GPIO_Pin_10;
	gpio_InitConfig.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_InitConfig);


	uart_InitConfig.USART_BaudRate = UART_BAUDRATE;
	uart_InitConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart_InitConfig.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	uart_InitConfig.USART_Parity =USART_Parity_No;
	uart_InitConfig.USART_StopBits = USART_StopBits_1;
	uart_InitConfig.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &uart_InitConfig);

	nvic_InitConfig.NVIC_IRQChannel = USART1_IRQn;
	nvic_InitConfig.NVIC_IRQChannelPreemptionPriority=3 ;
	nvic_InitConfig.NVIC_IRQChannelSubPriority = 3;		
	nvic_InitConfig.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&nvic_InitConfig);	

	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE); 
	
	Bsp_Uart_ClearTxBuf();
	Bsp_Uart_ClearRxBuf();
	Bsp_Uart_ClearTxStatus();
	Bsp_Uart_ClearRxStatus();
	
}


void Bsp_Uart_RxMainFunction(void) 
{
	uint8 frist_time = 0;
	uint8 last_time = 0;
	uint8 length = 0;
	length = sizeof(MsgStrcutType);	
	
}


void Bsp_Uart_TxMainFunction(void)
{
	uint8 frist_time = 0;
	uint8 last_time = 0;
	uint8 length = 0;
	length = sizeof(MsgStrcutType);
	if(g_TxMsgInfo.TxStatus & UART_TX_CONFIRM_FLAG) //进入发送环节
	{
		if(g_TxMsgInfo.TxStatus & UART_TX_FULL_FLAG) //确认buf有新的数据
		{
			frist_time = CosGetCounter();
			Bsp_Uart_SendMessage();
			last_time = CosGetCounter();
			if(Bsp_Uart_CheckTxIsTimeout(frist_time,last_time))
			{
				Bsp_Uart_SetTxTimeout(); //TODO 如果发生TxTimeout 应该怎么处理？
			}
			else
			{
				Bsp_Uart_ClearTxConfirm();
			}
		}
	}

	
}



/* 中断接收函数，只负责将接收到的输入缓存在queque中 */
/* 由RxMainFuntion 来判断数据的正确性 */
void USART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断 
	{

	}
	

}


/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/







