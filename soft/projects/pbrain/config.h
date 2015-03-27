#ifndef CONFIG_H
#define CONFIG_H

#define TICK_PER_180DEG   565694
#define TICK_PER_1M       1051600

#define ASSERV_DIST_MAX_ACCEL 0.1
#define ASSERV_DIST_MAX_SPEED 1.0
#define ASSERV_ANGU_MAX_ACCEL 0.05
#define ASSERV_ANGU_MAX_SPEED 7.0

#define ASSERV_DIST_KP    250
#define ASSERV_DIST_KD    1000
#define ASSERV_DIST_KI    0
#define ASSERV_DIST_IMAX  0

#define ASSERV_ANGU_KP    500
#define ASSERV_ANGU_KD    1000
#define ASSERV_ANGU_KI    0
#define ASSERV_ANGU_IMAX  0

#define LCD_SCHEDULER_PRIO LOWPRIO
#define POSITION_SCHEDULER_PRIO HIGHPRIO
#define POSITION_TRAJECTORY_PRIO (HIGHPRIO - 1)

#define PCM9685_FREQ_HZ 50

#define DC_MOTORS_TYPE DC_MOTORS_TYPE_LMD18200T

#define ROBOT_X 195
#define ROBOT_Y 360

#define USIR_MASTER

#endif// CONFIG_H
