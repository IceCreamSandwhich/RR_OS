#pragma once

#include "led_indicator.h"


#define BRIGHTNESS 1
#define LED_PIN 21
#define LED_FREQ 10 * 1000 * 1000

#define CONNECTED_COLOR 0x0000FF
#define INDEPENDENT_COLOR 0xFF0000

void initialise_led();

void set_led_color(int32_t color);