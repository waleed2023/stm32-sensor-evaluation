#ifndef MOTION_ANALYSIS_H_
#define MOTION_ANALYSIS_H_

#include "stm32u5xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif
// Sensor MKI197VI
void Motion_Process_Improved(I2C_HandleTypeDef *hi2c,
							float basePitch,
                            float gyroBias,
                            uint32_t timestamp_ms);


// Sensor 270
void Motion_Process_270(I2C_HandleTypeDef *hi2c,
							float basePitch,
                            float gyroBias,
                            uint32_t timestamp_ms);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_ANALYSIS_H_ */
