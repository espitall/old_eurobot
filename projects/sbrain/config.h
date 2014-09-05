#ifndef CONFIG_H
#define CONFIG_H

#define SCHED_COM_RECV_PRIORITY        3
#define SCHED_COM_SEND_PRIORITY        2
#define SCHED_ASSERV_PRIORITY          8
#define SCHED_HEARTBEAT_PRIORITY       9

#define MOTORS_PWM_PERIOD             1600

#define COM_ID                  COM_SBRAIN
#define COM_WRITE_COOL_TIME            100    

#define MOTOR_0                 left
#define MOTOR_0_INVERT
#define MOTOR_1                 right
#undef MOTOR_1_INVERT

  
#endif// CONFIG_H
