/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_Uart.h"
#include "Bsp_UartIf.h"
/*
*********************************************************************************************************
*                                        DEFINE LOCAL FLAG
*********************************************************************************************************
*/
#define   UNINITIAL             0x0U
#define   INITIAL               0x1U
#define   TRANSMIT              0x2U /*for Tx */
#define   RECEIVE               0x2U /*for Rx */
#define   BUSY                  0x3U
#define   PENDING               0x4U

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/
static UartIf_PduInfoType g_RxPduInfoGroup[UART_RXID_MAX] = {
                 /*MsgID */ /*   MsgBuf               */ /*updatebit */
     /* index 0*/{0x7C0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},UNINITIAL},
     /* index 1*/{0x7C1,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},UNINITIAL},
     /* index 2*/{0x7C2,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},UNINITIAL},
};

static UartIf_PduInfoType g_TxPduInfoGroup[UART_TXID_MAX] = {
                 /*MsgID */ /*   MsgBuf               */ /*updatebit */     
     /* index 0*/{0x6C0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},UNINITIAL},
     /* index 1*/{0x6C1,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},UNINITIAL},
     /* index 2*/{0x6C2,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},UNINITIAL},
};
/*
*********************************************************************************************************
*                                            LOCAL VAR
*********************************************************************************************************
*/
static uint8 g_UartIfTxMainFsmState = UNINITIAL;
static uint8 g_UartIfRxMainFsmState = UNINITIAL;
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void UartIf_MemCpy(void *destin, void *source, unsigned length)
{
     uint8 *dest,*src; 
     dest = (uint8 *)destin;
     src = (uint8 *)source;
     if((destin == NULL_PTR)||(source == NULL_PTR)) {
          return;
     }
     if(length <= 0) {
          return;
     }
     if(dest<=src || (uint8 *)dest >= (uint8 *)src +length) {
		 while(length--) {
			*(uint8 *)dest= *(uint8 *)src;
			dest=(uint8 *)dest+1;
			src = (uint8 *)src+1;
		 }
	 } else {
		src = (uint8 *)src+length-1;
		dest = (uint8 *)dest+length-1;
		while(length--) {
			*(uint8*)dest = *(uint8 *)src;
			dest = (uint8 *)dest-1;
			src = (uint8 *)src-1;
		}
	 }
}

static uint8 UartIf_GetTxUpdateBit(uint16 MsgIndex)
{
     UartIf_PduInfoType *txPduMsgInfo = &g_TxPduInfoGroup[0];
     return txPduMsgInfo[MsgIndex].updatebit;    
}

static void UartIf_SetTxUpdateBit(uint16 MsgIndex, uint8 state)
{
     UartIf_PduInfoType *txPduMsgInfo = &g_TxPduInfoGroup[0];
     txPduMsgInfo[MsgIndex].updatebit = state;
}

static void UartIf_ClearTxUpdateBit(uint16 MsgIndex) 
{
     UartIf_PduInfoType *txPduMsgInfo = &g_TxPduInfoGroup[0];
     txPduMsgInfo[MsgIndex].updatebit = INITIAL;    
}


static uint8 UartIf_GetRxUpdateBit(uint16 MsgIndex)
{
     UartIf_PduInfoType *txPduMsgInfo = &g_RxPduInfoGroup[0];
     return txPduMsgInfo[MsgIndex].updatebit;
}

static void UartIf_SetRxUpdateBit(uint16 MsgIndex, uint8 state)
{
     UartIf_PduInfoType *txPduMsgInfo = &g_RxPduInfoGroup[0];
     txPduMsgInfo[MsgIndex].updatebit = state;
}

static uint8 UartIf_FindRxMsgIndex(uint16 MsgId)
{
     uint8 loopCnt = 0;
     UartIf_PduInfoType *rxPduMsgInfo = &g_RxPduInfoGroup[0];
     for(loopCnt=0; loopCnt<UART_RXID_MAX; loopCnt++) {
          if(rxPduMsgInfo[loopCnt].msgid == MsgId) {
               return loopCnt;
          }
     }
     return UARTIF_E_OFR;/* returen id out of range */
}

