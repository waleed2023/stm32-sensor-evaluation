#include "pitch_utils.h"
#include "angle.h"
#include "lsm6dsox.h"
#include "math.h"

#define NUM_SAMPLES 20

float Get_Base_Pitch(I2C_HandleTypeDef *hi2c) {
    int16_t ax, ay, az;
    float sum = 0.0f;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        while (LSM6DSOX_ReadAccel(hi2c, &ax, &ay, &az) != HAL_OK);
        float pitch = Calculate_Pitch(ax, ay, az);
        sum += pitch;
        HAL_Delay(10);
    }

    float base_pitch = sum / NUM_SAMPLES;

    // Kleiner Wert rund um 0 korrigieren
    if (fabsf(base_pitch) < 0.01f) {
        base_pitch = 0.0f;
    }

    return base_pitch;
}
