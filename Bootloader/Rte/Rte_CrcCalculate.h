#ifndef _RTE_CRCCALCULATE_H
#define _RTE_CRCCALCULATE_H




/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Rte_PlatformType.h"

/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */


#define		CRC_OK			0
#define		CRC_PARAM		1
#define		CRC_LENGTH		2


#define		CRC_NULL_PTR		NULL_PTR

/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/	

typedef uint8	Crc_StdType;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern Crc_StdType Rte_Crc_CalculateCRC8(const Crc_StdType *Crc_DataPtr,
										 uint32 Crc_Length,
										 uint8 *Crc_Crc8Code
										 );


extern Crc_StdType Rte_Crc_CalculateCRC16(const Crc_StdType *Crc_DataPtr,
										 uint32 Crc_Length,
										 uint16 *Crc_Crc16Code
										 );


extern Crc_StdType Rte_Crc_CalculateCRC32(const Crc_StdType *Crc_DataPtr,
										  uint32 Crc_Length,
										  uint32 *Crc_Crc32Code
										 );















#endif


