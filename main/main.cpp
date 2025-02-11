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

// Component Headers
#include "BNO08x.hpp"

// Project Headers
#include "include/pins.h"
#include "include/drivetrain.hpp"
#include "include/twai_service.h"
#include "include/encoder.h"
#include "include/uros_service.h"
#include "include/led.h"
#include "include/events.h"

static const constexpr char *TAG = "Main";

#define IMU_ENABLE false


extern "C" void app_main(void)
{

    // Creating events queue
    initialise_events();
    state.connected = false;
    state.twai_active = false;

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


    twai_interrupt_init();


    // Initialising LED

    initialise_led();
    set_led_color(0xFF0000);


    while (1)
    {
        
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
        rr_os_event_handler();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}