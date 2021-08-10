//
// Created by arbin on 29.06.2021.
//

#ifndef DISPLAY_H
#define DISPLAY_H
#include "stm32f1xx_hal.h"
#include "font.h"

#define RESET_DISPLAY_PIN GPIO_PIN_1
#define RESET_DISPLAY_PORT GPIOC
#define NUM_DISPLAYS 8

void initDisplay (void);
void resetDisplay (void);
void put_char (uint8_t);
void put_string (uint8_t* str, uint8_t size);
#endif //DISPLAY_LED_H
