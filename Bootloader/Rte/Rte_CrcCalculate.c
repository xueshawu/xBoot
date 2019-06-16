
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "Rte_CrcCalculate.h"



/*
 ********************************************************************************************************
 *                                            LOCAL DEFINES
 ********************************************************************************************************
 */

#define		CRC_NULL_PTR		NULL_PTR

#if CRC_8H2F_MODE == CRC_8H2F_RUNTIME
/* For CRC8 SAE-1850 */
#define CRC8_POLYNOMIAL             0x1D
/* For CRC8 H2F */
#define CRC8_H2F_POLYNOMIAL         0x2F
#endif

#define CRC16_CCITT_START_VALUE    0xFFFFU
#define CRC16_CCITT_POLYNOMIAL     0x1021U
#define CRC16_CCITT_XOR            0x0000U

#define CRC32_START_VALUE           0xFFFFFFFFU
#define CRC32_POLYNOMIAL            0x04C11DB7U
#define CRC32_XOR                   0xFFFFFFFFU
/*
 ********************************************************************************************************
 *                                            LOCAL TABLE
 ********************************************************************************************************
 */

#if (CRC_8_MODE == CRC_8_TABLE)
uint8 CRC8TABLE[256] = {
    0x00, 0x1D, 0x3A, 0x27, 0x74, 0x69, 0x4E, 0x53, 0xE8, 0xF5, 0xD2, 0xCF, 0x9C,0x81, 0xA6, 0xBB, 
    0xCD, 0xD0, 0xF7, 0xEA, 0xB9, 0xA4, 0x83, 0x9E, 0x25, 0x38,0x1F, 0x02, 0x51, 0x4C, 0x6B, 0x76, 
    0x87, 0x9A, 0xBD, 0xA0, 0xF3, 0xEE, 0xC9,0xD4, 0x6F, 0x72, 0x55, 0x48, 0x1B, 0x06, 0x21, 0x3C, 
    0x4A, 0x57, 0x70, 0x6D, 0x3E, 0x23, 0x04, 0x19, 0xA2, 0xBF, 0x98, 0x85, 0xD6, 0xCB, 0xEC, 0xF1, 
    0x13, 0x0E, 0x29, 0x34, 0x67, 0x7A, 0x5D, 0x40, 0xFB, 0xE6, 0xC1, 0xDC, 0x8F, 0x92, 0xB5, 0xA8, 
    0xDE, 0xC3, 0xE4, 0xF9, 0xAA, 0xB7, 0x90, 0x8D, 0x36, 0x2B, 0x0C, 0x11, 0x42, 0x5F, 0x78, 0x65, 
    0x94, 0x89, 0xAE, 0xB3, 0xE0, 0xFD, 0xDA, 0xC7, 0x7C, 0x61, 0x46, 0x5B, 0x08, 0x15, 0x32, 0x2F, 
    0x59, 0x44, 0x63, 0x7E, 0x2D, 0x30, 0x17, 0x0A, 0xB1, 0xAC, 0x8B, 0x96, 0xC5, 0xD8, 0xFF, 0xE2, 
    0x26, 0x3B, 0x1C, 0x01, 0x52, 0x4F, 0x68, 0x75, 0xCE, 0xD3, 0xF4, 0xE9, 0xBA, 0xA7, 0x80, 0x9D, 
    0xEB, 0xF6, 0xD1, 0xCC, 0x9F, 0x82, 0xA5, 0xB8, 0x03, 0x1E, 0x39, 0x24, 0x77, 0x6A, 0x4D, 0x50, 
    0xA1, 0xBC, 0x9B, 0x86, 0xD5, 0xC8, 0xEF, 0xF2, 0x49, 0x54, 0x73, 0x6E, 0x3D, 0x20, 0x07, 0x1A, 
    0x6C, 0x71, 0x56, 0x4B, 0x18, 0x05, 0x22, 0x3F, 0x84, 0x99, 0xBE, 0xA3, 0xF0, 0xED, 0xCA, 0xD7, 
    0x35, 0x28, 0x0F, 0x12, 0x41, 0x5C, 0x7B, 0x66, 0xDD, 0xC0, 0xE7, 0xFA, 0xA9, 0xB4, 0x93, 0x8E,
    0xF8, 0xE5, 0xC2, 0xDF, 0x8C, 0x91, 0xB6, 0xAB, 0x10, 0x0D, 0x2A, 0x37, 0x64, 0x79, 0x5E, 0x43, 
    0xB2, 0xAF, 0x88, 0x95, 0xC6, 0xDB, 0xFC, 0xE1, 0x5A, 0x47, 0x60, 0x7D, 0x2E, 0x33, 0x14, 0x09, 
    0x7F, 0x62, 0x45, 0x58, 0x0B, 0x16, 0x31, 0x2C, 0x97, 0x8A, 0xAD, 0xB0, 0xE3, 0xFE, 0xD9, 0xC4};

