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
#include "include/drivetrain.h"
#include "include/twai_service.h"
#include "include/encoder.h"

void app_main(void)
{
    return;
}