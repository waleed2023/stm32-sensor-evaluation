#ifndef GYRO_CALIBRATION_H
#define GYRO_CALIBRATION_H

#include "stm32u5xx_hal.h"    // für I2C_HandleTypeDef

#define GYRO_SENSITIVITY_DPS_LSB  0.07f
#define GYRO_BIAS_SAMPLES         500

/**
 * @brief  Ermittelt den Bias (Nullpunkt) des Y-Gyros in °/s
 * @param  hi2c Zeiger auf einen initialisierten I2C-Handle
 * @return Mittelwert des Gyro-Bias in °/s
 */
float CalibrateGyroBias(I2C_HandleTypeDef *hi2c);

#endif // GYRO_CALIBRATION_H
