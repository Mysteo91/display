//
// Created by arbin on 29.06.2021.
//

#ifndef DISPLAY_LED_H
#define DISPLAY_LED_H
#include "stm32l4xx_hal.h"
#include "font.h"

#define RESET_DISPLAY_PIN GPIO_PIN_1
#define RESET_DISPLAY_PORT GPIOC
#define NUM_DISPLAYS 1

void initDisplay (void);
void resetDisplay (void);
void put_char (uint8_t);
#endif //DISPLAY_LED_H
