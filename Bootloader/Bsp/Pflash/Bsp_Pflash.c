/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Bsp_PflashIf.h"
#include "Rte_PlatformType.h"
#include "stm32f10x_flash.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define     PFLASH_E_OK           	0x0U
#define		PFLASH_E_NOTOK			0x1U
#define		PFLASH_E_ADDR			0x2U 					/* flash address error */

#define		PFLASH_PAGE_SIZE		((uint32_t)0x00000800) /* FLASH Page Size */
#define		PFALSH_USER_START_ADDR	((uint32_t)0x08009000) /* Start of user Flash area */
#define		PFALSH_USER_STOP_ADDR	((uint32_t)0x08020000) /* End of user Flash area */

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
/* buff指针 */
uint32 *g_PflashWriteBuffPtr = NULL_PTR;
uint32 *g_pflashReadBuffPtr = NULL_PTR;

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/
uint32 g_ProgramBuff[PFLASH_PAGE_SIZE/4] = {0xFFFFFFFF}; //映射一个Page的大小2kb

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static uint32 Pflash_ReadData(uint32 dataAddr) 
{
    return *(volatile uint32 *)dataAddr;
}

void Pflash_Init(void)
{
    g_PflashWriteBuffPtr = g_ProgramBuff; //写缓存指针
    g_pflashReadBuffPtr = g_ProgramBuff; //读缓存指针
}

uint32 *Pflash_GetReadBufferPtr(void)
{
    return g_pflashReadBuffPtr;
}

uint32 *pflash_GetWriteBufferPtr(void)
{
    return g_PflashWriteBuffPtr;
}

/* 按Page写入，每次写满一个Page */
/* 检查地址是否Page对齐  */
/* 检查地址的合法性 */
/* 1 解锁 */
/* 2 擦除Page */
Std_ReturnType Pflash_WriteByPage(uint32 pageAddr, uint32 *pagedata)
{
    uint32 pageSize = PFLASH_PAGE_SIZE;
    FLASH_Status status = FLASH_COMPLETE;
    uint32 loopCnt = 0x0;
    if((pageaddr % PFLASH_PAGE_SIZE) != 0) {
        return PFLASH_E_ADDR;  //地址对齐错误
    }
     FLASH_Unlock();
    for(; loopCnt<pageSize; loopCnt++) { //一次必须写满一个Page
       status = FLASH_ProgramWord(pageAddr, pagedata);
       if( status != FLASH_COMPLETE) {
           return PFLASH_E_NOTOK;
       }
    }
    FLASH_lock();
    return PFLASH_E_OK;

}

Std_ReturnType Pflash_ReadPage(uint32 pageAddr ,uint32 *readAddr) 
{
    uint32 pageSize = PFLASH_PAGE_SIZE;
    uint32 loopCnt = 0x0;
    if((pageAddr % PFLASH_PAGE_SIZE) != 0) {
        return PFLASH_E_ADDR;
    }
    for(; loopCnt<pageSize; loopCnt++) {
        *readAddr = Pflash_ReadData(pageAddr);
        pageAddr += 4;
    }
    return PFLASH_E_OK;
}


Std_ReturnType Pflash_EraseByPage(uint32 pageAddr)
{
    FLASH_Status status = FLASH_COMPLETE;
    if(pageaddr % PFLASH_PAGE_SIZE !=0) {
        return PFLASH_E_ADDR;
    }
	status = FLASH_ErasePage(pageAddr);
    if(status != FLASH_COMPLETE) {
        return PFLASH_E_NOTOK;
    } else {
        return PFLASH_E_OK;
    }

}











