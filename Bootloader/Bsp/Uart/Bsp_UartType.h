#ifndef _BSP_UARTTYPE_H
#define	_BSP_UARTTYPE_H

#include "Rte_PlatformType.h"




#define		MESSAGE_LENGTH			8U

typedef boolean	Uart_Bool;

typedef uint8 Uart_StdType;

typedef struct
{
	uint16 MsgID;
	uint8 MsgData[MESSAGE_LENGTH];
	uint16 MsgCrc;
	uint16 EndFrame;//0x0D0A
}MsgStrcutType;  //�������ֽڶ��룬�������Ҫ����










#endif

