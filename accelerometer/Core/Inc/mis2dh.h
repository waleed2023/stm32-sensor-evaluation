#ifndef INC_MIS2DH_H_
#define INC_MIS2DH_H_

#include "main.h" // Notwendig für HAL-Typen

// MIS2DH I2C Slave Adresse (7-bit Adresse ist 0x18 wenn SA0 pin low ist)
#define MIS2DH_I2C_ADDR_7BIT    0x18

// 8-bit Adressen für HAL-Funktionen (inkl. R/W-Bit im höchstwertigen Bit)
#define MIS2DH_I2C_ADDR_8BIT_W  (MIS2DH_I2C_ADDR_7BIT << 1)       // 0x30 für Schreiben
#define MIS2DH_I2C_ADDR_8BIT_R  ((MIS2DH_I2C_ADDR_7BIT << 1) | 0x01) // 0x31 für Lesen


// Register Adressen aus dem Datenblatt (wie aus deinem Bild)
#define MIS2DH_WHO_AM_I_REG     0x0F
#define MIS2DH_WHO_AM_I_VALUE   0x33 // Erwarteter Wert für MIS2DH (aus Datenblatt)

#define MIS2DH_CTRL_REG1        0x20
#define MIS2DH_OUT_X_L          0x28
#define MIS2DH_OUT_X_H          0x29
#define MIS2DH_OUT_Y_L          0x2A
#define MIS2DH_OUT_Y_H          0x2B
#define MIS2DH_OUT_Z_L          0x2C
#define MIS2DH_OUT_Z_H          0x2D

// Funktionen für den Sensor
HAL_StatusTypeDef MIS2DH_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MIS2DH_ReadID(I2C_HandleTypeDef *hi2c, uint8_t *id);
HAL_StatusTypeDef MIS2DH_ReadXYZ(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z);

#endif /* INC_MIS2DH_H_ */