#endif

#if (CRC_8H2F_MODEE == CRC_8H2F_TABLE)
uint8 CRC8H2FTABLE[256] = { 
    0x00, 0x2F, 0x5E, 0x71, 0xBC, 0x93, 0xE2, 0xCD, 0x57, 0x78, 0x09, 0x26, 0xEB, 0xC4, 0xB5, 0x9A, 
    0xAE, 0x81, 0xF0, 0xDF, 0x12, 0x3D, 0x4C, 0x63, 0xF9, 0xD6, 0xA7, 0x88, 0x45, 0x6A, 0x1B, 0x34, 
    0x73, 0x5C, 0x2D, 0x02, 0xCF, 0xE0, 0x91, 0xBE, 0x24, 0x0B, 0x7A, 0x55, 0x98, 0xB7, 0xC6, 0xE9, 
    0xDD, 0xF2, 0x83, 0xAC, 0x61, 0x4E, 0x3F, 0x10, 0x8A, 0xA5, 0xD4, 0xFB, 0x36, 0x19, 0x68, 0x47, 
    0xE6, 0xC9, 0xB8, 0x97, 0x5A, 0x75, 0x04, 0x2B, 0xB1, 0x9E, 0xEF, 0xC0, 0x0D, 0x22, 0x53, 0x7C, 
    0x48, 0x67, 0x16, 0x39, 0xF4, 0xDB, 0xAA, 0x85, 0x1F, 0x30, 0x41, 0x6E, 0xA3, 0x8C, 0xFD, 0xD2,
    0x95, 0xBA, 0xCB, 0xE4, 0x29, 0x06, 0x77, 0x58, 0xC2, 0xED, 0x9C, 0xB3, 0x7E, 0x51, 0x20, 0x0F, 
    0x3B, 0x14, 0x65, 0x4A, 0x87, 0xA8, 0xD9, 0xF6, 0x6C, 0x43, 0x32, 0x1D, 0xD0, 0xFF, 0x8E, 0xA1, 
    0xE3, 0xCC, 0xBD, 0x92, 0x5F, 0x70, 0x01, 0x2E, 0xB4, 0x9B, 0xEA, 0xC5, 0x08, 0x27, 0x56, 0x79, 
    0x4D, 0x62, 0x13, 0x3C, 0xF1, 0xDE, 0xAF, 0x80, 0x1A, 0x35, 0x44, 0x6B, 0xA6, 0x89, 0xF8, 0xD7, 
    0x90, 0xBF, 0xCE, 0xE1, 0x2C, 0x03, 0x72, 0x5D, 0xC7, 0xE8, 0x99, 0xB6, 0x7B, 0x54, 0x25, 0x0A, 
    0x3E, 0x11, 0x60, 0x4F, 0x82, 0xAD, 0xDC, 0xF3, 0x69, 0x46, 0x37, 0x18, 0xD5, 0xFA, 0x8B, 0xA4, 
    0x05, 0x2A, 0x5B, 0x74, 0xB9, 0x96, 0xE7, 0xC8, 0x52, 0x7D, 0x0C, 0x23, 0xEE, 0xC1, 0xB0, 0x9F, 
    0xAB, 0x84, 0xF5, 0xDA, 0x17, 0x38, 0x49, 0x66, 0xFC, 0xD3, 0xA2, 0x8D, 0x40, 0x6F, 0x1E, 0x31, 
    0x76, 0x59, 0x28, 0x07, 0xCA, 0xE5, 0x94, 0xBB, 0x21, 0x0E, 0x7F, 0x50, 0x9D, 0xB2, 0xC3, 0xEC,
    0xD8, 0xF7, 0x86, 0xA9, 0x64, 0x4B, 0x3A, 0x15, 0x8F, 0xA0, 0xD1, 0xFE, 0x33, 0x1C, 0x6D, 0x42};
