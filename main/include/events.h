#pragma once
#include "freertos/FreeRTOS.h"


typedef enum EVENTS {
    EVENT_NONE = 0,
    EVENT_CONNECTION
} event_t;

typedef struct state_t {
    bool twai_active;
    bool connected;
} state_t;

static state_t state;

void initialise_events();
void add_event(event_t event);
void rr_os_event_handler();