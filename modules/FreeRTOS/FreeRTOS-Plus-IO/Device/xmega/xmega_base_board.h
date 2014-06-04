#ifndef XMEGA_BASE_BOARD_H
#define XMEGA_BASE_BOARD_H

#include <avr/io.h>

#define boardNUM_UARTS				8

#define boardAVAILABLE_DEVICES_LIST												\
{																				\
	{ ( const int8_t * const ) "/UARTC0/", eUART_TYPE, ( void * ) &USARTC0 },	\
	{ ( const int8_t * const ) "/UARTC1/", eUART_TYPE, ( void * ) &USARTC1 },	\
}

/*******************************************************************************
 * Map the FreeRTOS+IO interface to the xmega specific functions.
 ******************************************************************************/
portBASE_TYPE vFreeRTOS_xmega_PopulateFunctionPointers( const Peripheral_Types_t ePeripheralType, Peripheral_Control_t * const pxPeripheralControl );
#define boardFreeRTOS_PopulateFunctionPointers vFreeRTOS_xmega_PopulateFunctionPointers


/*******************************************************************************
 * Configure port UART port pins to be correct for the wiring of the
 * LPCXpresso base board.
 ******************************************************************************/
#define boardCONFIGURE_UART_PINS( cPeripheralNumber, xPinConfig )					\
	switch( ( cPeripheralNumber ) )													\
	{																				\
		case 0x00	:	                          \
      PORTC.DIRSET = (1 << 3);            \
      break;															\
																					\
		case 0x01	:	                          \
      PORTC.DIRSET = (1 << 7);            \
      break;															\
																					\
		default	:	/* These are either not implemented yet, or not available		\
					on this board.  Force an assert failure. */						\
					configASSERT( ( cPeripheralNumber ) - ( cPeripheralNumber ) );	\
					break;															\
	}



#endif// XMEGA_BASE_BOARD_H
