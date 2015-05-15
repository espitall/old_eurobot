#ifndef CONFIG_H
#define CONFIG_H


#define TICK_PER_180DEG  (23645500.0 * (3746.0 / 3600) / 180.0)
#define TICK_PER_1M       (10000.0 * 57050.0 / 900 * (2600.0 / 2500.0))

#define ASSERV_DIST_MAX_ACCEL 0.007
#define ASSERV_DIST_MAX_SPEED 2.0
#define ASSERV_ANGU_MAX_ACCEL 0.005
#define ASSERV_ANGU_MAX_SPEED 5.0


#define ASSERV_DIST_KP    50
#define ASSERV_DIST_KD    0
#define ASSERV_DIST_KI    1
#define ASSERV_DIST_IMAX  4000

#define ASSERV_ANGU_KP    250
#define ASSERV_ANGU_KD    0
#define ASSERV_ANGU_KI    10
#define ASSERV_ANGU_IMAX  4000

#define LCD_SCHEDULER_PRIO LOWPRIO
#define POSITION_SCHEDULER_PRIO HIGHPRIO
#define POSITION_TRAJECTORY_PRIO (HIGHPRIO - 1)

#define PCM9685_FREQ_HZ 50

#define DC_MOTORS_TYPE DC_MOTORS_TYPE_L298N

#define ROBOT_X 150
#define ROBOT_Y 185

#define USIR_MASTER

#define BAT_WARNING_THRESHOLD_MV 20000
#define BAT_ERROR_THRESHOLD_MV 19000

#define ROBOT_WEDGING_BACK_OFFSET 105

#define PMI

#define ROBOT SECONDAIRE

#endif// CONFIG_H
