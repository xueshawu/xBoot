/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_Uart.h"
#include "Bsp_Uart_Queue.h"
#include "Cos.h"
#include "Rte_CrcCalculate.h"
#include "Rte_PlatformType.h"
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
/*
*********************************************************************************************************
*                                        DEFINE LOCAL FLAG
*********************************************************************************************************
*/

/* define status mask */
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
/*  end of status mask */

#define 	UART_TIMEOUT_VAL				200U
#define		UART_CRC_STARTVAL				0xFFFFU			

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

const uint16 g_RxMessageIDTable[UART_RXID_MAX] = {
	0x7C0,
	0x7C1,
	0x7C2
};

const uint16 g_TxMessageIDTable[UART_TXID_MAX] ={
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


QuequeStructType g_Uart_RxQueque;
QuequeStructType g_Uart_TxQueque;
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


static void Uart_MemCpy(void *SrcPtr,void *DestPtr,uint8 Length)
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


static void Uart_SetTxStatus(uint8 flagbit)
{
	g_TxMsgInfo.TxStatus |= flagbit;
}
static uint8 Uart_GetTxStatus(uint8 flagbit)
{
	return (g_TxMsgInfo.TxStatus & flagbit);;
}
static void Uart_ClearTxStatus(uint8 flagbit)
{
	g_TxMsgInfo.TxStatus &= ~(flagbit);
}
static void Uart_SetRxStatus(uint8 flagbit)
{
	g_RxMsgInfo.RxStatus |= flagbit;
}
static uint8 Uart_GetRxStatus(uint8 flagbit)
{
	return (g_RxMsgInfo.RxStatus & flagbit);
}
static void Uart_ClearRxStatus(uint8 flagbit)
{
	g_RxMsgInfo.RxStatus &= ~(flagbit);
}

static void Uart_ClearAllTxStatus(void)
{
	g_TxMsgInfo.TxStatus = 0x00;
}

static void Uart_ClearAllRxStatus(void)
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
	if(Uart_GetRxStatus(UART_RX_FULL_FLAG)) {
		return UART_FALSE;
	} else {
		return UART_TRUE;
	}
}

static Uart_Bool Uart_CheckTxBufIsFull(void)
{
	if(Uart_GetRxStatus(UART_RX_FULL_FLAG)) {
		return UART_TRUE;
	} else {
		return UART_FALSE;
	}
}

static Uart_Bool Uart_CheckTxIsTimeout(uint8 fristTime,uint8 lastTime)
{
	if((lastTime - fristTime) > TX_TIMEOUT) {
		return UART_TRUE;
	} else {
		return UART_FALSE;
	}
}

static Uart_Bool Uart_CheckRxIsTimeout(uint8 fristTime,uint8 lastTime)
{
	if((lastTime - fristTime) > TX_TIMEOUT){
		return UART_TRUE;
	} else {
		return UART_FALSE;
	}	
}

static Uart_Bool Uart_CheckTxMsgID(uint16 id)
{
	uint8 loopcnt;
	for(loopcnt=0;loopcnt<UART_TXID_MAX;loopcnt++) {
		if(id == g_TxMessageIDTable[loopcnt]) {
			return UART_TRUE;
		}
	}
	return UART_FALSE;
}

static Uart_Bool Uart_CheckRxMsgID(uint16 id)
{
	uint8 loopcnt;
	for(loopcnt=0;loopcnt<UART_RXID_MAX;loopcnt++) {
		if(id == g_RxMessageIDTable[loopcnt]) {
			return UART_TRUE;
		}
	}
	return UART_FALSE;	
}

static Uart_Bool Uart_CheckRxCrcVal(void)
{
	uint16 crcVal = Crc_CalculateCRC16(&g_RxMsgInfo.RxMsgBuf->MsgData[0],MESSAGE_LENGTH,UART_CRC_STARTVAL,TRUE);
	return (Uart_Bool)(crcVal == g_RxMsgInfo.RxMsgBuf->MsgCrc);
}

