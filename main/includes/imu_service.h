#include "BNO08x.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void init_imu();
void imu_loop(void *pvParameter);
BaseType_t imu_service(void);
void launch_rr_os_service();