Uart_StdType UartIf_AsyncTransmit(uint16 MsgIndex,uint8 *pMsgContent)
{
	 uint8 *txmsgDataBuf = &g_TxPduInfoGroup[MsgIndex].msgdata[0];
     if (pMsgContent == NULL_PTR) {
          return UARTIF_E_NULL;
     }
	 if (MsgIndex > UART_TXID_MAX) {
		 return UARTIF_E_OFR;
	 }
     if (PENDING !=UartIf_GetTxUpdateBit(MsgIndex)) { //数据还没有被TxConfirm清除，不能发送第二帧
          return UARTIF_E_NOTOK;
     }
     UartIf_MemCpy(txmsgDataBuf, pMsgContent, MESSAGE_LENGTH);
     UartIf_SetTxUpdateBit(MsgIndex,TRANSMIT);
     return UARTIF_E_OK;
}

Uart_StdType UartIf_AsyncReceive(uint16 MsgIndex,uint8 *pMsgContent)
{
     uint8 *rxmsgDataBuf = &g_RxPduInfoGroup[MsgIndex].msgdata[0];
     if (pMsgContent == NULL_PTR) {
          return UARTIF_E_NULL;
     }
     if(UartIf_GetRxUpdateBit(MsgIndex) != RECEIVE) {
          return UARTIF_E_NOTOK;
     } else {
          UartIf_MemCpy(pMsgContent, rxmsgDataBuf, MESSAGE_LENGTH);
          UartIf_SetRxUpdateBit(MsgIndex, PENDING);//将UpdateBit 设置为PENDING状态，表示已经拿走了数据，
          return UARTIF_E_OK;
     }   
}

Uart_StdType UartIf_TxConfirmation(uint16 MsgIndex,uint8 *pMsgContent)
{
     UartIf_SetTxUpdateBit(MsgIndex, PENDING);//下层确认已经发送了数据，将状态位置成PENDING，后面允许处理下一帧报文
     return UARTIF_E_OK;
}
Uart_StdType UartIf_RxIndication(MsgStrcutType *pUartMsgInfoPtr)
{
     uint16 MsgIndex = 0;
     MsgStrcutType *MsgStructInfo = pUartMsgInfoPtr;
     UartIf_PduInfoType *rxPduMsgInfo = &g_TxPduInfoGroup[0];
     if(MsgStructInfo == UART_NULL_PTR) {
          return UARTIF_E_NULL; 
	 }
     MsgIndex = UartIf_FindRxMsgIndex(MsgStructInfo->MsgID);
     if(MsgIndex == UARTIF_E_OFR) {
          return UARTIF_E_ID;
     }
     UartIf_MemCpy(rxPduMsgInfo[MsgIndex].msgdata, MsgStructInfo->MsgData, MESSAGE_LENGTH);
     UartIf_SetRxUpdateBit(MsgIndex,BUSY);
     return UARTIF_E_OK;
}
	 
void UartIf_RxMainFunction(void)//由RxIndication 将数据存储Buf并将状态位置为BUSY，由RxMainFunction 轮询BUSY状态，将状态置为RECEIVE，最后AsyncReceive拿走数据后重新置为PEDING
{
     uint8 loopCnt = 0;
     if ((g_UartIfTxMainFsmState == UNINITIAL) || (g_UartIfRxMainFsmState == UNINITIAL)){
          return;
     }
     for(loopCnt=0; loopCnt<UART_RXID_MAX; loopCnt++) {
          if(UartIf_GetRxUpdateBit(loopCnt) == BUSY) {
               UartIf_SetTxUpdateBit(loopCnt,RECEIVE);
               return;
          }
     }
}

void UartIf_TxMainFunction(void)//轮询UpdateBit的状态，将TRANSMIT状态的数据送给UART并将状态置为BUSY
{
     uint8 loopCnt = 0;
     UartIf_PduInfoType  *txPduInfo = &g_TxPduInfoGroup[0];
     if(g_UartIfTxMainFsmState == UNINITIAL) {
          return;
     }
     for(loopCnt=0; loopCnt<UART_TXID_MAX; loopCnt++) {
          if(UartIf_GetTxUpdateBit(loopCnt) == TRANSMIT) {
               Uart_TransmitReq(txPduInfo[loopCnt].msgid, &txPduInfo[loopCnt].msgdata[0]);
               UartIf_SetTxUpdateBit(loopCnt,BUSY);
               return;
          }
     }    
}

void UartIf_Init(void)
{
    g_UartIfTxMainFsmState = INITIAL;
    g_UartIfRxMainFsmState = INITIAL;
}

void UartIf_DeInit(void)
{
     g_UartIfTxMainFsmState = UNINITIAL;
     g_UartIfRxMainFsmState = UNINITIAL;
}

