#ifndef DC_MOTORS_H
#define DC_MOTORS_H

#define DC_MOTORS_TYPE_LMD18200T 0
#define DC_MOTORS_TYPE_L298N 1

/*
 * Initialization of the dc motor module
 */
void dcmInit(void);

/*
 * Set the pulse width (between -10000 and 10000)
 */
void dcmSetWidth(unsigned int channel, int width);


#endif// DC_MOTORS_H
