#include "lsm6dsox.h"
void LSM6DSOX_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t data;

    // Software reset
    data = 0x01;
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, LSM6DSOX_REG_CTRL3_C, 1, &data, 1, HAL_MAX_DELAY);
    HAL_Delay(100);

    // Configure accelerometer: 104 Hz, ±2g
    data = 0x40;
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, LSM6DSOX_REG_CTRL1_XL, 1, &data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef LSM6DSOX_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buf[6];
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Read(hi2c, LSM6DSOX_I2C_ADDR, LSM6DSOX_REG_OUTX_L_A, 1, buf, 6, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;

    *ax = (int16_t)(buf[1] << 8 | buf[0]);
    *ay = (int16_t)(buf[3] << 8 | buf[2]);
    *az = (int16_t)(buf[5] << 8 | buf[4]);
    return HAL_OK;
}
