#include <xmega/clock/clock.h>
#include <avr/interrupt.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* IO library includes. */
#include "FreeRTOS_IO.h"
#include "IOUtils_Common.h"
#include "FreeRTOS_uart.h"

#include "xmega_base_board.h"

///* The bits in the FIFOLVL register that represent the Tx Fifo level. */
//#define uartTX_FIFO_LEVEL_MASK		( 0xf00UL )
//
///* The TEMT bit in the line status register. */
//#define uartTX_BUSY_MASK			( 1UL << 6UL )
//
///*-----------------------------------------------------------*/
//
///*
// * Write as many characters as possible from *ppucBuffer into the UART FIFO.
// */
//static inline size_t prvFillFifoFromBuffer( LPC_UART_TypeDef * const pxUART, uint8_t **ppucBuffer, const size_t xTotalBytes );

/*-----------------------------------------------------------*/

/* Stores the transfer control structures that are currently in use by the
supported UART ports. */
static Transfer_Control_t *pxTxTransferControlStructs[ boardNUM_UARTS ] = { NULL };
static Transfer_Control_t *pxRxTransferControlStructs[ boardNUM_UARTS ] = { NULL };

///* Stores the IRQ numbers of the supported UART ports. */
//static const IRQn_Type xIRQ[] = { UART0_IRQn, UART1_IRQn, UART2_IRQn, UART3_IRQn };
//
///*-----------------------------------------------------------*/
//
portBASE_TYPE FreeRTOS_UART_open( Peripheral_Control_t * const pxPeripheralControl )
{
  USART_t * const pxUART = ( USART_t * const ) diGET_PERIPHERAL_BASE_ADDRESS( pxPeripheralControl );
  portBASE_TYPE xReturn;
  const uint8_t cPeripheralNumber = diGET_PERIPHERAL_NUMBER( pxPeripheralControl );
  pxPeripheralControl->read = FreeRTOS_UART_read;
  pxPeripheralControl->write = FreeRTOS_UART_write;
  pxPeripheralControl->ioctl = FreeRTOS_UART_ioctl;

  /* Setup the pins for the UART being used. */
  taskENTER_CRITICAL();
  {
    boardCONFIGURE_UART_PINS( cPeripheralNumber, xPinConfig );

    // async mode, no parity, 1 stop bit, 8 data bits
    pxUART->CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;

    pxUART->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
  }
  taskEXIT_CRITICAL();

  xReturn = pdPASS;

  return xReturn;
}
/*-----------------------------------------------------------*/

