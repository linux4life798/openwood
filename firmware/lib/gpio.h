#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t bank;
    uint8_t pin;
} GPIOPin;

extern const GPIOPin power_button_gpio;
extern const GPIOPin ptt_button_gpio;

void GPIOConfigureOutput(GPIOPin gpioPin);
void GPIODriveLow(GPIOPin gpioPin);
bool GPIOReadPin(GPIOPin gpioPin);

#endif
