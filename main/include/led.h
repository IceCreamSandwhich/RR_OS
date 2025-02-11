#pragma once

#include "led_indicator.h"

static led_indicator_handle_t led_handle;

#define BRIGHTNESS 1
#define LED_PIN 21
#define LED_FREQ 10 * 1000 * 1000

void initialise_led();

void set_led_color(int32_t color);