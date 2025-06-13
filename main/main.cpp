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
//#include "include/uros_service.h"
#include "include/imu_service.h"
// #include "include/encoder.h"
#include "include/led.h"
#include "include/events.h"
#include "RadioLibCustomHAL.hpp"

static const constexpr char *TAG = "Main";

void initialise(rr_state_t state);

extern "C" void app_main(void)
{
    // CLK, MISO, MOSI, CS
    RadioLibCustomHAL hal = RadioLibCustomHAL(1, 2, 3, 5);
    // CS, G0, RST
    SX1278 radio = new Module(&hal, 5, 7, 4);
    int status = radio.begin();
    if (status == RADIOLIB_ERR_NONE)
    {
        ESP_LOGI("Radio", "Radio initialised successfully");
    } else {
        ESP_LOGE("Radio", "Radio failed to initialise");
    }

    radio.setFrequency(433.0);
    radio.setSpreadingFactor(12);
    // Creating events queue
    // initialise_events();
    // state.connected = false;
    // state.twai_active = false;
    state.imu_enabled = true;
    state.led_enabled = true;

    // // Initialising peripherals
    initialise(state);


}

void initialise(rr_state_t state)
{
    if (state.imu_enabled)
    {
        init_imu();
        imu_service();
    }

    if (state.led_enabled)
    {
        initialise_led();
        set_led_color(INDEPENDENT_COLOR);
        twai_interrupt_init();
    }
    
    initialise_drivetrain();
    launch_rr_os_service();
}