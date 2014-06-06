#ifndef XMEGA_BASE_BOARD_H
#define XMEGA_BASE_BOARD_H

#include <avr/io.h>

#define boardNUM_UARTS				8
#define boardNUM_SSPS	  			4

#define boardAVAILABLE_DEVICES_LIST												\
{																				\
	{ ( const int8_t * const ) "/UARTC0/", eUART_TYPE, ( void * ) &USARTC0 },	\
	{ ( const int8_t * const ) "/UARTC1/", eUART_TYPE, ( void * ) &USARTC1 },	\
	{ ( const int8_t * const ) "/UARTD0/", eUART_TYPE, ( void * ) &USARTD0 },	\
	{ ( const int8_t * const ) "/UARTD1/", eUART_TYPE, ( void * ) &USARTD1 },	\
	{ ( const int8_t * const ) "/UARTE0/", eUART_TYPE, ( void * ) &USARTE0 },	\
	{ ( const int8_t * const ) "/UARTE1/", eUART_TYPE, ( void * ) &USARTE1 },	\
	{ ( const int8_t * const ) "/UARTF0/", eUART_TYPE, ( void * ) &USARTF0 },	\
	{ ( const int8_t * const ) "/UARTF1/", eUART_TYPE, ( void * ) &USARTF1 },	\
	{ ( const int8_t * const ) "/SPIC/", eSSP_TYPE, ( void * ) &SPIC },	\
	{ ( const int8_t * const ) "/SPID/", eSSP_TYPE, ( void * ) &SPID },	\
	{ ( const int8_t * const ) "/SPIE/", eSSP_TYPE, ( void * ) &SPIE },	\
	{ ( const int8_t * const ) "/SPIF/", eSSP_TYPE, ( void * ) &SPIF },	\
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

#define boardCONFIGURE_SSP_PINS( cPeripheralNumber)					\
	switch( ( cPeripheralNumber ) )													\
	{																				\
		case 0:																					\
      PORTC.DIR |= (1 << 7) | (1 << 4) | (1 << 5);\
      PORTC.DIR &= ~(1 << 6);\
      break;\
\
		case 1:																					\
      PORTD.DIR |= (1 << 7) | (1 << 4) | (1 << 5);\
      PORTD.DIR &= ~(1 << 6);\
      break;\
\
		case 2:																					\
      PORTE.DIR |= (1 << 7) | (1 << 4) | (1 << 5);\
      PORTE.DIR &= ~(1 << 6);\
      break;\
\
		case 3:																					\
      PORTF.DIR |= (1 << 7) | (1 << 4) | (1 << 5);\
      PORTF.DIR &= ~(1 << 6);\
      break;\
																					\
		default	:	/* These are either not implemented yet, or not available		\
					on this board.  Force an assert failure. */						\
					configASSERT( ( cPeripheralNumber ) - ( cPeripheralNumber ) );	\
					break;															\
	}


#endif// XMEGA_BASE_BOARD_H
