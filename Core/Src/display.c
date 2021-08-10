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

uint8_t inputString[256];
uint8_t buf[NUM_DISPLAYS][6];
uint8_t workingField[NUM_DISPLAYS+1];
uint8_t inDisplay = 0;
uint8_t takt = 0;
uint8_t oldTakt = 0;
uint8_t sec = 0;
uint8_t inputStrSize = 0;
uint8_t runningStrEnable = 0;




void running_str (void)
{
    if (runningStrEnable == 1)
    {
        put_char(inputString[inDisplay]);
    }
}
void put_char (uint8_t ch)
{
    HAL_TIM_Base_Stop_IT(&htim2);
    if (inDisplay != 0)
    {
        memcpy(buf[inDisplay] , buf[inDisplay - 1], 5);
    }
    for (uint8_t i = 1; i < 6; i++)
    {
        buf[inDisplay][i] = ~ (Font5x7[((ch - 32) * 5) + i - 1])  ;
    }
    inDisplay++;
    HAL_TIM_Base_Start_IT(&htim2);

}
void put_string (uint8_t* str, uint8_t size)
{
    if (size > 8) runningStrEnable = 1;
    HAL_TIM_Base_Stop_IT(&htim2);
    inputStrSize = size;
    memcpy(inputString, str, inputStrSize);
    HAL_TIM_Base_Start_IT(&htim2);

}

void initDisplay (void)
{
    memset(buf, 0, sizeof(buf));
    memset(workingField, 0xAA, sizeof(workingField));
    resetDisplay();
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_Base_Start_IT(&htim3);


}
void resetDisplay (void)
{
    inDisplay = 0;
    takt = 0;
    oldTakt = 0;
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(SRCLR_GPIO_Port, SRCLR_Pin, GPIO_PIN_SET);
    memset(buf, 0xFF ,sizeof(buf));
    memset(workingField, 0xFF, sizeof (workingField) -1);
    workingField[NUM_DISPLAYS] = 0xFE;
    HAL_SPI_Transmit(&hspi1, workingField, NUM_DISPLAYS + 1, 100);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_RESET);

}
void updateDisplay(void)
{
    HAL_SPI_Transmit_IT(&hspi1, workingField, NUM_DISPLAYS + 1);
}

void updateField (void)
{
    if (oldTakt != takt)
    {
        oldTakt = takt;
        for (uint8_t i = 0; i < NUM_DISPLAYS; i ++)
        {
            workingField[i] = buf[i][takt];
        }
        workingField[NUM_DISPLAYS] = (uint8_t) ~(1<<takt);
        updateDisplay();
    }
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM2)
    {
        if (takt == 5) takt = 0;
        takt++;

    }
    if (htim->Instance == TIM3)
    {
        sec++;
        running_str();
    }

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_RESET);
}