#include <stdbool.h>
#include "lsm6dsox.h"

/**
 * @brief Initialize the LSM6DSOX sensor over I2C.
 *
 * Configures software reset, block data update, timestamp,
 * FIFO batch rates, watermark, continuous FIFO mode, routing,
 * high-performance settings, and starts accelerometer & gyroscope.
 *
 * @param hi2c Pointer to a configured I2C handle.
 */
void LSM6DSOX_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t data;

    /* Set FIFO batch rates: Gyro & Accel @ 1.66 kHz */
    data = (0x08 << 4) | 0x08;      // 1667 Hz codes (BDR_GY=1000, BDR_XL=1000)
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, 0x09, 1, &data, 1, HAL_MAX_DELAY);

    /* FIFO watermark level */
    data = 8;                       // LSB
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, 0x07, 1, &data, 1, HAL_MAX_DELAY);
    data = 0x00;                    // MSB
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, 0x08, 1, &data, 1, HAL_MAX_DELAY);

    /* FIFO: timestamp batched + continuous mode */
    data = 0x46;                    // DEC_TS=01, FIFO_MODE=110
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, 0x0A, 1, &data, 1, HAL_MAX_DELAY);

    /* Remove erroneous write to 0x0B (COUNTER_BDR_REG1) */

    /* Start accelerometer @ 1.66 kHz, ±2g */
    data = 0x80;                    // ODR_XL=1000, FS_XL=00
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, 0x10, 1, &data, 1, HAL_MAX_DELAY);

    /* Start gyroscope @ 1.66 kHz, ±2000 dps */
    data = 0x8C;                    // ODR_G=1000, FS_G=11
    HAL_I2C_Mem_Write(hi2c, LSM6DSOX_I2C_ADDR, 0x11, 1, &data, 1, HAL_MAX_DELAY);


    HAL_Delay(100);
}

/**
 * @brief Reads raw accelerometer data over I2C.
 *
 * @param hi2c Pointer to I2C handle.
 * @param ax Pointer to store X-axis value (LSB, MSB).
 * @param ay Pointer to store Y-axis value.
 * @param az Pointer to store Z-axis value.
 * @return HAL status code (HAL_OK on success).
 */
HAL_StatusTypeDef LSM6DSOX_ReadAccel(I2C_HandleTypeDef *hi2c, int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t buf[6];
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(hi2c, LSM6DSOX_I2C_ADDR,
                                             LSM6DSOX_REG_OUTX_L_A, 1,
                                             buf, 6, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;
    *ax = (int16_t)(buf[1] << 8 | buf[0]);
    *ay = (int16_t)(buf[3] << 8 | buf[2]);
    *az = (int16_t)(buf[5] << 8 | buf[4]);
    return HAL_OK;
}

/**
 * @brief Reads raw gyroscope data over I2C.
 *
 * @param hi2c Pointer to I2C handle.
 * @param gx Pointer to store X-axis rate (LSB, MSB).
 * @param gy Pointer to store Y-axis rate.
 * @param gz Pointer to store Z-axis rate.
 * @return HAL status code (HAL_OK on success).
 */
HAL_StatusTypeDef LSM6DSOX_ReadGyro(I2C_HandleTypeDef *hi2c, int16_t *gx, int16_t *gy, int16_t *gz) {
    uint8_t buf[6];
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(hi2c, LSM6DSOX_I2C_ADDR, 0x22,
                                             1, buf, 6, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;
    *gx = (int16_t)(buf[1] << 8 | buf[0]);
    *gy = (int16_t)(buf[3] << 8 | buf[2]);
    *gz = (int16_t)(buf[5] << 8 | buf[4]);
    return HAL_OK;
}

/**
 * @brief Reads a single sample from the FIFO buffer.
 *
 * Extracts the tag byte and three 16-bit axis values.
 *
 * @param hi2c Pointer to I2C handle.
 * @param tag Pointer to store FIFO tag.
 * @param x Pointer to store first axis sample.
 * @param y Pointer to store second axis sample.
 * @param z Pointer to store third axis sample.
 * @return true if read succeeds, false otherwise.
 */
bool LSM6DSOX_ReadFIFOSample(I2C_HandleTypeDef *hi2c,
                             uint8_t *tag,
                             int16_t *x,
                             int16_t *y,
                             int16_t *z) {
    uint8_t buf[7];
    if (HAL_I2C_Mem_Read(hi2c, LSM6DSOX_I2C_ADDR,
                         0x78, 1, buf, 7, HAL_MAX_DELAY) != HAL_OK) {
        return false;
    }
    *tag = buf[0];
    *x   = (int16_t)(buf[2] << 8 | buf[1]);
    *y   = (int16_t)(buf[4] << 8 | buf[3]);
    *z   = (int16_t)(buf[6] << 8 | buf[5]);
    return true;
}

/**
 * @brief Retrieves the number of samples available in FIFO.
 *
 * Reads FIFO_STATUS1/2 registers.
 *
 * @param hi2c Pointer to I2C handle.
 * @return Number of unread FIFO samples (0 on error).
 */
uint16_t LSM6DSOX_GetFIFOLength(I2C_HandleTypeDef *hi2c) {
    uint8_t status[2];
    if (HAL_I2C_Mem_Read(hi2c, LSM6DSOX_I2C_ADDR,
                         0x3A, 1, status, 2, HAL_MAX_DELAY) != HAL_OK) {
        return 0;
    }
    return (uint16_t)(((status[1] & 0x03) << 8) | status[0]);
}