#endif


#if (CRC_16_MODE == CRC_16_TABLE)

uint16 CRC16TABLE[256] ={ 
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0};
#endif

#if (CRC_32_MODE == CRC_32_TABLE)
uint32 CRC32TABLE[256] = { 
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005, 
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD, 
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75, 
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD, 
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5, 
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D, 
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95, 
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D, 
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072, 
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA, 
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02, 
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA, 
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692, 
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A, 
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2, 
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A, 
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB, 
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53, 
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B, 
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623, 
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B, 
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3, 
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B, 
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3, 
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C, 
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24, 
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC, 
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654, 
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C, 
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4, 
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C, 
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4};

#endif


/*
 ********************************************************************************************************
 *                                       Local function declarations 
 ********************************************************************************************************
 */



uint8 Crc_CalculateCRC8_RUNTIME(const uint8* Data, uint32 Length, uint8 Crc);
uint8 Crc_CalculateCRC8_TABLE(const uint8* Data, uint32 Length, uint8 Crc);
uint8 Crc_CalculateCRC8_HARDWARE(const uint8* Data, uint32 Length, uint8 Crc);


uint8 Crc_CalculateCRC8H2F_RUNTIME(const uint8* Data, uint32 Length, uint8 Crc);
uint8 Crc_CalculateCRC8H2F_TABLE(const uint8* Data, uint32 Length, uint8 Crc);
uint8 Crc_CalculateCRC8H2F_HARDWARE(const uint8* Data, uint32 Length, uint8 Crc);


uint16 Crc_CalculateCRC16_RUNTIME(const uint8* Data, uint32 Length, uint16 Crc);
uint16 Crc_CalculateCRC16_TABLE(const uint8* Data, uint32 Length, uint16 Crc);
uint16 Crc_CalculateCRC16_HARDAWRE(const uint8* Data, uint32 Length, uint16 Crc);


uint32 Crc_CalculateCRC32_RUNTIME(const uint8* Data, uint32 Length, uint32 Crc);
uint32 Crc_CalculateCRC32_TABLE(const uint8* Data, uint32 Length, uint32 Crc);
uint32 Crc_CalculateCRC32_HARDWARE(const uint8* Data, uint32 Length, uint32 Crc);






