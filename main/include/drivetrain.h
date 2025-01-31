#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"

#include "driver/gpio.h"
#include "driver/ledc.h"

#define SPEED_MODE LEDC_LOW_SPEED_MODE
#define PWM_FREQ 5000
#define PWM_RES LEDC_TIMER_10_BIT

#define LEFT_MOTOR_A CONFIG_LEFT_MOTOR_A
#define LEFT_MOTOR_B CONFIG_LEFT_MOTOR_B
#define RIGHT_MOTOR_A CONFIG_RIGHT_MOTOR_A
#define RIGHT_MOTOR_B CONFIG_RIGHT_MOTOR_B

ledc_timer_config_t ledc_timer = {
    .speed_mode = SPEED_MODE,
    .duty_resolution = PWM_RES,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = PWM_FREQ,
    .clk_cfg = LEDC_AUTO_CLK,
};

typedef struct motor
{
    gpio_num_t pwma;
    gpio_num_t pwmb;
    ledc_channel_t channela;
    ledc_channel_t channelb;
} motor_t;

motor_t left_motor = {
    .pwma = (gpio_num_t)LEFT_MOTOR_A,
    .pwmb = (gpio_num_t)LEFT_MOTOR_B,
    .channela = LEDC_CHANNEL_1,
    .channelb = LEDC_CHANNEL_0,
};

motor_t right_motor = {
    .pwma = RIGHT_MOTOR_A,
    .pwmb = RIGHT_MOTOR_B,
    .channela = (gpio_num_t)LEDC_CHANNEL_2,
    .channelb = (gpio_num_t)LEDC_CHANNEL_3,
};

void initialise_motor(motor_t m);
void set_motor_speed(motor_t m, int speed, bool forward);
void speed_callback(int32_t left_velocity, int32_t right_velocity);