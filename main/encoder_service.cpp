#include "includes/encoder.h"
#include "driver/gpio.h"

volatile int encoderPosition = 0;
volatile int position = 0;

void init_encoder()
{
    // Set up the encoder pins
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    ioconf.mode = GPIO_MODE_INPUT;
    ioconf.pin_bit_mask = (1ULL << ENCODER_A_PIN) | (1ULL << ENCODER_B_PIN);
    ioconf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    ioconf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    // Set up the interrupt
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ENCODER_A_PIN, encoder_isr_handler, (void *)ENCODER_A_PIN);
    gpio_isr_handler_add(ENCODER_B_PIN, encoder_isr_handler, (void *)ENCODER_B_PIN);
}

void IRAM_ATTR encoder_ISR(void *arg)
{
    int a_val = gpio_get_level(ENCODER_A_PIN);
    int b_val = gpio_get_level(ENCODER_B_PIN);
    int encoding = (a_val << 1) | b_val;

    if (encoding == 0b00)
    {
        if (lastPosition == 0b01)
        {
            encoderPosition++;
        }
        else if (lastPosition == 0b10)
        {
            encoderPosition--;
        }
    }
    else if (encoding == 0b11)
    {
        if (lastPosition == 0b10)
        {
            encoderPosition++;
        }
        else if (lastPosition == 0b01)
        {
            encoderPosition--;
        }
    }

    xQueueSendFromISR(encoder_queue, &encoderPosition, NULL);
}

void encoder_task(void *arg)
{
    int position;
    while (1)
    {
        xQueueReceive(encoder_queue, &position, portMAX_DELAY);
        printf("Encoder Position: %d\n", position);
    }
}