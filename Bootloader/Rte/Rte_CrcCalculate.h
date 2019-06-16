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

/* For CRC 8*/
#define CRC8_START_VALUE        0xFFU
#define CRC8_XOR                0xFFU

/* For CRC8 H2F */
#define CRC8_H2F_START_VALUE    0xFFU
#define CRC8_H2F_XOR            0xFFU

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


