#include "include/drivetrain.h"

void initialise_motor(motor_t m)
{
    ledc_channel_config_t ledc_channel_cf;

    ledc_channel_cf.gpio_num = m.pwma;
    ledc_channel_cf.speed_mode = SPEED_MODE;
    ledc_channel_cf.channel = m.channela;
    ledc_channel_cf.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_cf.timer_sel = LEDC_TIMER_0;
    ledc_channel_cf.duty = 0;
    ledc_channel_cf.hpoint = 0;
    // ledc_channel_cf.flags = LEDC_FADE_NO_WAIT;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_cf));

    ledc_channel_cf.gpio_num = m.pwmb;
    ledc_channel_cf.channel = m.channelb;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_cf));
}

void set_motor_speed(motor_t m, int speed, bool forward)
{
    if (forward)
    {
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, m.channela, speed));
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, m.channelb, 0));
    }
    else
    {
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, m.channela, 0));
        ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, m.channelb, speed));
    }
    ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, m.channela));
    ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, m.channelb));
}

void speed_callback(int32_t left_velocity, int32_t right_velocity)
{
    int32_t speed_left = abs(left_velocity);
    int32_t speed_right = abs(right_velocity);
    set_motor_speed(left_motor, speed_left, left_velocity > 0);
    set_motor_speed(right_motor, speed_right, right_velocity > 0);
}
