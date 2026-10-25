
#ifndef INC_CALIBRATEBASEPITCH_H_
#define INC_CALIBRATEBASEPITCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32u5xx_hal.h"   // für I2C_HandleTypeDef
#include <stdint.h>

/**
 * @brief Kalibriert den Basis-Pitch im Ruhezustand.
 *
 * Liest mehrere Accelerometer-Werte ein, berechnet den Pitch
 * und mittelt über alle Samples, um einen stabilen Basiswert zu erhalten.
 *
 * @param hi2c Zeiger auf einen initialisierten I2C-Handle
 * @return gemittelter Basis-Pitch in Grad
 */
float CalibrateBasePitch(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* INC_CALIBRATEBASEPITCH_H_ */
