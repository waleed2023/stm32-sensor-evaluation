#ifndef INC_COMPLEMENTARY_FILTER_H_
#define INC_COMPLEMENTARY_FILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @file complementary_filter.h
 * @brief Complementary Filter for fusing accelerometer and gyroscope data into a stable pitch angle.
 */

/**
 * @brief Initializes the complementary filter state.
 *
 * Should be called once with the initial accelerometer-derived pitch angle.
 *
 * @param accelPitch Initial pitch angle from accelerometer (degrees).
 */
void ComplementaryFilter_Init(float accelPitch);

/**
 * @brief Updates the complementary filter with new sensor data.
 *
 * Merges the gyroscope-based rate integration with the accelerometer-derived angle.
 * Uses a constant blending factor internally.
 *
 * @param accelPitch Current pitch angle from accelerometer (degrees).
 * @param gyroYdps   Current gyroscope Y-axis angular rate (degrees per second).
 * @param dt         Time interval since last update (seconds).
 * @return Fused pitch angle (degrees).
 */
float ComplementaryFilter_Update(float accelPitch, float gyroYdps, float dt);

#ifdef __cplusplus
}
#endif

#endif // INC_COMPLEMENTARY_FILTER_H_
