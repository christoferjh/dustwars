#ifndef Oo_DRIVNING_H_oO
#define Oo_DRIVNING_H_oO

#define MOTOR_A_STOP 9135
#define MOTOR_A_BACKWARDS 9250
#define MOTOR_A_FORWARD 9000

#define MOTOR_B_FORWARD 9250
#define MOTOR_B_STOP 9080
#define MOTOR_B_BACKWARDS 9000

void init_drivning();
void set_motor_A(int value);
void set_motor_B(int value);
void stop_motors();
void start_motors();
#endif
