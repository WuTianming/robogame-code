#include <gyro.hpp>
#include <i2c.h>
#include <usart.h>
#include <printf.h>

MPU6050_t mpu;
static int consttt = 0;

void gyro_calibrate() {
  return;
  consttt = 0;
  mpu.AngleZ = 0.00;
  HAL_Delay(2000);
  double offset = (mpu.AngleZ / 40) * 81;
  consttt = offset;
  mpu.AngleZ = 0.00;
  char s1[50];
  int len = sprintf(s1, "mpu offset done, consttt = %d\n", consttt);
  HAL_USART_Transmit(&husart1, (const uint8_t *)s1, len, 100);
}

void gyro_init() {
  char s0[] = "mpu before init\n";
  HAL_USART_Transmit(&husart1, (const uint8_t *)s0, sizeof(s0), 100);
  while (MPU6050_Init(&hi2c2) == 1);
  char s[] = "mpu init done\n";
  HAL_USART_Transmit(&husart1, (const uint8_t *)s, sizeof(s), 100);
  gyro_calibrate();
}

void gyro_callback() {
  // tick time = 50ms (tim3)

  // static int consttt = 164;
  // static int consttt = -100;

  char ss[200];
  int len = sprintf(ss, "callback \n");
  HAL_USART_Transmit(&husart1, (const uint8_t *)ss, len, 100);

  // needed when calling HAL I2C from callback handler
  // if (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY) { return; }

  MPU6050_Read_Gyro(&hi2c2, &mpu);
  // MPU6050_Read_All(&hi2c2, &mpu);

  len = sprintf(ss, "%lf\n", getAngle());
  HAL_USART_Transmit(&husart1, (const uint8_t *)ss, len, 100);

  mpu.AngleZ += (((int)mpu.Gyro_Z_RAW - consttt)) / 81;
  // mpu.AngleZ += (((int)mpu.Gyro_X_RAW + 600)) / 81;
  // mpu.AngleZ = mpu.KalmanAngleX * 16;
}

// 0, 90, 180 to the left = positive
double getAngle() {
  return mpu.AngleZ / 32;
}