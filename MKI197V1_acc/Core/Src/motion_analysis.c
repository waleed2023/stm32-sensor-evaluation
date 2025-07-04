#include "motion_analysis.h"
#include "lsm6dsox.h"
#include "angle.h"
#include <stdio.h>
#include <math.h>
#include "stm32u5xx_nucleo.h"

#define GYRO_THRESHOLD 200.0f
#define PITCH_DELTA_THRESHOLD 30.0f

void Motion_Process(I2C_HandleTypeDef *hi2c, float base_pitch)
{
    //printf("DEBUG: base_pitch uebergeben: %.2f\n", base_pitch);

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    if (LSM6DSOX_ReadAccel(hi2c, &ax, &ay, &az) != HAL_OK ||
        LSM6DSOX_ReadGyro(hi2c, &gx, &gy, &gz) != HAL_OK)
    {
        printf("I2C Fehler\n");
        BSP_LED_Toggle(LED_RED);
        return;
    }

    // Beschleunigung: Pitch berechnen
    float pitch = Calculate_Pitch(ax, ay, az);
    float delta = pitch - base_pitch;

    // Gyro Y in °/s (±2000 dps = 0.07°/s per LSB)
    float gyro_y_dps = gy * 0.07f;

    printf("Pitch: %.2f, Delta: %.2f, Gyro Y: %.2f °/s\n", pitch, delta, gyro_y_dps);

    // --- Kombinierte Entscheidung ---
    if (fabsf(delta) > PITCH_DELTA_THRESHOLD) {
        if (delta > 0) {
            printf("Arm langsam gehoben\n");
            BSP_LED_On(LED_GREEN);
            BSP_LED_Off(LED_RED);
        } else {
            printf("Arm langsam gesenkt\n");
            BSP_LED_On(LED_RED);
            BSP_LED_Off(LED_GREEN);
        }
    }
    else if (fabsf(gyro_y_dps) > GYRO_THRESHOLD) {
        if (gyro_y_dps > 0) {
            printf("Schnelle Bewegung nach unten\n");
            BSP_LED_Toggle(LED_RED);
        } else {
            printf("Schnelle Bewegung nach oben\n");
            BSP_LED_Toggle(LED_GREEN);
        }
    }
    else {
        printf("Arm ruhig\n");
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_RED);
    }
}
