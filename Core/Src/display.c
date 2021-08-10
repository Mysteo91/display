//
// Created by arbin on 09.08.2021.
//

//
// Created by arbin on 29.06.2021.
//

#include "display.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include <string.h>

uint8_t buf[NUM_DISPLAYS][6];
uint8_t cycle ;
uint8_t workingField[NUM_DISPLAYS+1];
uint8_t inputDisplay = 0;





void initDisplay (void)
{
    memset(buf, 0, sizeof(buf));
    memset(workingField, 0xAA, sizeof(workingField));
    cycle = 0;
    resetDisplay();
    HAL_TIM_Base_Start_IT(&htim2);

}
void resetDisplay (void)
{
    inputDisplay = 0;
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_SET);
    memset(buf, 0xFF ,sizeof(buf));
    memset(workingField, 0x00, sizeof (workingField) -1);
    workingField[NUM_DISPLAYS] = 0xFE;
    HAL_SPI_Transmit(&hspi1, workingField, NUM_DISPLAYS + 1, 100);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_RESET);

}
void updateDisplay(void)
{
     HAL_SPI_Transmit(&hspi1, workingField, NUM_DISPLAYS + 1, 100);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_RESET);
}





uint8_t takt = 0;



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM2)
    {
        if (takt == 6) takt = 0;
        workingField[NUM_DISPLAYS] = (uint8_t) ~(1<<takt);
        updateDisplay();
        takt++;

    }

}