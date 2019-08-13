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
#define UARTIF_E_OK                     0x0U
#define UARTIF_E_NOTOK                  0x1U
#define UARTIF_E_NULL                   0x2U
#define UARTIF_E_ID                     0x3U
#define UARTIF_E_OFR                    0xFFU           /* id out of range */
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern Uart_StdType UartIf_AsyncTransmit(uint16 MsgIndex,uint8 *pMsgContent);
extern Uart_StdType UartIf_AsyncReceive(uint16 MsgIndex,uint8 *pMsgContent);
extern Uart_StdType UartIf_TxConfirmation(uint16 MsgIndex,uint8 *pMsgContent);
extern Uart_StdType UartIf_RxIndication(MsgStrcutType *pUartMsgInfoPtr);
extern void UartIf_RxMainFunction(void);
extern void UartIf_TxMainFunction(void);
extern void UartIf_Init(void);
#endif

