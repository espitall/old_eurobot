#ifndef CONFIG_H
#define CONFIG_H

#define TICK_PER_180DEG   (57400 / 180.0)
#define TICK_PER_1M       10000.0

#define ASSERV_DIST_MAX_ACCEL 100.0
#define ASSERV_DIST_MAX_SPEED 100.0
#define ASSERV_ANGU_MAX_ACCEL 100.0
#define ASSERV_ANGU_MAX_SPEED 100.0

#define ASSERV_DIST_KP    0
#define ASSERV_DIST_KD    0
#define ASSERV_DIST_KI    0
#define ASSERV_DIST_IMAX  0

#define ASSERV_ANGU_KP    0
#define ASSERV_ANGU_KD    0
#define ASSERV_ANGU_KI    0
#define ASSERV_ANGU_IMAX  0

#define LCD_SCHEDULER_PRIO LOWPRIO
#define POSITION_SCHEDULER_PRIO HIGHPRIO
#define POSITION_TRAJECTORY_PRIO (HIGHPRIO - 1)

#define PCM9685_FREQ_HZ 50

#define DC_MOTORS_TYPE DC_MOTORS_TYPE_L298N

#define USIR_MASTER

#define BAT_WARNING_THRESHOLD_MV 20000
#define BAT_ERROR_THRESHOLD_MV 19000

#define ROBOT_X 195
#define ROBOT_Y 360
#define ROBOT_WEDGING_BACK_OFFSET 105



#endif// CONFIG_H
