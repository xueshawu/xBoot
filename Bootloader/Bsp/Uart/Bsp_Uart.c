/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_Uart.h"
#include "Cos.h"
#include "Rte_CrcCalculate.h"
#include "Rte_PlatformType.h"
#include "Rte_Queque.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

/*
*********************************************************************************************************
*                                            LOCAL Function Macro
*********************************************************************************************************
*/

#define Uart_Enter_Critical()	CosDisableAllInterrupt()
#define Uart_Quit_Critical()	CosEnableAllInterrupt()

#define Uart_GetCounter()			CosGetCounter()



/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/



#define 	UART_BAUDRATE			115200
#define 	UART_MSGLENGTH			10


#define		UART_TX_LENGTH			12
#define		UART_RX_LENGTH			12


#define 	UART_ID_MAX				3

/*
*********************************************************************************************************
*                                        DEFINE LOCAL FLAG
*********************************************************************************************************
*/

#define 	UART_TX_CONFIRM_FLAG			0x01U
#define		UART_TX_TIMEOUT_FLAG			0x02U
#define 	UART_TX_FULL_FLAG				0x04U
#define		UART_TX_CRC_FLAG				0x08U
#define		UART_TX_START_FLAG				0x10U
#define		UART_TX_STOP_FLAG				0x20U


#define		UART_RX_CONFIRM_FLAG			0x01U
#define		UART_RX_TIMEOUT_FLAG			0x02U
#define		UART_RX_FULL_FLAG				0x04U
#define		UART_RX_CRC_FLAG				0x08U
#define 	UART_RX_START_FLAG				0x10U
#define		UART_RX_STOP_FLAG				0x20U



#define 	UART_TIMEOUT_VAL				200U			



/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

const uint16 g_RxMessageIDTable[UART_ID_MAX]
{
	0x7C0,
	0x7C1,
	0x7C2
};

const uint16 g_TxMessageIDTable[UART_ID_MAX]
{
	0x6C0,
	0x6C1,
	0x6C2
};


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
	0x00,
	&g_UartRxMsgBuf,
};
Uart_TxMsgInfoType	g_TxMsgInfo =
{
	0x00,
	0x00,
	&g_UartTxMsgBuf,
};


Queque_T g_Uart_RxQueque;
Queque_T g_Uart_TxQueque;
uint16 g_FristTime = 0;
uint16 g_LastTime = 0;



/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


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



static void Uart_SetTxConfirmFlag(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_CONFIRM_FLAG;
}

static void Uart_SetRxConfirmFlag(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_CONFIRM_FLAG;
}


static void Uart_SetTxTimeoutFlag(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_TIMEOUT_FLAG;
}

static void Uart_SetRxTimeoutFlag(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_TIMEOUT_FLAG;
}

static void Uart_SetTxBufFullFlag(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_FULL_FLAG;
}

static void Uart_SetRxBufFullFlag(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_FULL_FLAG;
}

static void Uart_SetTxCrcErrFlag(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_CRC_FLAG;
}


static void Uart_SetRxCrcErrFlag(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_CRC_FLAG;
}


static void Uart_SetTxStartFlag(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_START_FLAG;
}


static void Uart_SetRxStartFlag(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_START_FLAG;
}


static void Uart_SetTxStopFlag(void)
{
	g_TxMsgInfo.TxStatus |= UART_TX_STOP_FLAG;
}


static void Uart_SetRxStopFlag(void)
{
	g_RxMsgInfo.RxStatus |= UART_RX_STOP_FLAG;
}



static void Uart_ClearTxConfirmFlag(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_CONFIRM_FLAG);
}


static void Uart_ClearRxConfirmFlag(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_CONFIRM_FLAG);
}


static void Uart_ClearTxTimeOutFlag(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_TIMEOUT_FLAG);
}

static void Uart_ClearRxTimeOutFlag(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_TIMEOUT_FLAG);
}

static void Uart_ClearTxBufFullFlag(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_FULL_FLAG);
}


static void Uart_ClearRxBufFullFlag(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_FULL_FLAG);
}


static void Uart_ClearTxCrcErrFlag(void)
{
	g_TxMsgInfo.TxStatus &= ~(UART_TX_CRC_FLAG);
}


static void Uart_ClearRxCrcErrFlag(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_CRC_FLAG);
}



static void Uart_ClearTxStartFlag(void)
{
	g_RxMsgInfo.RxStatus &= ~(UART_RX_START_FLAG);
}


static void Uart_ClearTxStatus(void)
{
	g_TxMsgInfo.TxStatus = 0x00;
}

static void Uart_ClearRxStatus(void)
{
	g_RxMsgInfo.RxStatus = 0x00;
}

static void Uart_ClearTxCounter(void)
{
	g_TxMsgInfo.TxCnt = 0x00;
}

static void Uart_ClearRxCounter(void)
{
	g_RxMsgInfo.RxCnt = 0x00;
}


static void Uart_ClearTxBuf(void)
{
	Uart_MemSet(g_TxMsgInfo.TxMsgBuf,0x00,MESSAGE_LENGTH);
}


static void Uart_ClearRxBuf(void)
{
	Uart_MemSet(g_RxMsgInfo.RxMsgBuf,0x00,MESSAGE_LENGTH);
}



static Uart_Bool Uart_CheckRxBufIsEmpty(void)
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

static Uart_Bool Uart_CheckTxBufIsFull(void)
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

