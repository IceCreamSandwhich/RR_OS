// OS and C Headers
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// ESP-IDF Headers
#include "esp_err.h"
#include "esp_log.h"
#include "driver/gpio.h"

// Project Headers
#include "include/pins.h"
#include "include/drivetrain.hpp"
#include "include/twai_service.h"
#include "include/encoder.h"
#include "include/uros_service.h"
#include "BNO08x.hpp"
#include "led_indicator.h"

static const constexpr char *TAG = "Main";

#define IMU_ENABLE false

static bool twai_active = false;
static bool twai_event = false;

static void IRAM_ATTR twai_service_isr_handler(void *arg)
{
    if (twai_active) return;
    twai_event = true;
    gpio_set_intr_type(TWAI_RX, GPIO_INTR_DISABLE);
}



extern "C" void app_main(void)
{

    static BNO08x imu;
    bno08x_quat_t quat;
    bno08x_euler_angle_t euler;
    bno08x_ang_vel_t omega;

    if (IMU_ENABLE)
    {
        ESP_LOGI(TAG, "IMU Enabled");
        // initialize imu
        if (!imu.initialize())
        {
            ESP_LOGE(TAG, "Init failure, returning from main.");
            return;
        }
        imu.rpt.rv_gyro_integrated.enable(900UL); // Set report rate to 900ms

    }
    
    // Initialising task parameters
    uros_service_params_t uros_service_params = {
        .euler = euler,
    };

    // uros_service(); //TODO: Figure out how to pass parameters to this function

    initialise_drivetrain();


    // Set the TWAI RX pin as an input pin
    // gpio_pad_select_gpio(TWAI_RX);
    gpio_set_direction(TWAI_RX, GPIO_MODE_INPUT);
    gpio_set_pull_mode(TWAI_RX, GPIO_PULLDOWN_ONLY);
    // Set the TWAI RX pin as an interrupt pin
    gpio_set_intr_type(TWAI_RX, GPIO_INTR_POSEDGE);
    // Install the driver's GPIO ISR handler service, which allows per-pin GPIO interrupt handlers
    gpio_install_isr_service(0);
    // Hook up the TWAI RX pin to the ISR handler
    gpio_isr_handler_add(TWAI_RX, twai_service_isr_handler, NULL);


    // Initialising LED



    led_strip_config_t strip_config = {
        .strip_gpio_num = 21,              // The GPIO that connected to the LED strip's data line
        .max_leds = 1,                  // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,       // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,                  // LED strip model
        // .flags.invert_out = false,                      // whether to invert the output signal
    };

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000
        // .flags.with_dma = false,
    };

    led_indicator_strips_config_t strips_config = {
        .led_strip_cfg = strip_config,
        .led_strip_driver = LED_STRIP_RMT,
        .led_strip_rmt_cfg = rmt_config,
    };

    const led_indicator_config_t config = {
        .mode = LED_STRIPS_MODE,
        .led_indicator_strips_config = &strips_config,
        .blink_lists = NULL,
        .blink_list_num = 0,
    };

    led_indicator_handle_t led = led_indicator_create(&config);
    led_indicator_set_brightness(led, 1);
    led_indicator_set_rgb(led, 0x00FF00);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    led_indicator_set_rgb(led, 0xFF0000);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    led_indicator_set_rgb(led, 0x0000FF);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    led_indicator_set_rgb(led, 0x000000);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    


    while (1)
    {
        if (twai_event)
        {
            twai_service_init();
            vTaskDelay(100 / portTICK_PERIOD_MS);
            twai_service_start();
            // ESP_LOGI(TAG, "Moving forward");
            // set_motor_speed(left_motor, 1024, true);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            // ESP_LOGI(TAG, "Moving backward");
            // set_motor_speed(left_motor, 0, true);
            // vTaskDelay(1000 / portTICK_PERIOD_MS);
            twai_event = false;
            twai_active = true;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        


        //TODO: Move this to a separate task
        // if (IMU_ENABLE)
        // {
        //     // block until new report is detected
        //     if (imu.data_available())
        //     {
        //         // get the latest report
        //         imu.rpt.rv_gyro_integrated.get(quat, omega);
        //         euler = quat;
        //         ESP_LOGI(TAG, "Roll: %f, Pitch: %f, Yaw: %f", euler.x, euler.y, euler.z);
        //     }
        // }
    }
}