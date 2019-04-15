#ifndef _BSP_UARTTYPE_H
#define	_BSP_UARTTYPE_H

#include "Rte_PlatformType.h"




#define		MESSAGE_LENGTH			8U

typedef boolean	Uart_Bool;

typedef uint8 Uart_StdType;

typedef struct
{
	uint16 MsgID;
	uint16 MsgCrc;
	uint8 MsgData[MESSAGE_LENGTH];
}MsgStrcutType;  //必须四字节对齐，不足的需要补齐










#endif

