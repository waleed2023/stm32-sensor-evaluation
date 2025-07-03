#ifndef __LSM6DSOX_H__
#define __LSM6DSOX_H__

#include "stm32u5xx_hal.h"

#define LSM6DSOX_I2C_ADDR     (0x6A << 1) // 7-bit shifted
#define LSM6DSOX_REG_CTRL1_XL 0x10
#define LSM6DSOX_REG_CTRL3_C  0x12
#define LSM6DSOX_REG_OUTX_L_A 0x28

#ifdef __cplusplus
extern "C" {
#endif

void LSM6DSOX_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef LSM6DSOX_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *ay, int16_t *az);

#ifdef __cplusplus
}
#endif

#endif /* __LSM6DSOX_H__ */
