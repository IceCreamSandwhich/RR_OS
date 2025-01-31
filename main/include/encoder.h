#pragma once

#define ENCODER_A CONFIG_ENCODER_A
#define ENCODER_B CONFIG_ENCODER_B

void IRAM_ATTR encoder_ISR(void* arg);

void setup_encoder(void);
void encoder_task(void* arg);