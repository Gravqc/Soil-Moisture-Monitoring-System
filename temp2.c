#include <LPC17xx.h>

#define LED_PIN_MASK 0x000FF0 // P0.4 to P0.11
#define SENSOR_PIN 0x00800000 // P0.23

int main(void) {
    // Initialize GPIO
    LPC_GPIO0->FIODIR |= LED_PIN_MASK; // set P0.4 to P0.11 as output

    while (1) {
        // Read digital sensor value
        int sensor_value = (LPC_GPIO0->FIOPIN & SENSOR_PIN) >> 23;

        // Check if moisture level is high
        if (sensor_value == 0) {
            // Turn on LED
            LPC_GPIO0->FIOSET |= LED_PIN_MASK;
        } else {
            // Turn off LED
            LPC_GPIO0->FIOCLR |= LED_PIN_MASK;
        }
    }
}
