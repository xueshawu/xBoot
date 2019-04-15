#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H


#define DIO_ERR		(-1)
#define DIO_OK		(1)


typedef enum
{

	GPIOFSM_INIT = 0,
	FAULT_LAMP = 1,
	RUNNING_LAMP = 2,
	GPIOFSM_PENDING = 0xFF,
}DioChannelType;








extern void Bsp_Gpio_Init(void);
extern void Bsp_Gpio_DioWrtie(DioChannelType Channel);
extern void Bsp_Gpio_DioRead(DioChannelType Channel);








#else
	#error "Bsp_Gpio.h has been define"


#endif
/*****************************************************************************/
/*                  		end of the file                                  */
/*****************************************************************************/

