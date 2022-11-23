#include <gyro.hpp>
#include <i2c.h>
#include <usart.h>

MPU6050_t mpu;

void gyro_init() {
  while (MPU6050_Init(&hi2c2) == 1);
  char s[] = "mpu init done\n";
  HAL_USART_Transmit(&husart1, (const uint8_t *)s, sizeof(s), 100);
  mpu.AngleZ = 0.00;
}

void gyro_callback() {
  // tick time = tim3

  static int consttt = 164;
  MPU6050_Read_Gyro(&hi2c2, &mpu);
  mpu.AngleZ += (((int)mpu.Gyro_Z_RAW - consttt)) / 81;
}

// 0, 90, 180 to the left = positive
double getAngle() {
  return mpu.AngleZ / 32;
}