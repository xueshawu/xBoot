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
#define   TRANSMIT              0x2U
#define   BUSY                  0x3U
#define   PENDING               0x4U

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/
static UartIf_PduInfoType g_RxPduInfoGroup[UART_RXID_MAX] = {
                 /*MsgID */ /*   MsgBuf               */ /*updatebit */
     /* index 0*/{0x7C0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},0x0},
     /* index 1*/{0x7C1,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},0x0},
     /* index 2*/{0x7C2,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},0x0},
};

static UartIf_PduInfoType g_TxPduInfoGroup[UART_TXID_MAX] = {
                 /*MsgID */ /*   MsgBuf               */ /*updatebit */     
     /* index 0*/{0x6C0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},0x0},
     /* index 1*/{0x6C1,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},0x0},
     /* index 2*/{0x6C2,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,},0x0},
};
/*
*********************************************************************************************************
*                                            LOCAL VAR
*********************************************************************************************************
*/
static uint8 g_UartIfRxMainFsmState = UNINITIAL;
static uint8 g_UartIfTxMainFsmState = UNINITIAL;
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* Service name: UartIf_CheckMessageId
   Service ID[hex]: 0x01
   Sync/Async: Synchronous
   Reentrancy: Non Reentrant
   Parameters (in): ConfigPtr Pointer to configuration parameter set, used e.g. for post build parameters
   Parameters (inout): None
   Parameters (out): None
   Return value: None
   Description: This service Initializes internal and external interfaces of the CAN Interface for the further processing.
*/
Uart_StdType UartIf_TxMessage(uint16 MsgIndex,uint8 *pMsgContent)
{
     if (pMsgContent == NULL_PTR) {
          return UARTIF_E_NULL;
     }
     return UARTIF_E_OK;
}
Uart_StdType UartIf_RxMessage(uint16 MsgIndex,uint8 *pMsgContent)
{
     if (pMsgContent == NULL_PTR) {
          return UARTIF_E_NULL;
     }
     return UARTIF_E_OK;    
}
Uart_StdType UartIf_TxConfirmation(uint16 MsgIndex,uint8 *pMsgContent)
{
     return UARTIF_E_OK;
}
Uart_StdType UartIf_RxIndication(MsgStrcutType *pUartMsgInfoPtr)
{
     return UARTIF_E_OK;
}
void UartIf_RxMainFunction(void)
{

}
void UartIf_TxMainFunction(void)
{

}
void UartIf_Init(void)
{

}
void UartIf_DeInit(void)
{

}