size_t FreeRTOS_UART_write( Peripheral_Descriptor_t const pxPeripheral, const void *pvBuffer, const size_t xBytes )
{
  (void)pxPeripheral;
  (void)pvBuffer;
  (void)xBytes;
Peripheral_Control_t * const pxPeripheralControl = ( Peripheral_Control_t * const ) pxPeripheral;
size_t xReturn = 0U;
USART_t * const pxUART = ( USART_t * const ) diGET_PERIPHERAL_BASE_ADDRESS( ( ( Peripheral_Control_t * const ) pxPeripheral ) );
int8_t cPeripheralNumber;

	if( diGET_TX_TRANSFER_STRUCT( pxPeripheralControl ) == NULL )
	{
		#if ioconfigUSE_UART_POLLED_TX == 1
		{
#error "Not implemented"
		}
		#endif /* ioconfigUSE_UART_POLLED_TX */
	}
	else
	{
		/* Remember which transfer control structure is being used.
		The Tx interrupt will use this to continue to write data to the
		Tx FIFO/UART until the length member of the structure reaches
		zero. */
		cPeripheralNumber = diGET_PERIPHERAL_NUMBER( pxPeripheralControl );
		pxTxTransferControlStructs[ cPeripheralNumber  ] = diGET_TX_TRANSFER_STRUCT( pxPeripheralControl );

		switch( diGET_TX_TRANSFER_TYPE( pxPeripheralControl ) )
		{
			case ioctlUSE_ZERO_COPY_TX :

				#if ioconfigUSE_UART_ZERO_COPY_TX == 1
				{
#error "Not implemented"
				}
				#endif /* ioconfigUSE_UART_ZERO_COPY_TX */
				break;


			case ioctlUSE_CHARACTER_QUEUE_TX :

				#if ioconfigUSE_UART_TX_CHAR_QUEUE == 1
				{
					/* The queue allows multiple tasks to attempt to write
					bytes, but ensures only the highest priority of these tasks
					will actually succeed.  If two tasks of equal priority
					attempt to write simultaneously, then the application must
					ensure mutual exclusion, as time slicing could result in
					the strings being sent to the queue being interleaved. */
					ioutilsBLOCKING_SEND_CHARS_TO_TX_QUEUE
						(
							pxPeripheralControl,
							(pxUART->STATUS & USART_DREIF_bm),  /* Peripheral busy condition. */
              {pxUART->DATA = ucChar; pxUART->CTRLA |= (((pxUART->CTRLA & 0x30) >> 4) << USART_DREINTLVL_gp);},/* Peripheral write function. */
							( ( uint8_t * ) pvBuffer ),			/* Data source. */
							xBytes, 							/* Number of bytes to be written. */
							xReturn );
				}
				#endif /* ioconfigUSE_UART_TX_CHAR_QUEUE */
				break;


			default :

				/* Other methods can be implemented here.  For now set the
				stored transfer structure back to NULL as nothing is being
				sent. */
				configASSERT( xReturn );
				pxTxTransferControlStructs[ cPeripheralNumber ] = NULL;

				/* Prevent compiler warnings when the configuration is set such
				that the following parameters are not used. */
				( void ) pvBuffer;
				( void ) xBytes;
				( void ) pxUART;
				break;
		}
	}

	return xReturn;
}
///*-----------------------------------------------------------*/
//
size_t FreeRTOS_UART_read( Peripheral_Descriptor_t const pxPeripheral, void * const pvBuffer, const size_t xBytes )
{
  (void)pxPeripheral;
  (void)pvBuffer;
  (void)xBytes;
Peripheral_Control_t * const pxPeripheralControl = ( Peripheral_Control_t * const ) pxPeripheral;
size_t xReturn = 0U;
USART_t * const pxUART = ( USART_t * const ) diGET_PERIPHERAL_BASE_ADDRESS( ( ( Peripheral_Control_t * const ) pxPeripheral ) );

	if( diGET_RX_TRANSFER_STRUCT( pxPeripheralControl ) == NULL )
	{
		#if ioconfigUSE_UART_POLLED_RX == 1
		{
#error "Not implemented"
		}
		#endif /* ioconfigUSE_UART_POLLED_RX */
	}
	else
	{
		/* Sanity check the array index. */
		configASSERT( diGET_PERIPHERAL_NUMBER( pxPeripheralControl ) < ( int8_t ) ( sizeof( xIRQ ) / sizeof( IRQn_Type ) ) );

		switch( diGET_RX_TRANSFER_TYPE( pxPeripheralControl ) )
		{
			case ioctlUSE_CIRCULAR_BUFFER_RX :

				#if ioconfigUSE_UART_CIRCULAR_BUFFER_RX == 1
				{
#error "Not implemented";
				}
				#endif /* ioconfigUSE_UART_CIRCULAR_BUFFER_RX */
				break;


			case ioctlUSE_CHARACTER_QUEUE_RX :

				#if ioconfigUSE_UART_RX_CHAR_QUEUE == 1
				{
					/* The queue allows multiple tasks to attempt to read
					bytes, but ensures only the highest priority of these
					tasks will actually receive bytes.  If two tasks of equal
					priority attempt to read simultaneously, then the
					application must ensure mutual exclusion, as time slicing
					could result in the string being received being partially
					received by each task. */
					xReturn = xIOUtilsReceiveCharsFromRxQueue( pxPeripheralControl, ( uint8_t * ) pvBuffer, xBytes );
				}
				#endif /* ioconfigUSE_UART_RX_CHAR_QUEUE */
				break;


			default :

				/* Other methods can be implemented here. */
				configASSERT( xReturn );

				/* Prevent compiler warnings when the configuration is set such
				that the following parameters are not used. */
				( void ) pvBuffer;
				( void ) xBytes;
				( void ) pxUART;
				break;
		}
	}

	return xReturn;
}
///*-----------------------------------------------------------*/
//
//static inline size_t prvFillFifoFromBuffer( LPC_UART_TypeDef * const pxUART, uint8_t **ppucBuffer, const size_t xTotalBytes )
//{
//size_t xBytesSent = 0U;
//
//	/* This function is only used by zero copy transmissions, so mutual
//	exclusion is already taken care of by the fact that a task must first
//	obtain a semaphore before initiating a zero copy transfer.  The semaphore
//	is part of the zero copy structure, not part of the application. */
//	while( ( pxUART->FIFOLVL & uartTX_FIFO_LEVEL_MASK ) != uartTX_FIFO_LEVEL_MASK )
//	{
//		if( xBytesSent >= xTotalBytes )
//		{
//			break;
//		}
//		else
//		{
//			pxUART->THR = **ppucBuffer;
//			( *ppucBuffer )++;
//			xBytesSent++;
//		}
//	}
//
//	return xBytesSent;
//}
///*-----------------------------------------------------------*/
//
portBASE_TYPE FreeRTOS_UART_ioctl( Peripheral_Descriptor_t pxPeripheral, uint32_t ulRequest, void *pvValue )
{
  Peripheral_Control_t * const pxPeripheralControl = ( Peripheral_Control_t * const ) pxPeripheral;
  uint32_t ulValue = ( uint16_t ) pvValue;
  const int8_t cPeripheralNumber = diGET_PERIPHERAL_NUMBER( ( ( Peripheral_Control_t * const ) pxPeripheral ) );
  USART_t * pxUART = ( USART_t * ) diGET_PERIPHERAL_BASE_ADDRESS( ( ( Peripheral_Control_t * const ) pxPeripheral ) );
  portBASE_TYPE xReturn = pdPASS;

	taskENTER_CRITICAL();
	{
		switch( ulRequest )
		{
			case ioctlUSE_INTERRUPTS :

				if( ulValue == pdFALSE )
				{
          pxUART->CTRLA = 0;
				}
				else
				{
          pxUART->CTRLA = USART_RXCINTLVL_MED_gc;
          PMIC.CTRL |= PMIC_MEDLVLEN_bm;

          pxUART->CTRLB = 0;
          pxUART->CTRLB = USART_RXEN_bm | USART_TXEN_bm;

					/* If the Rx is configured to use interrupts, remember the
					transfer control structure that should be used.  A reference
					to the Tx transfer control structure is taken when a write()
					operation is actually performed. */
					pxRxTransferControlStructs[ cPeripheralNumber ] = pxPeripheralControl->pxRxControl;
				}
				break;


			case ioctlSET_SPEED :
        {
          /* Set up the default UART configuration. */
          uint16_t bsel_value = ((uint32_t)(CLOCK_CPU_FREQ) / (ulValue * 16UL)) - 1;

          pxUART->BAUDCTRLB = (uint8_t)((~USART_BSCALE_gm) & (bsel_value >> 8));
          pxUART->BAUDCTRLA = (uint8_t)(bsel_value);

          pxUART->CTRLB = 0;
          pxUART->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
        }
				break;


			case ioctlSET_INTERRUPT_PRIORITY :

				/* The ISR uses ISR safe FreeRTOS API functions, so the priority
				being set must be lower than (ie numerically larger than)
				configMAX_LIBRARY_INTERRUPT_PRIORITY. */
        if(pxUART->CTRLA != 0)
        {
          pxUART->CTRLA = ((ulValue & 0x03) << USART_RXCINTLVL_gp);
          PMIC.CTRL |= (1 << ((ulValue & 0x03) - 1));
        }
				break;


			default :

				xReturn = pdFAIL;
				break;
		}
	}
	taskEXIT_CRITICAL();

	return xReturn;
}
/*-----------------------------------------------------------*/

