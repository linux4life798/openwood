#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t bank;
    uint8_t pin;
} GPIOPin;

/** Power button GPIO input, active-high. */
extern const GPIOPin power_button_gpio;

/** PTT button GPIO input, active-low. */
extern const GPIOPin ptt_button_gpio;

/**
 * Configures a GPIO pin for output.
 */
void GPIOConfigureOutput(GPIOPin gpioPin);

/**
 * Drives a GPIO output low.
 */
void GPIODriveLow(GPIOPin gpioPin);

/**
 * Reads a GPIO input level.
 */
bool GPIOReadPin(GPIOPin gpioPin);

#endif
