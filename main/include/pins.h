#pragma once
#include "driver/gpio.h"

#ifdef SEED_S3

#define D0 GPIO_NUM_1
#define D1 GPIO_NUM_2
#define D2 GPIO_NUM_3
#define D3 GPIO_NUM_4
#define D4 GPIO_NUM_5
#define D5 GPIO_NUM_6
#define D6 NULL // Connected to U0TXD
#define D7 NULL // Connected to U0RXD
#define D8 GPIO_NUM_7
#define D9 GPIO_NUM_7
#define D10 GPIO_NUM_9

#define A0 D0
#define A1 D1
#define A2 D2
#define A3 D3
#define A9 D9
#define A10 D10

#endif