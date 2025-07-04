#ifndef MOTION_ANALYSIS_H_
#define MOTION_ANALYSIS_H_

#include "stm32u5xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void Motion_Process(I2C_HandleTypeDef *hi2c, float base_pitch);

#ifdef __cplusplus
}
#endif

#endif /* MOTION_ANALYSIS_H_ */
