#ifndef UART_COMM_H_
#define UART_COMM_H_

#include "stm32u5xx_hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void UART_Comm_Process(UART_HandleTypeDef *huart);
void SendToPython(float pitch, float delta, float gyro_y);

#ifdef __cplusplus
}
#endif

#endif /* UART_COMM_H_ */
