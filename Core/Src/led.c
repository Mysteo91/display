//
// Created by arbin on 29.06.2021.
//

#include "led.h"
#include "main.h"
#include "gpio.h"


uint8_t buf[NUM_DISPLAYS][6];
uint8_t clk = 0;


void initDisplay (void)
{
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_MSI, RCC_MCODIV_8);
    clk = 0;
}
void resetDisplay (void)
{
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    HAL_GPIO_WritePin(RESET_DISPLAY_PORT, RESET_DISPLAY_PIN, GPIO_PIN_SET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(RESET_DISPLAY_PORT, RESET_DISPLAY_PIN, GPIO_PIN_RESET);
    clk = 0;
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}


void put_char (uint8_t ch)
{
    HAL_NVIC_DisableIRQ(EXTI0_IRQn);
    for (uint8_t i = 0; i < 5; i++)
    {
        buf[0][i] = Font5x7[((ch - 32) * 5) + i];
    }
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    resetDisplay();
}
void clock_1 (void)
{
    if (clk > 5) clk = 0;
    if (clk > 0)
    {
        GPIOB -> BSRR = ((uint32_t) buf[0][clk-1] << 8 );
    }
   
}

void clock_0 (void)
{
    if (clk > 0)
    GPIOB -> BRR = ((uint32_t) buf[0][clk] << 8 );
    clk++;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (!READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_0))
    {
        clock_1();
    }
    else
    {
        clock_0();
    }
}