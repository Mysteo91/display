//
// Created by arbin on 29.06.2021.
//

#include "led.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"

uint8_t buf[NUM_DISPLAYS][6];
uint8_t clk = 0;


void initDisplay (void)
{
    clk = 0;
    HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1); // генерирует прерывание
}
void resetDisplay (void)
{
    HAL_TIM_OC_Stop_IT(&htim3, TIM_CHANNEL_1); // генерирует прерывание
    HAL_GPIO_WritePin(RESET_DISPLAY_PORT, RESET_DISPLAY_PIN, GPIO_PIN_SET);
    HAL_Delay(2);
    clk = 0;
    HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1); // генерирует прерывание
    HAL_GPIO_WritePin(RESET_DISPLAY_PORT, RESET_DISPLAY_PIN, GPIO_PIN_RESET);
}



void put_char (uint8_t ch)
{
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    for (uint8_t i = 0; i < 5; i++)
    {
        buf[0][i] = (Font5x7[((ch - 32) * 5) + i]) << 1 ;
    }
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    resetDisplay();
}

uint8_t takt = 0;

void clock_1 (void)
{
    if (clk > 0)
        GPIOB -> BRR = ((uint32_t) buf[0][clk-1] << 8 );
    if (clk == 5)
    {
        GPIOC -> BSRR = RESET_DISPLAY_PIN;
    }
    else
    {
        if (takt == 0)
        GPIOB -> BSRR = ((uint32_t) buf[0][clk] << 8 );
    }



   
}



void clock_0 (void)
{
    if (clk == 5 )
    {
        takt ++ ;
        GPIOC -> BRR = RESET_DISPLAY_PIN;
        //GPIOB -> BRR = 0xFF;
        clk = 0;
        if (takt == 4) takt = 0;
    }
    else
    {
       // GPIOB -> BRR = 0xFF;
        clk++;
    }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (READ_BIT(GPIOA->IDR, GPIO_IDR_IDR_6))
    {
        clock_1();
    }
    else
    {
        clock_0();
    }
}