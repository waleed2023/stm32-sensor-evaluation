#include "mis2dh.h"

// Globale I2C Handle für den Zugriff von Funktionen
static I2C_HandleTypeDef *mis2dh_hi2c; // Dieser Pointer wird in den Funktionen gesetzt

/**
 * @brief Initialisiert den MIS2DH Accelerometer.
 * @param hi2c Pointer zum I2C Handle (z.B. &hi2c2).
 * @retval HAL Status
 */
HAL_StatusTypeDef MIS2DH_Init(I2C_HandleTypeDef *hi2c) {
    mis2dh_hi2c = hi2c; // Speichere den übergebenen Handle

    uint8_t ctrl_reg1_data = 0;
    HAL_StatusTypeDef status;

    // CTRL_REG1 konfigurieren:
    // ODR = 100 Hz (Bit 7:4 = 0101 = 0x50), Normal Mode (Bit 3 = 0), X,Y,Z Enable (Bit 2:0 = 111 = 0x07)
    // Ergibt 0x57.
    ctrl_reg1_data = 0x57;

    // Schreibe Konfigurations-Byte in CTRL_REG1
    status = HAL_I2C_Mem_Write(mis2dh_hi2c, MIS2DH_I2C_ADDR_8BIT_W, MIS2DH_CTRL_REG1, I2C_MEMADD_SIZE_8BIT, &ctrl_reg1_data, 1, 100);
    if (status != HAL_OK) {
        return status;
    }

    return HAL_OK;
}

/**
 * @brief Liest die Sensor-ID (WHO_AM_I Register).
 * @param hi2c Pointer zum I2C Handle.
 * @param id Pointer zur Speicherung der ID.
 * @retval HAL Status
 */
HAL_StatusTypeDef MIS2DH_ReadID(I2C_HandleTypeDef *hi2c, uint8_t *id) {
    mis2dh_hi2c = hi2c; // Speichere den übergebenen Handle
    return HAL_I2C_Mem_Read(mis2dh_hi2c, MIS2DH_I2C_ADDR_8BIT_R, MIS2DH_WHO_AM_I_REG, I2C_MEMADD_SIZE_8BIT, id, 1, 100);
}

/**
 * @brief Liest die X, Y und Z Beschleunigungswerte.
 * @param hi2c Pointer zum I2C Handle.
 * @param x Pointer zur Speicherung des X-Wertes.
 * @param y Pointer zur Speicherung des Y-Wertes.
 * @param z Pointer zur Speicherung des Z-Wertes.
 * @retval HAL Status
 */
HAL_StatusTypeDef MIS2DH_ReadXYZ(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z) {
    mis2dh_hi2c = hi2c; // Speichere den übergebenen Handle
    uint8_t raw_data[6];
    HAL_StatusTypeDef status;

    // Lese 6 Bytes beginnend von OUT_X_L (X_L, X_H, Y_L, Y_H, Z_L, Z_H)
    // Das höchstwertige Bit (MSB) des Register-Addresses (0x80 = 1000 0000 binär)
    // wird gesetzt, um den Auto-Inkrement-Modus zu aktivieren. Das bedeutet,
    // der Sensor inkrementiert die Registeradresse automatisch nach jedem Byte.
    status = HAL_I2C_Mem_Read(mis2dh_hi2c, MIS2DH_I2C_ADDR_8BIT_R, MIS2DH_OUT_X_L | 0x80, I2C_MEMADD_SIZE_8BIT, raw_data, 6, 100);
    if (status != HAL_OK) {
        return status;
    }

    // Werte zusammensetzen (Low Byte | High Byte << 8)
    // Die Werte sind 16-bit signed, daher int16_t cast.
    *x = (int16_t)((raw_data[1] << 8) | raw_data[0]);
    *y = (int16_t)((raw_data[3] << 8) | raw_data[2]);
    *z = (int16_t)((raw_data[5] << 8) | raw_data[4]);

    return HAL_OK;
}
