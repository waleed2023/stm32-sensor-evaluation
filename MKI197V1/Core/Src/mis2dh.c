#include "mis2dh.h"

static HAL_StatusTypeDef wr(I2C_HandleTypeDef *hi2c, uint16_t a, uint8_t reg, uint8_t val) {
    return HAL_I2C_Mem_Write(hi2c, a, reg, 1, &val, 1, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef rd(I2C_HandleTypeDef *hi2c, uint16_t a, uint8_t reg, uint8_t *val) {
    return HAL_I2C_Mem_Read(hi2c, a, reg, 1, val, 1, HAL_MAX_DELAY);
}

bool MIS2DH_Init(I2C_HandleTypeDef *hi2c, uint16_t a,
                 mis2dh_odr_t odr, mis2dh_fs_t fs, bool enable_drdy_int)
{
    HAL_Delay(5); // boot time ~5 ms  :contentReference[oaicite:10]{index=10}

    // 1) Sanity check WHO_AM_I = 0x33
    uint8_t who=0;
    if (rd(hi2c, a, MIS2DH_REG_WHO_AM_I, &who) != HAL_OK || who != 0x33) return false;  // :contentReference[oaicite:11]{index=11}

    // 2) CTRL2: high-pass filter OFF, filtered data bypassed (default 0x00)  :contentReference[oaicite:12]{index=12}
    if (wr(hi2c, a, MIS2DH_REG_CTRL2, 0x00) != HAL_OK) return false;

    // 3) CTRL5: ensure FIFO disabled (FIFO_EN=0)  :contentReference[oaicite:13]{index=13}
    if (wr(hi2c, a, MIS2DH_REG_CTRL5, 0x00) != HAL_OK) return false;

    // 4) CTRL4: BDU=1, HR=1, FS as requested, BLE=0, ST=00, SIM=0  :contentReference[oaicite:14]{index=14}
    uint8_t ctrl4 = 0x80 /*BDU*/ | 0x08 /*HR*/ | (fs & 0x30);
    if (wr(hi2c, a, MIS2DH_REG_CTRL4, ctrl4) != HAL_OK) return false;

    // 5) CTRL3: optionally route DRDY1 to INT1 pin  :contentReference[oaicite:15]{index=15}
    uint8_t ctrl3 = enable_drdy_int ? 0x08 /*I1_DRDY1*/ : 0x00;
    if (wr(hi2c, a, MIS2DH_REG_CTRL3, ctrl3) != HAL_OK) return false;

    // 6) CTRL6: default (no action). Polarity etc. can be set here if needed.  :contentReference[oaicite:16]{index=16}

    // 7) CTRL1: set ODR + enable all axes (Xen/Yen/Zen=1), normal (LPen=0)  :contentReference[oaicite:17]{index=17}
    uint8_t ctrl1 = (uint8_t)odr | 0x07; // ODR[7:4] | Xen|Yen|
    if (wr(hi2c, a, MIS2DH_REG_CTRL1, ctrl1) != HAL_OK) return false;

    return true;
}

/* Burst read X/Y/Z with auto-increment: set MSB of subaddress to 1 (0x80)  :contentReference[oaicite:18]{index=18} */
HAL_StatusTypeDef MIS2DH_ReadXYZ(I2C_HandleTypeDef *hi2c, uint16_t a,
                                 int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t raw[6];
    uint8_t start = MIS2DH_REG_OUT_X_L | 0x80; // auto-increment
    HAL_StatusTypeDef st = HAL_I2C_Mem_Read(hi2c, a, start, 1, raw, sizeof raw, HAL_MAX_DELAY);
    if (st != HAL_OK) return st;
    *x = (int16_t)((raw[1] << 8) | raw[0]);
    *y = (int16_t)((raw[3] << 8) | raw[2]);
    *z = (int16_t)((raw[5] << 8) | raw[4]);
    return HAL_OK;
}
