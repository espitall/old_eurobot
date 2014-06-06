#ifndef ENCODER_MAGNETIC_H
#define ENCODER_MAGNETIC_H

#include <FreeRTOS/module.h> 

/** 
 * Initialisation of encoders, variables
 */
void encoder_magnetic_init(Peripheral_Descriptor_t * spi);

/** 
 * Update encoders values, need to be done quite often
 */
void encoder_magnetic_manage(void);

/** 
 * Extract encoder value.
 */
int32_t encoder_magnetic_get_value(int id);


#endif// ENCODER_MAGNETIC_H
