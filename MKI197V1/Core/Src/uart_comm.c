#include "uart_comm.h"
#include <string.h>
#include <stdio.h>
#include "stm32u5xx_nucleo.h"  // Für BSP_LED

void UART_Comm_Process(UART_HandleTypeDef *huart)
{
    uint8_t rx_buf[32] = {0};

    if (HAL_UART_Receive(huart, rx_buf, sizeof(rx_buf), 100) == HAL_OK)
    {
        printf("UART3-Empfangen: %s\n", rx_buf);

        // LED-Steuerung
        if (strncmp((char*)rx_buf, "LED_ON", 6) == 0) {
            BSP_LED_On(LED_GREEN);
        }
        else if (strncmp((char*)rx_buf, "LED_OFF", 7) == 0) {
            BSP_LED_Off(LED_GREEN);
        }

        // Antwort senden
        char response[] = "ACK\n";
        HAL_UART_Transmit(huart, (uint8_t*)response, strlen(response), HAL_MAX_DELAY);
    }
}

extern UART_HandleTypeDef huart3;

void SendToPython(float pitch, float delta, float gyro_y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Pitch:%.2f,Delta:%.2f,Gyro Y:%.2f\n", pitch, delta, gyro_y);
    HAL_UART_Transmit(&huart3, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}
