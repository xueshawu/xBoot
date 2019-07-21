#ifndef __BSP_UARTIF_H
#define __BSP_UARTIF_H
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_UartType.h"
/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/
typedef struct
{
    uint16 msgid;
    uint8  msgdata[MESSAGE_LENGTH];
    uint8  updatebit;
} UartIf_PduInfoType;
/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */
#define UARTIF_E_OK                     0x0
#define UARTIF_E_NOTOK                  0x1
#define UARTIF_E_NULL                   0x2
#define UARTIF_E_ID                     0x3
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern Uart_StdType UartIf_TxMessage(uint16 MsgIndex,uint8 *pMsgContent);
extern Uart_StdType UartIf_RxMessage(uint16 MsgIndex,uint8 *pMsgContent);
extern Uart_StdType UartIf_TxConfirmation(uint16 MsgIndex,uint8 *pMsgContent);
extern Uart_StdType UartIf_RxIndication(MsgStrcutType *pUartMsgInfoPtr);
extern void UartIf_RxMainFunction(void);
extern void UartIf_TxMainFunction(void);
extern void UartIf_Init(void);
#endif

