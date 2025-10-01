/**
 * @brief  Kalibriert den Basis Gyro im Ruhezustand.
 * @param  hi2c Zeiger auf einen initialisierten I2C-Handle
 * @return Gemittelter Basis G<ro in Grad
 */

#include "lsm6dsox.h"
#include "stm32u5xx_hal.h"
#include "gyro_calibration.h"

#define GYRO_SENSITIVITY_DPS_LSB  0.07f
#define GYRO_BIAS_SAMPLES         500

float CalibrateGyroBias(I2C_HandleTypeDef *hi2c) {
    const int N = 500;
    int32_t sumY = 0;
    int16_t gx, gy, gz;
    for (int i = 0; i < N; ++i) {
        HAL_Delay(5);
        LSM6DSOX_ReadGyro(hi2c, &gx, &gy, &gz);
        sumY += gy;
    }
    return (sumY / (float)N) * LSM6DSOX_GYRO_SENSITIVITY_DPS_LSB;
}
