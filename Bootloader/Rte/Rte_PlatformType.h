#ifndef _RTE_PLATFORMTYPE_H


#define _RTE_PLATFORMTYPE_H

#ifndef 	TRUE	
	#define TRUE		1

#else 
	#error "TRUE has been define"
#endif

#ifndef FALSE
	#define FALSE		0
	
#else 
	#error "FALSE has been define"
#endif


#ifndef  NULL_PTR
	#define NULL_PTR	((void *)0)

#else 
	#error "NULL_PTR has been define"

#endif







typedef unsigned char boolean;
typedef unsigned char uint8;
typedef short int sint8;
typedef unsigned short int	uint16;
typedef short int sint16;
typedef unsigned long int uint32;
typedef long int sint32;
typedef unsigned long long int uint64;
typedef long long int sint64;
typedef float float32;
typedef double	float64;
typedef uint8 Std_ReturnType;



#endif
/*****************************************************************************/
/*                  		end of the file                                  */
/*****************************************************************************/