#if ((CRC_8_MODE == CRC_8_RUNTIME) || (CRC_8H2F_MODE == CRC_8H2F_RUNTIME))
static uint8 Crc_CalculateCRC8_RUNTIME(const uint8* Data, uint32 Length, uint8 Crc)
{
    uint8 remainder = Crc;
    uint8 bit;
    uint8 topbit = 0x80;
    uint8 poly = CRC8_POLYNOMIAL;

    for (uint32 byte = 0; byte < Length; byte++) {
        remainder ^= *(Data++);
        for (bit = 8; bit > 0; bit--) {
            if (remainder & topbit) {
                remainder = (uint8)(remainder << 1) ^ poly;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder;
}

#endif



#if (CRC_8_MODE == CRC_8_TABLE)
static uint8 Crc_CalculateCRC8_TABLE(const uint8* Crc_DataPtr, uint32 Length, uint8 Crc)
{
  uint32 loopCnt;
  for(loopCnt = 0; loopCnt < Length; loopCnt++) {
   	Crc = CRC8TABLE[(Crc ^ Crc_DataPtr[loopCnt])];
  }
  return Crc;	
}

#endif

#if (CRC_8_MODE == CRC_8_HARDWARE)
static uint8 Crc_CalculateCRC8_HARDWARE(const uint8* Crc_DataPtr, uint32 Length, uint8 Crc)
{
    /* 请使用对应的硬件CRC接口 */
}
#endif 



#if (CRC_8H2F_MODE == CRC_8H2F_RUNTIME)
static uint8 Crc_CalculateCRC8H2F_RUNTIME(const uint8* Crc_DataPtr, uint32 Length, uint8 Crc)
{
    uint8 remainder = Crc;
    uint8 bit;
    uint8 topbit = 0x80;
    uint8 poly = CRC8_H2F_POLYNOMIAL;
    for (uint32 byte = 0; byte < Length; byte++) {
        remainder ^= *(Crc_DataPtr++);
        for (bit = 8; bit > 0; bit--) {
            if (remainder & topbit) {
                remainder = (uint8)(remainder << 1) ^ poly;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder; 
}
#endif

#if (CRC_8H2F_MODE == CRC_8H2F_TABLE)
static uint8 Crc_CalculateCRC8H2F_TABLE(const uint8* Crc_DataPtr, uint32 Length, uint8 Crc)
{
    uint32 loopCnt;
    for(loopCnt = 0; loopCnt < Length; loopCnt++) {
        Crc = CRC8H2FTABLE[(Crc ^ Crc_DataPtr[loopCnt])];
        Crc_DataPtr++;
    }
    return Crc;    
}
#endif


#if (CRC_16_MODE == CRC_16_RUNTIME)
static uint16 Crc_CalculateCRC16_RUNTIME(const uint8* Crc_DataPtr, uint32 Length, uint16 Crc)
{
    uint16 remainder = Crc;
    uint8  bit;
    uint16 topbit = 0x8000;
    uint16 poly = CRC16_CCITT_POLYNOMIAL；
    for (uint32 byte = 0; byte < Length; byte++) {
        remainder ^= (*(Crc_DataPtr++) << 8);
        for (bit = 8; bit > 0; bit--) {
            if (remainder & topbit) {
                remainder = (uint16)(remainder << 1) ^ poly;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return remainder; 
}

#endif 

#if (CRC_16_MODE == CRC_16_TABLE)
static uint16 Crc_CalculateCRC16_TABLE(const uint8* Crc_DataPtr, uint32 Length, uint16 Crc)
{
  uint32 loopCnt;
  for(loopCnt = 0; loopCnt < Length; loopCnt++) {
    Crc = (Crc << 8) ^ CRC16TABLE[(uint8)((Crc >> 8) ^ Crc_DataPtr[loopCnt])];
  }
  return Crc;    
}

#endif

#if (CRC_16_MODE == CRC_16_HARDWARE)
uint16 Crc_CalculateCRC16_HARDWARE(const uint8* Crc_DataPtr, uint32 Length, uint8 Crc)
{

}
#endif



#if (CRC_32_MODE == CRC_32_RUNTIME)
static INLINE uint32 reflectResult(uint32 data)
{
    uint32 reflection = 0x00000000U;
    uint8 bit;
    for (bit = 0; bit < 32; bit++) {
        if (data & 0x01U) {
            reflection |= (1U << (31 - bit));
        }   
        data = data >> 1;
    }
    return reflection;
}

static INLINE uint8 reflectInData(uint8 data)
{
    uint8 reflection = 0x00;
    uint8 bit;
    for (bit = 0; bit < 8; bit++) {
        if (data & 0x01U) {
            reflection |= (1U << (7-bit));
        }
        data = (data >> 1);
    }
    return reflection;
}



static uint32 Crc_CalculateCRC32_RUNTIME(const uint8* Data, uint32 Length, uint32 Crc)
{
    uint32 remainder = Crc;
    uint8  bit;
    uint32  topbit = 0x80000000U;
    for (uint32 byte = 0; byte < nBytes; byte++) {
        remainder ^= (reflectInData(*(message++)) << 24);
        for (bit = 8; bit > 0; bit--) {
            if (remainder & topbit) {
                remainder = (remainder << 1) ^ CRC32_POLYNOMIAL;
            } else {
                remainder = (remainder << 1);
            }
        }
    }
    return reflectResult(remainder); 
}

#endif


#if (CRC_32_MODE == CRC_32_RUNTIME)
static uint32 Crc_CalculateCRC32_TABLE(const uint8* Crc_DataPtr, uint32 Length, uint32 Crc)
{
    for( uint32 loopCnt = 0; loopCnt < Length; loopCnt++) {
        Crc = ((Crc >> 8) & 0x00FFFFFFU) ^ CRC32TABLE[(Crc ^ *Crc_DataPtr) & 0xFFU];
        Crc_DataPtr++;
    }
    return  Crc;
}

#endif


/*
 ********************************************************************************************************
 *                                            Moudle Api
 ********************************************************************************************************
 */


uint8 Crc_CalculateCRC8(const uint8* Crc_DataPtr, uint32 Crc_Length, uint8 Crc_StartValue8, boolean Crc_IsFirstCall)
{
    uint8 initial = 0xFF, xor = 0xFF;
    uint8 crc = 0x00;
    if(Crc_DataPtr != CRC_NULL_PTR) {
        crc = Crc_IsFirstCall ? CRC8_START_VALUE : Crc_StartValue8;
    }
#if (CRC_8_MODE == CRC_8_RUNTIME)
    crc = Crc_CalculateCRC8_RUNTIME(Crc_DataPtr, Crc_Length, crc);
#elif (CRC_8_MODE == CRC_8_TABLE)
    crc = Crc_CalculateCRC8_TABLE(Crc_DataPtr, Crc_Length, crc);
#else
    #error "CRC calculation in Hardware not supported"
#endif
    return crc ^ xor;    
}

uint8 Crc_CalculateCRC8H2F(const uint8* Crc_DataPtr, uint32 Crc_Length, 
									 uint8 Crc_StartValue8H2F, boolean Crc_IsFirstCall)
{
    uint8 initial = 0xFF, xor = 0xFF;
    uint8 crc = 0x00;
    if (Crc_DataPtr != CRC_NULL_PTR) {
        crc = Crc_IsFirstCall ? CRC8_H2F_START_VALUE : Crc_StartValue8H2F;
    }
#if (CRC_8H2F_MODE == CRC_8_RUNTIME)
    crc = Crc_CalculateCRC8H2F_RUNTIME(Crc_DataPtr, Crc_Length, crc, CRC8_H2F_POLYNOMIAL);
#elif (CRC_8H2F_MODE == CRC_8_TABLE)
    crc = Crc_CalculateCRC8H2F_TABLE(Crc_DataPtr, Crc_Length, crc);
#else
    #error "CRC calculation in Hardware not supported"
#endif
    return crc ^ xor;     
}                        



uint16 Crc_CalculateCRC16(const uint8* Crc_DataPtr, uint32 Crc_Length, uint16 Crc_StartValue16, boolean Crc_IsFirstCall)
{
    uint16 initial = 0xFFFF；
    uint6 xor = CRC16_CCITT_XOR;
    uint16 crc = 0x0000;
    if(Crc_DataPtr != CRC_NULL_PTR) {
         crc = Crc_IsFirstCall ? CRC16_CCITT_START_VALUE : Crc_StartValue16;       
    }
#if (CRC_16_MODE == CRC_16_RUNTIME)
    crc = Crc_CalculateCRC16_RUNTIME(Crc_DataPtr, Crc_Length, crc);
#elif (CRC_16_MODE == CRC_16_TABLE)
    crc = Crc_CalculateCRC16_TABLE(Crc_DataPtr, Crc_Length, crc);
#else
    #error "CRC calculation in Hardware not supported"
#endif
    return crc ^ xor;

}  

uint32 Crc_CalculateCRC32(const uint8* Crc_DataPtr, uint32 Crc_Length, uint32 Crc_StartValue32, boolean Crc_IsFirstCall)
{
    uint32 initial = 0xFFFFFFFF;
    uint32 xor = CRC32_XOR;
    uint32 crc = 0x00000000;
    if(Crc_DataPtr != CRC_NULL_PTR) {
        crc = Crc_IsFirstCall ? CRC32_START_VALUE : Crc_StartValue32;       
    }
    crc = reflect32(crc);
#if (CRC_32_MODE == CRC_32_RUNTIME)
    crc = Crc_CalculateCRC32_RUNTIME(Crc_DataPtr, Crc_Length, crc);
#elif (CRC_32_MODE == CRC_32_TABLE)
    crc = Crc_CalculateCRC32_TABLE(Crc_DataPtr, Crc_Length, crc);
#else
    #error "CRC calculation in Hardware not supported"
#endif
  return reflect32(crc) ^ xor;    
}    



                                    