static void FreeRTOS_xmega_uart_isr_rxc(USART_t * pxUART, int id)
{
  uint32_t ulReceived;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
Transfer_Control_t *pxTransferStruct;

		pxTransferStruct = pxRxTransferControlStructs[id];
		if( pxTransferStruct != NULL )
		{
			switch( diGET_TRANSFER_TYPE_FROM_CONTROL_STRUCT( pxTransferStruct ) )
			{
				case ioctlUSE_CIRCULAR_BUFFER_RX :

					#if ioconfigUSE_UART_CIRCULAR_BUFFER_RX == 1
					{
#error "Not implemented"
					}
					#endif /* ioconfigUSE_UART_CIRCULAR_BUFFER_RX */
					break;


				case ioctlUSE_CHARACTER_QUEUE_RX :

					#if ioconfigUSE_UART_RX_CHAR_QUEUE == 1
					{
            int s = 0;
						ioutilsRX_CHARS_INTO_QUEUE_FROM_ISR( pxTransferStruct, (s++ == 0), pxUART->DATA, ulReceived, xHigherPriorityTaskWoken );
					}
					#endif /* ioconfigUSE_UART_RX_CHAR_QUEUE */
					break;


				default :

					/* This must be an error.  Force an assert. */
					configASSERT( xHigherPriorityTaskWoken );
					break;
			}
		}
}


