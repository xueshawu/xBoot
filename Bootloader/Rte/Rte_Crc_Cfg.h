#ifndef _RTE_CRC_CFG_H
#define _RTE_CRC_CFG_H

#define CRC_16_HARDWARE 0
#define CRC_16_RUNTIME 1
#define CRC_16_TABLE 2

#define CRC_16_MODE CRC_16_RUNTIME

#define CRC_32_HARDWARE 0
#define CRC_32_RUNTIME 1
#define CRC_32_TABLE 2

#define CRC_32_MODE CRC_32_RUNTIME

#define CRC_8_HARDWARE 0
#define CRC_8_RUNTIME 1
#define CRC_8_TABLE 2

#define CRC_8_MODE CRC_8_TABLE

#define CRC_8H2F_HARDWARE 0
#define CRC_8H2F_RUNTIME 1
#define CRC_8H2F_TABLE 2

#define CRC_8H2F_MODE CRC_8H2F_RUNTIME











#else
	#error "Rte_Crc_Cfg.h has been define"


#endif
/* end of this file */


