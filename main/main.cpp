// // OS and C Headers
// #include <stdio.h>
// #include <stdlib.h>
// #include <inttypes.h>

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"

// // ESP-IDF Headers
// #include "esp_err.h"
// #include "esp_log.h"
// #include "driver/gpio.h"

// // Project Headers
// #include "include/pins.h"
// #include "include/drivetrain.h"
// #include "include/twai_service.h"
// #include "include/encoder.h"

// void app_main(void)
// {
//     return;
// }
#include <stdio.h>
#include "BNO08x.hpp"

static const constexpr char *TAG = "Main";

float calculate_heading(float x, float y) {
    float heading = atan2(y, x);  // Radians
    if (heading < 0) {
        heading += 2 * M_PI;  // Ensure heading is positive
    }
    return heading * (180 / M_PI);  // Convert to degrees
}


extern "C" void app_main(void)
{
    static BNO08x imu;
    // if a custom wiring scheme is desired instead of default:

    /*
    bno08x_config_t imu_config;        //create config struct
    imu_config.io_mosi = GPIO_NUM_X;   //assign pin
    imu_config.io_miso = GPIO_NUM_X;   //assign pin
    //etc...
    BNO08x imu(imu_config);            //pass config to BNO08x constructor
    */

    // initialize imu
    if (!imu.initialize())
    {
        ESP_LOGE(TAG, "Init failure, returning from main.");
        return;
    }

    // enable game rotation vector and calibrated gyro reports
    imu.rpt.rv_game.enable(100000UL);  // 100,000us == 100ms report interval
    imu.rpt.cal_gyro.enable(100000UL); // 100,000us == 100ms report interval
    imu.rpt.cal_magnetometer.enable(100000UL);
    imu.rpt.rv_geomagnetic.enable(100000UL);

    while (1)
    {
        // block until new report is detected
        if (imu.data_available())
        {
            // check for game rotation vector report
            if (imu.rpt.rv_game.has_new_data())
            {
                // get absolute heading in degrees
                bno08x_euler_angle_t euler = imu.rpt.rv_game.get_euler();
                // display heading
                ESP_LOGI(TAG, "Euler Angle: x (roll): %.2f y (pitch): %.2f z (yaw): %.2f", euler.x, euler.y, euler.z);
            }

            // // check for cal gyro report
            // if (imu.rpt.cal_gyro.has_new_data())
            // {
            //     // get angular velocity in rad/s
            //     bno08x_gyro_t velocity = imu.rpt.cal_gyro.get();
            //     // display velocity
            //     ESP_LOGW(TAG, "Velocity: x: %.2f y: %.2f z: %.2f", velocity.x, velocity.y, velocity.z);
            // }

            // if (imu.rpt.cal_magnetometer.has_new_data())
            // {
            //     bno08x_magf_t mag = imu.rpt.cal_magnetometer.get();
            //     ESP_LOGI(TAG, "Magnetometer: x: %.2f y: %.2f z: %.2f", mag.x, mag.y, mag.z);
            //     float heading = calculate_heading(mag.x, mag.y);
            //     ESP_LOGI(TAG, "Heading: %.2f", heading);
            // }

            if (imu.rpt.rv_geomagnetic.has_new_data())
            {
                bno08x_euler_angle_t euler = imu.rpt.rv_geomagnetic.get_euler();
                ESP_LOGI(TAG, "Geomagnetic Euler Angle: x (roll): %.2f y (pitch): %.2f z (yaw): %.2f", euler.x, euler.y, euler.z);
                float heading = calculate_heading(euler.x, euler.y);
                ESP_LOGI(TAG, "Heading: %.2f", heading);
            }


        }
    }
}