#pragma once
#include "freertos/FreeRTOS.h"


typedef enum EVENTS {
    EVENT_NONE = 0,
    EVENT_CONNECTION,
    EVENT_DISCONNECT_REQUEST,
    EVENT_DISCONNECT,
} event_t;

typedef struct state_t {
    bool twai_active;
    bool connected;
    uint32_t last_isr_time;
} state_t;

extern state_t state;

void initialise_events();
void add_event(event_t event);
void rr_os_event_handler();