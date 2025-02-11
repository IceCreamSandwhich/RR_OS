#include "include/events.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "include/twai_service.h"
#include "include/led.h"

static QueueHandle_t event_queue;

void initialise_events() {
    event_queue = xQueueCreate(10, sizeof(event_t));
}

void add_event(event_t event) {
    xQueueSend(event_queue, &event, 0);
}
void rr_os_event_handler()
{
    const char* TAG = "Events";
    event_t event;
    if (xQueueReceive(event_queue, &event, 0))
    {
        switch (event)
        {
        case EVENT_CONNECTION:
            ESP_LOGI(TAG, "Connection event detected");
            // Disable the interrupt to prevent triggering on TWAI comms
            gpio_set_intr_type(TWAI_RX, GPIO_INTR_DISABLE);
            state.connected = true;
            set_led_color(0x0000FF);
            break;
        default:
            break;
        }
    }
}