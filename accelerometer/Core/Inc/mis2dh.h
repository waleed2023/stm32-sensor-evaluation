#ifndef INC_MIS2DH_H_
#define INC_MIS2DH_H_

#include "main.h"
#include <stdbool.h>

// MIS2DH I2C Slave Adresse (7-bit Adresse ist 0x18 wenn SA0 pin low ist)
#define MIS2DH_I2C_ADDR_7BIT    0x18

// 8-bit Adressen für HAL-Funktionen (inkl. R/W-Bit im höchstwertigen Bit)
#define MIS2DH_I2C_ADDR_8BIT_W  (MIS2DH_I2C_ADDR_7BIT << 1)       // 0x30 für Schreiben
#define MIS2DH_I2C_ADDR_8BIT_R  ((MIS2DH_I2C_ADDR_7BIT << 1) | 0x01) // 0x31 für Lesen


// Register Adressen
#define MIS2DH_WHO_AM_I_REG     0x0F
#define MIS2DH_WHO_AM_I_VALUE   0x33

#define MIS2DH_CTRL_REG1        0x20
#define MIS2DH_OUT_X_L          0x28
#define MIS2DH_OUT_X_H          0x29
#define MIS2DH_OUT_Y_L          0x2A
#define MIS2DH_OUT_Y_H          0x2B
#define MIS2DH_OUT_Z_L          0x2C
#define MIS2DH_OUT_Z_H          0x2D

// FIFO
#define MIS2DH_CTRL_REG3      0x22
#define MIS2DH_CTRL_REG5      0x24
#define MIS2DH_FIFO_CTRL_REG  0x2E
#define MIS2DH_FIFO_SRC_REG   0x2F


// Funktionen für den Sensor
HAL_StatusTypeDef MIS2DH_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MIS2DH_ReadID(I2C_HandleTypeDef *hi2c, uint8_t *id);
HAL_StatusTypeDef MIS2DH_ReadXYZ(I2C_HandleTypeDef *hi2c, int16_t *x, int16_t *y, int16_t *z);
HAL_StatusTypeDef MIS2DH_ReadFIFO(int16_t *x, int16_t *y, int16_t *z, uint8_t num_samples);
HAL_StatusTypeDef MIS2DH_EnableMotionInterrupt(void);
void MIS2DH_EXTI_Callback(uint16_t GPIO_Pin);
HAL_StatusTypeDef MIS2DH_EnableFIFO(void);
extern volatile bool myflag;

#endif /* INC_MIS2DH_H_ */