static void Uart_SendMsg(void)
{
	const uint8 length = sizeof(MsgStrcutType);
	uint8 txbuf[length];
	uint8 MemCpyCnt = 0;
	uint8 SendCnt = 0;
	uint8 *BufPtr = (uint8 *)g_RxMsgInfo.RxMsgBuf;
	for(MemCpyCnt =0;MemCpyCnt<length;MemCpyCnt++) {
		txbuf[MemCpyCnt] = *BufPtr;
		BufPtr++;
	}
	for(SendCnt=0;SendCnt<length;SendCnt++ ) {
		while((USART1->SR & USART_FLAG_TXE) == UART_FALSE) {
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
	if(Uart_GetRxStatus(UART_RX_STOP_FLAG)) { //当一帧数据接收完成，这时不接收数据，当START 跟STOP都为0时，认为接收状态机为IDLE状态，准备接收下一帧数据
		return;
	}
	g_RxMsgInfo.RxCnt++;
	if(!Uart_GetRxStatus(UART_RX_START_FLAG)) {
		Uart_SetRxStatus(UART_RX_START_FLAG);
		Uart_Enter_Critical();
		g_FristTime = Uart_GetCounter();
		Uart_Quit_Critical();
		Uart_Queue_Insert(&g_Uart_RxQueque,recvData);
	} else {
		if(g_RxMsgInfo.RxCnt == sizeof(MsgStrcutType)) { //is full 异常情况要提前判断
			Uart_SetRxStatus(UART_RX_START_FLAG);
		} else if(Uart_CheckRxIsTimeout(g_FristTime, g_LastTime)) {
			Uart_SetRxStatus(UART_RX_TIMEOUT_FLAG);
			Uart_SetRxStatus(UART_RX_STOP_FLAG);
		} else {
			Uart_Enter_Critical();
			g_LastTime = Uart_GetCounter();
			Uart_Quit_Critical();
			Uart_Queue_Insert(&g_Uart_RxQueque,recvData);
		}
	}
}

static void Uart_CrcCalculate(uint16 MessageId, uint8 *pMessageContent, MsgStrcutType *pMsgStructInfo)
{
	uint8 buf[sizeof(MsgStrcutType)];
	uint16 crcVal;
	uint8 loopCnt=0;
	buf[0] = (uint8)MessageId & 0x00ff;
	buf[1] = (uint8)(MessageId>>8);
	for(loopCnt=0; loopCnt<MESSAGE_LENGTH; loopCnt++) {
		buf[loopCnt+2] = pMessageContent[loopCnt];
	}
	crcVal = Crc_CalculateCRC16(buf,sizeof(uint16)+MESSAGE_LENGTH,UART_CRC_STARTVAL,TRUE);
	pMsgStructInfo->MsgCrc = crcVal;
	Uart_MemCpy(pMessageContent,pMsgStructInfo->MsgData,MESSAGE_LENGTH);
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
	Uart_ClearAllTxStatus();
	Uart_ClearRxBuf();
	Uart_ClearAllRxStatus();
	Uart_Queue_Init(&g_Uart_RxQueque);
	Uart_Queue_Init(&g_Uart_TxQueque);
}

void Bsp_Uart_RxMainFunction(void) 
{
	uint8 buf[sizeof(MsgStrcutType)] = {0};
	if(Uart_GetRxStatus(UART_RX_STOP_FLAG))
	{
		Uart_Queue_TransmitToBuffer(&g_Uart_RxQueque,buf);
		Uart_MemCpyArray2Msg(buf,g_RxMsgInfo.RxMsgBuf);
		if(Uart_CheckRxMsgID(g_RxMsgInfo.RxMsgBuf->MsgID)) {//只接收指定ID的报文
			if(Uart_CheckRxCrcVal()) {//CRC校验
				//将数据去除crc数据，并将数据推送到UartIf
			} else {
				Uart_SetRxStatus(UART_RX_CRC_FLAG);
			}
 		} else {
			 // do nothing
		 }
	}
	
}

void Bsp_Uart_TxMainFunction(void)
{
	uint8 frist_time = 0;
	uint8 last_time = 0;
	uint8 length = sizeof(MsgStrcutType);
	if(Uart_GetTxStatus(UART_TX_CONFIRM_FLAG)) {
		if(Uart_GetTxStatus(UART_TX_FULL_FLAG)) {//队列的深度只能发送一帧数据
			Uart_Enter_Critical();
			frist_time = CosGetCounter();
			Uart_Quit_Critical();
			Uart_SendMsg();
			Uart_Enter_Critical();
			last_time = CosGetCounter();
			Uart_Quit_Critical();
			if(Uart_CheckTxIsTimeout(frist_time,last_time)) {
				//Uart_SetTxTimeoutFlag(); //TODO 如果发生TxTimeout 应该怎么处理,
				Uart_SetTxStatus(UART_TX_TIMEOUT_FLAG);
			} else {
				//Uart_ClearTxConfirmFlag();
				//Uart_ClearTxBufFullFlag();
				Uart_ClearTxStatus(UART_TX_CONFIRM_FLAG);
				Uart_ClearTxStatus(UART_TX_FULL_FLAG);
			}
		}
	}

	
}

Uart_StdType Uart_TransmitReq(uint16 messageId,uint8 *pMessageContent)
{
	Uart_StdType length = sizeof(MsgStrcutType);
	MsgStrcutType msgStructInfo;
	if(pMessageContent == UART_NULL_PTR) {
		return UART_PARAM;
	}
	if(Uart_CheckTxBufIsFull()) {
		return UART_TX_BUFISFULL;
	}
	msgStructInfo.MsgID = messageId;
	Uart_MemCpy(pMessageContent,&msgStructInfo.MsgData[0],MESSAGE_LENGTH);
	Uart_CrcCalculate(messageId,pMessageContent,&msgStructInfo);
	Uart_MemCpy(&msgStructInfo,g_TxMsgInfo.TxMsgBuf,length);
	Uart_SetTxStatus(UART_TX_CONFIRM_FLAG);
	return UART_OK;
}

Uart_StdType Uart_ReceiveReq(uint16 messageId,uint8 *pMessageContent)
{	
	Uart_StdType length = sizeof(MsgStrcutType);
	if(pMessageContent == UART_NULL_PTR) {
		return UART_PARAM;
	}
	if(Uart_CheckRxBufIsEmpty()) {
		return UART_RX_BUFISEMPTY;
	}
	Uart_MemCpy(g_RxMsgInfo.RxMsgBuf,pMessageContent,length);
	Uart_SetRxStatus(UART_RX_CONFIRM_FLAG);//Uart_SetRxConfirmFlag();
	return UART_OK;
}




/* 中断接收函数，只负责将接收到的输入缓存在queque中 */
/* 由RxMainFuntion 来判断数据的正确性 */
/* 中断Cbk函数 判定帧长度以及是否发生timeout事件 */
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		Uart_ReceviceMsgCbk();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//从移位寄存器中读取数据貌似能清除接收中断标志位，尚未Debug确认
	}
	
}


/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/








