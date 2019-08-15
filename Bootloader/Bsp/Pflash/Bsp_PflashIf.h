#ifndef __BSP_PFLASHIF_H
#define __BSP_PFLASHIF_H
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Rte_PlatformType.h"


/*
*********************************************************************************************************
*                                 			LOCAL DATA TYPES
*********************************************************************************************************
*/
typedef struct {
    uint8 data[4];
    uint8 Crc;
}BlockType;



extern PflashIf_WriteBlock(uint32 blockAddr,BlockType *blockPtr);
extern PflashIf_MainFunction(void);
extern PflashIf_ReadBlock(BlockType *blockPtr);


