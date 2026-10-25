#pragma once
#include "stm32u5xx_hal.h"
#include <stdbool.h>


#define MIS2DH_ADDR  (0x18 << 1)
#define MIS2DH_SA0_LOW_ADDR   (0x18 << 1)
#define MIS2DH_SA0_HIGH_ADDR  (0x19 << 1)

/* Registers */
#define MIS2DH_REG_WHO_AM_I     0x0F
#define MIS2DH_REG_CTRL1        0x20
#define MIS2DH_REG_CTRL2        0x21
#define MIS2DH_REG_CTRL3        0x22
#define MIS2DH_REG_CTRL4        0x23
#define MIS2DH_REG_CTRL5        0x24
#define MIS2DH_REG_CTRL6        0x25
#define MIS2DH_REG_STATUS       0x27
#define MIS2DH_REG_OUT_X_L      0x28


/* WHO_AM_I expected = 0x33 (0011 0011) */  // :contentReference[oaicite:8]{index=8}

/* ODR options for CTRL1[7:4] — see Table 28 */
typedef enum {
    MIS2DH_ODR_1HZ    = 0x10,
    MIS2DH_ODR_10HZ   = 0x20,
    MIS2DH_ODR_25HZ   = 0x30,
    MIS2DH_ODR_50HZ   = 0x40,
    MIS2DH_ODR_100HZ  = 0x50,
    MIS2DH_ODR_200HZ  = 0x60,
    MIS2DH_ODR_400HZ  = 0x70,
    MIS2DH_ODR_1344HZ = 0x90,  // HR/Normal 1.344 kHz  :contentReference[oaicite:9]{index=9}
} mis2dh_odr_t;

/* Full-scale for CTRL4[5:4] */
typedef enum {
    MIS2DH_FS_2G  = 0x00,
    MIS2DH_FS_4G  = 0x10,
    MIS2DH_FS_8G  = 0x20,
    MIS2DH_FS_16G = 0x30,
} mis2dh_fs_t;

bool MIS2DH_Init(I2C_HandleTypeDef *hi2c, uint16_t i2c_addr,
                 mis2dh_odr_t odr, mis2dh_fs_t fs, bool enable_drdy_int);

HAL_StatusTypeDef MIS2DH_ReadXYZ(I2C_HandleTypeDef *hi2c, uint16_t i2c_addr,
                                 int16_t *x, int16_t *y, int16_t *z);
