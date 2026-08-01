#ifndef HEARTBEAT_LED_H_
#define HEARTBEAT_LED_H_

#include <Arduino.h>

class HeartbeatLED {
public:
    // Constructor: Pass the LED pin number
    HeartbeatLED(uint8_t ledPin);

    // Initialize the LED pin
    void begin();

    // Turn LED on
    void on();

    // Turn LED off
    void off();

    // Toggle LED state
    void toggle();

    // Check if LED is ON
    bool isOn();

private:
    uint8_t pin;
    bool state;
};

#endif
