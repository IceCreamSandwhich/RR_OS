#pragma once

#include <inttypes.h>

typedef enum msg_type {
    VELOCITY,
    DISCONNET,
    CONNECT,
    HELP
} msg_type_t;

typedef struct twai_msg {
    msg_type_t type;
    union {
        struct {
            uint8_t v;
            uint8_t w;
        } velocity;

        struct {
            uint8_t id;
        } help;
    };
} twai_msg_t;