#ifndef MOTION_ANALYSIS_IMPROVED_H
#define MOTION_ANALYSIS_IMPROVED_H

#include "motion_analysis.h"
#include "lsm6dsox.h"
#include "angle.h"
#include "uart_comm.h"
#include "stm32u5xx_nucleo.h"
#include "complementary_filter.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

// Empfindlichkeiten und Schwellwerte
#define GYRO_SENSITIVITY_DPS_LSB   0.07f    // 0,07 °/s pro LSB bei ±2000 dps
#define SLOW_ANGLE_THRESHOLD_DEG   2.0f     // Minimale Neigungsänderung für langsame Bewegung
#define FAST_GYRO_THRESHOLD_DPS    100.0f   // Schwellwert für schnelle Bewegung (°/s)
#define FAST_GYRO_THRESHOLD_RAW    ((int16_t)(FAST_GYRO_THRESHOLD_DPS / GYRO_SENSITIVITY_DPS_LSB))
#define LED_BLINK_PERIOD_MS       100      // Blink-Intervall für schnelle Bewegungen

typedef enum {
    STATE_IDLE,
    STATE_SLOW_UP,
    STATE_SLOW_DOWN,
    STATE_FAST_UP,
    STATE_FAST_DOWN
} MotionState;

static MotionState currentState = STATE_IDLE;
static uint32_t lastBlinkTime = 0;

// Für Complementary Filter
static bool filterInitialized = false;
static uint32_t filterLastTimestamp = 0;

/**
 * @brief Ermittelt Bewegungstyp und -richtung basierend auf gefiltertem Pitch und Gyro-Daten.
 *
 * - Langsame Bewegungen: gefilterte Winkeländerung > SLOW_ANGLE_THRESHOLD_DEG
 * - Schnelle Bewegungen: Gyro Y > FAST_GYRO_THRESHOLD_DPS
 *
 * LEDs:
 * - STATE_SLOW_UP/DOWN: LED permanent an (grün/rot)
 * - STATE_FAST_UP/DOWN: LED blinkt (grün/rot)
 * - STATE_IDLE: beide LEDs aus
 *
 * @param hi2c Zeiger auf I2C-Handle
 * @param basePitch Referenz-Pitch (Ruhezustand)
 * @param timestamp_ms Laufzeit-Timestamp zur LED-Blinksteuerung
 */
void Motion_Process_Improved(I2C_HandleTypeDef *hi2c, float basePitch, uint32_t timestamp_ms){
    uint16_t samples = LSM6DSOX_GetFIFOLength(hi2c);
    if (!samples) return;

    // 1) Einmaliger Block-Read
    uint16_t bytes = samples * 7;
    uint8_t buf[bytes];
    HAL_I2C_Mem_Read(hi2c, LSM6DSOX_I2C_ADDR, 0x78, 1, buf, bytes, HAL_MAX_DELAY);

    // 2) Lokales Parsen
    float pitchSum = 0, gyroYSum = 0;
    for (uint16_t i = 0; i < samples; ++i) {
        uint8_t tag = buf[i*7 + 0];
        int16_t x = (buf[i*7+2] << 8) | buf[i*7+1];
        int16_t y = (buf[i*7+4] << 8) | buf[i*7+3];
        int16_t z = (buf[i*7+6] << 8) | buf[i*7+5];
        uint8_t type = tag & 0x1F;
        if (type == 0x02 || type == 0x12) {
            pitchSum  += Calculate_Pitch(x, y, z);
        } else {
            gyroYSum  += y * GYRO_SENSITIVITY_DPS_LSB;
        }
    }
    float avgAccelPitch = pitchSum / samples;
    float avgGyroY      = gyroYSum  / samples;

    // Complementary Filter initialisieren (erste Messung)
    if (!filterInitialized) {
        ComplementaryFilter_Init(avgAccelPitch);
        filterLastTimestamp = timestamp_ms;
        filterInitialized = true;
    }

    // Filter-Schritt
    float dt = (timestamp_ms - filterLastTimestamp) / 1000.0f;
    filterLastTimestamp = timestamp_ms;
    float fusedPitch = ComplementaryFilter_Update(avgAccelPitch, avgGyroY, dt);

    float deltaAngle = fusedPitch - basePitch;

    // Bestimme neuen Zustand
    MotionState newState = STATE_IDLE;
    if (abs((int16_t)avgGyroY) >= FAST_GYRO_THRESHOLD_RAW) {
        newState = (avgGyroY > 0) ? STATE_FAST_DOWN : STATE_FAST_UP;
    } else if (fabsf(deltaAngle) >= SLOW_ANGLE_THRESHOLD_DEG) {
        newState = (deltaAngle > 0) ? STATE_SLOW_UP : STATE_SLOW_DOWN;
    }

    // Bei Zustandwechsel Ausgabe & LEDs anpassen
    if (newState != currentState) {
        BSP_LED_Off(LED_GREEN);
        BSP_LED_Off(LED_RED);
        lastBlinkTime = timestamp_ms;

        switch (newState) {
            case STATE_SLOW_UP:    printf("Langsame Bewegung nach oben\n"); break;
            case STATE_SLOW_DOWN:  printf("Langsame Bewegung nach unten\n"); break;
            case STATE_FAST_UP:    printf("Schnelle Bewegung nach oben\n"); break;
            case STATE_FAST_DOWN:  printf("Schnelle Bewegung nach unten\n"); break;
            default: break;
        }
    }

    // LED-Verhalten nach Zustand
    switch (newState) {
        case STATE_SLOW_UP:
            BSP_LED_On(LED_GREEN);
            break;
        case STATE_SLOW_DOWN:
            BSP_LED_On(LED_RED);
            break;
        case STATE_FAST_UP:
            if ((timestamp_ms - lastBlinkTime) >= LED_BLINK_PERIOD_MS) {
                BSP_LED_Toggle(LED_GREEN);
                lastBlinkTime = timestamp_ms;
            }
            break;
        case STATE_FAST_DOWN:
            if ((timestamp_ms - lastBlinkTime) >= LED_BLINK_PERIOD_MS) {
                BSP_LED_Toggle(LED_RED);
                lastBlinkTime = timestamp_ms;
            }
            break;
        default:
            break;
    }

    currentState = newState;

    // Telemetrie-Ausgabe
    //printf("FusedPitch=%.1f°, Δ=%.1f°, GyroY=%.1f°/s, State=%d\n", fusedPitch, deltaAngle, avgGyroY, newState);
    SendToPython(fusedPitch, deltaAngle, avgGyroY);
}

#endif // MOTION_ANALYSIS_IMPROVED_H
