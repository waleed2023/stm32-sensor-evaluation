#ifndef __BMI270_H__
#define __BMI270_H__

#include "stm32u5xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

/* 7-bit I2C address (SA0/SDO low => 0x68). HAL erwartet 8-bit shifted */
#define BMI270_I2C_ADDR          (0x68 << 1)

/* Registers (Auszug) */
#define BMI270_REG_CHIP_ID       0x00
#define BMI270_REG_STATUS        0x03
#define BMI270_REG_DATA_8        0x0C  /* ACC X LSB */
#define BMI270_REG_DATA_14       0x12  /* GYR X LSB */
#define BMI270_REG_TEMP0         0x22
#define BMI270_REG_FIFO_LEN0     0x24
#define BMI270_REG_FIFO_DATA     0x26
#define BMI270_REG_ACC_CONF      0x40
#define BMI270_REG_ACC_RANGE     0x41
#define BMI270_REG_GYR_CONF      0x42
#define BMI270_REG_GYR_RANGE     0x43
#define BMI270_REG_FIFO_WTM_0    0x46
#define BMI270_REG_FIFO_WTM_1    0x47
#define BMI270_REG_FIFO_CFG_0    0x48
#define BMI270_REG_FIFO_CFG_1    0x49
#define BMI270_REG_PWR_CONF      0x7C
#define BMI270_REG_PWR_CTRL      0x7D
#define BMI270_REG_CMD           0x7E
#define BMI270_REG_INIT_CTRL       0x59
#define BMI270_REG_INIT_ADDR_0     0x5B
#define BMI270_REG_INIT_ADDR_1     0x5C
#define BMI270_REG_INIT_DATA       0x5E
#define BMI270_REG_INTERNAL_STATUS 0x21

#define BMI270_GYRO_LSB_PER_DPS_2000   16.384f   /* 2000 dps => 16.384 LSB/dps */
#define BMI270_ACCEL_LSB_PER_G_2G      16384.0f  /* ±2g => 16384 LSB/g */

extern const uint8_t  bmi270_maximum_fifo_config_file[];
extern const uint16_t bmi270_maximum_fifo_config_file_len;

/* API */

bool     BMI270_Init(I2C_HandleTypeDef *hi2c);
struct bmi2_dev* BMI270_GetDev(void);
bool bmi_wait_cmd_ready(I2C_HandleTypeDef *hi2c,  uint32_t ms);
void bmi_dump_init_error(I2C_HandleTypeDef *hi2c);
uint16_t BMI270_GetFIFOLengthBytes(I2C_HandleTypeDef *hi2c);
bool     BMI270_ReadFIFOBytes(I2C_HandleTypeDef *hi2c, uint8_t *dst, uint16_t nbytes);

#endif /* __BMI270_H__ */
