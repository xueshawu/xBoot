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



/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/	



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

uint8 Crc_CalculateCRC8(const uint8* Crc_DataPtr, uint32 CRC_Length, 
								uint8 Crc_StartValue8, boolean Crc_IsFirstCall);

uint8 Crc_CalculateCRC8H2F(const uint8* Crc_DataPtr, uint32 CRC_Length, 
									 uint8 Crc_StartValue8, boolean Crc_IsFirstCall);

uint16 Crc_CalculateCRC16(const uint8* Crc_DataPtr, uint32 CRC_Length, 
								   uint16 Crc_StartValue16, boolean Crc_IsFirstCall);

uint32 Crc_CalculateCRC32(const uint8* Crc_DataPtr, uint32 CRC_Length, 
									uint32 Crc_StartValue32, boolean Crc_IsFirstCall);


#endif


