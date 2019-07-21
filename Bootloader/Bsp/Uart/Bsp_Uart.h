#ifndef __BSP_UART_H
#define __BSP_UART_H
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_UartType.h"

/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */



#define		TX_TIMEOUT				200U
#define		RX_TIMEOUT				200U

#define 	UART_NULL_PTR			NULL_PTR

#define 	UART_OK					0x00U
#define		UART_NOTOK				0x01U
#define		UART_PARAM				0x02U
#define		UART_TX_TIMEOUT			0x03U
#define		UART_RX_TIMEOUT			0x04U
#define		UART_TX_BUFISFULL		0x05U
#define		UART_RX_BUFISEMPTY		0x06U

#define		UART_TRUE				TRUE
#define		UART_FALSE				FALSE

#define 	UART_RXID_MAX			3
#define		UART_TXID_MAX			3				
/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/
typedef struct
{
	uint8 TxStatus;
	uint8 TxCnt;
	MsgStrcutType *TxMsgBuf; //����������ָ�룬����sizeof Ӧ���ǣ�4+1��
}Uart_TxMsgInfoType;


typedef struct
{
	uint8 RxStatus;
	uint8 RxCnt;
	MsgStrcutType *RxMsgBuf;
}Uart_RxMsgInfoType;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern void Uart_Init(void);
extern void Uart_RxMainFunction(void);
extern void Uart_TxMainFunction(void);
extern void Uart_RxIndication(void);
extern void Uart_TxConfirm(void);
extern Uart_StdType Uart_TransmitReq(uint16 messageId,uint8 *pMessageContent);
extern Uart_StdType Bsp_Uart_ReceiveReq(uint16 messageId, uint8 *pMessageContent);

/*
*********************************************************************************************************
*                                      ENF OF   FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#else 
	#error "Bsp_Uart.h has been define"

#endif
/*
*********************************************************************************************************
*                                        END OF THE FILE
*********************************************************************************************************
*/








