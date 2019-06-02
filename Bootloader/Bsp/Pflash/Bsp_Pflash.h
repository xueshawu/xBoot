#ifndef __PFLASH_H
#define __PFLASH_H




#include "Rte_PlatformType.h"
extern void Pflash_Init(void);
extern Std_ReturnType Pflash_Write(uint32 addr, uint8 data);
extern Std_ReturnType Pflash_EraseByPage(uint32 addr);





















#else
    #error "Pflash.h has been define "


#endif