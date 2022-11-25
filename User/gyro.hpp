#ifndef gyro_h_included

#include "mpu6050.h"

extern MPU6050_t mpu;
void gyro_init();
void gyro_calibrate();
void gyro_callback();
double getAngle();

#define gyro_h_included
#endif