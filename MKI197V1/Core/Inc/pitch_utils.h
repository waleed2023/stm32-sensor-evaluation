#ifndef PITCH_UTILS_H_
#define PITCH_UTILS_H_

#include "stm32u5xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

float Get_Base_Pitch(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* PITCH_UTILS_H_ */