static void FreeRTOS_xmega_uart_isr_dre(USART_t * pxUART, int id)
{
  Transfer_Control_t *pxTransferStruct;
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  pxTransferStruct = pxTxTransferControlStructs[id];
  if( pxTransferStruct != NULL )
  {
    switch( diGET_TRANSFER_TYPE_FROM_CONTROL_STRUCT( pxTransferStruct ) )
    {
      case ioctlUSE_ZERO_COPY_TX:

#if ioconfigUSE_UART_ZERO_COPY_TX == 1
#error "Not implemented"
#endif
        break;


      case ioctlUSE_CHARACTER_QUEUE_TX:

#if ioconfigUSE_UART_TX_CHAR_QUEUE == 1
        {
          int s = 0;
          ioutilsTX_CHARS_FROM_QUEUE_FROM_ISR( pxTransferStruct, (s == 0), { pxUART->DATA = ucChar; s = 1; }, xHigherPriorityTaskWoken );

          if(s == 0)
          {
            pxUART->CTRLA &= ~USART_DREINTLVL_gm;
          }

        }
#endif 
        break;


      default :

        configASSERT( xHigherPriorityTaskWoken );
        break;
    }
  }


}

ISR(USARTC0_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTC0, 0x00);
}
ISR(USARTC1_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTC1, 0x01);
}
ISR(USARTD0_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTD0, 0x02);
}
ISR(USARTD1_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTD1, 0x03);
}
ISR(USARTE0_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTE0, 0x04);
}
ISR(USARTE1_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTE1, 0x05);
}
ISR(USARTF0_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTF0, 0x06);
}
ISR(USARTF1_RXC_vect)
{
  FreeRTOS_xmega_uart_isr_rxc(&USARTF1, 0x07);
}


ISR(USARTC0_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTC0, 0x00);
}
ISR(USARTC1_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTC1, 0x01);
}
ISR(USARTD0_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTD0, 0x02);
}
ISR(USARTD1_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTD1, 0x03);
}
ISR(USARTE0_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTE0, 0x04);
}
ISR(USARTE1_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTE1, 0x05);
}
ISR(USARTF0_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTF0, 0x06);
}
ISR(USARTF1_DRE_vect)
{
  FreeRTOS_xmega_uart_isr_dre(&USARTF1, 0x07);
}