static Uart_Bool Uart_CheckTxIsTimeout(uint8 fristTime,uint8 lastTime)
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




static Uart_Bool Uart_CheckRxIsTimeout(uint8 fristTime,uint8 lastTime)
{
	if((lastTime - fristTime) > TX_TIMEOUT)
	{
		return UART_TRUE;
	}
	else
	{
		return UART_FALSE;
	}	
}



static Uart_Bool Uart_CheckTxMsgID(uint16 id)
{
	uint8 loopcnt;
	for(loopcnt=0;loopcnt<UART_ID_MAX;loopcnt++)
	{
		if(id == g_TxMessageIDTable[loopcnt])
		{
			return UART_TRUE;
		}
	}
	return UART_FALSE;
}

static Uart_Bool Uart_CheckRxMsgID(uint16 id)
{
	uint8 loopcnt;
	for(loopcnt=0;loopcnt<UART_ID_MAX;loopcnt++)
	{
		if(id == g_RxMessageIDTable[loopcnt])
		{
			return UART_TRUE;
		}
	}
	return UART_FALSE;	
}


static void Uart_SendMsg(void)
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
// 这里queque的大小定义为 siezof(MsgStrcutType)  12Byte

static void Uart_ReceviceMsgCbk(void)
{
	static uint8 recvData = 0;
	recvData = USART_ReceiveData(USART1);
	g_RxMsgInfo.RxCnt++;
	if(g_RxMsgInfo.RxStatus & UART_RX_STOP_FLAG)
	{
		return;
	}
	else if((g_RxMsgInfo.RxStatus & UART_RX_START_FLAG) == 0) //未收到数据
	{

		Uart_SetRxStartFlag();
		Uart_Enter_Critical();
		g_FristTime = Uart_GetCounter();
		Uart_Quit_Critical();
		Uart_Queque_Insert(&g_Uart_RxQueque,recvData);
		
	}
	else //已经开始收数据了
	{
		if(g_RxMsgInfo.RxCnt == sizeof(MsgStrcutType))//数据接收完成
		{
			Uart_SetRxStopFlag();
		}
		if(Uart_CheckRxIsTimeout(g_FristTime, g_LastTime))//接收超时
		{
			Uart_SetRxTimeoutFlag();
			Uart_SetRxStopFlag();
		}
		else//接收未完成
		{
			Uart_Enter_Critical();
			g_LastTime = Uart_GetCounter();
			Uart_Quit_Critical();
			Uart_Queque_Insert(&g_Uart_RxQueque,recvData);
			
		}
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
	
	Uart_ClearTxBuf();
	Uart_ClearTxStatus();
	Uart_ClearRxBuf();
	Uart_ClearRxStatus();
	Uart_Queque_Init();
	
}


void Bsp_Uart_RxMainFunction(void) 
{
	uint8 buf[sizeof(MsgStrcutType)] = {0};
	if(g_RxMsgInfo.RxStatus & UART_RX_STOP_FLAG)//接收完成
	{
		Uart_Queque_TransmitToBuffer(&g_Uart_RxQueque,buf);
		Uart_MemCpyArray2Msg(buf,g_RxMsgInfo.RxMsgBuf);
		if(Uart_CheckRxMsgID(g_RxMsgInfo.RxMsgBuf->MsgID)) //Message ID Check	输出通信矩阵文档
		{
			if()//Check Crc 
		}

	}
	
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
			Uart_Enter_Critical()();
			frist_time = CosGetCounter();
			Uart_Quit_Critical()();
			
			Uart_SendMsg();
			
			Uart_Enter_Critical()();
			last_time = CosGetCounter();
			Uart_Quit_Critical()();
			
			if(Uart_CheckTxIsTimeout(frist_time,last_time))
			{
				Uart_SetTxTimeoutFlag(); //TODO 如果发生TxTimeout 应该怎么处理,
			}
			else
			{
				Uart_ClearTxConfirmFlag();
				Uart_ClearTxBufFullFlag();
				
			}
		}
	}

	
}




Uart_StdType Uart_TransmitReq(MsgStrcutType *Message)
{
	Uart_StdType length = sizeof(MsgStrcutType);
	if(Message == UART_NULL_PTR)
	{
		return UART_PARAM;
	}
	if(Uart_CheckTxBufIsFull())
	{
		return UART_TX_BUFISFULL;
	}
	Uart_MemCpy(Message,g_TxMsgInfo.TxMsgBuf,length);
	Uart_SetTxConfirmFlag();
	return UART_OK;
	
}


Uart_StdType Bsp_Uart_ReceiveReq(MsgStrcutType *Message)
{	
	Uart_StdType length = sizeof(MsgStrcutType);
	if(Message == UART_NULL_PTR)
	{
		return UART_PARAM;
	}
	if(Uart_CheckRxBufIsEmpty())
	{
		return UART_RX_BUFISEMPTY;
	}
	Uart_MemCpy(g_RxMsgInfo.RxMsgBuf,Message,length);
	Uart_SetRxConfirmFlag();
	return UART_OK;
}




/* 中断接收函数，只负责将接收到的输入缓存在queque中 */
/* 由RxMainFuntion 来判断数据的正确性 */
/* 中断Cbk函数 判定帧长度以及是否timeout */
void USART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断 
	{
		Uart_ReceviceMsgCbk();
	}
	

}


/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/








