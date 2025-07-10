#include "calibratebasepitch.h"
#include "lsm6dsox.h"
#include "angle.h"

/**
 * @brief  Kalibriert den Basis-Pitch im Ruhezustand.
 * @param  hi2c Zeiger auf einen initialisierten I2C-Handle
 * @return Gemittelter Basis-Pitch in Grad
 */
float CalibrateBasePitch(I2C_HandleTypeDef *hi2c) {
    float sum = 0.0f;
    const int samples = 500;
    for (int i = 0; i < samples; ++i) {
        int16_t ax, ay, az;
        if (LSM6DSOX_ReadAccel(hi2c, &ax, &ay, &az) == HAL_OK) {
            sum += Calculate_Pitch((float)ax, (float)ay, (float)az);
        }
        HAL_Delay(5);
    }
    return sum / (float)samples;
}
