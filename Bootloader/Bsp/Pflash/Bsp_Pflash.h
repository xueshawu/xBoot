#ifndef __PFLASH_H
#define __PFLASH_H
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Rte_PlatformType.h"

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
extern void Pflash_Init(void);
extern 
extern Std_ReturnType Pflash_Write(uint32 addr, uint8 data);
extern Std_ReturnType Pflash_ReadPage(uint32 pageAddr ,uint32 *readAddr) ;
extern Std_ReturnType Pflash_EraseByPage(uint32 addr);

#else
    #error "Pflash.h has been define "


#endif