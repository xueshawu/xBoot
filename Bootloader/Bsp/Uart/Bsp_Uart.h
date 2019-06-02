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
#define		UART_PARAM				0x01U
#define		UART_TX_TIMEOUT			0x02U
#define		UART_RX_TIMEOUT			0x03U
#define		UART_TX_BUFISFULL		0x04U
#define		UART_RX_BUFISEMPTY		0x05U



#define		UART_TRUE				TRUE
#define		UART_FALSE				FALSE

/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/




typedef struct
{
	uint8 TxStatus;
	uint8 TxCnt;
	MsgStrcutType *TxMsgBuf; //这里数据是指针，按照sizeof 应该是（4+1）
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
extern void Uart_StdType Uart_TransmitReq(MsgStrcutType *Message);
extern Uart_StdType Bsp_Uart_ReceiveReq(MsgStrcutType *Message);










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








