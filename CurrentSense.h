#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H

#include <Arduino.h>
#include <Adafruit_INA260.h>

class CurrentSense {
public:
    // Constructor
    CurrentSense(uint8_t i2c_addr, uint8_t oc_pin, uint8_t sw_pin, void (*isr)(void));

    // Initialize INA260 and pins
    bool begin();

    // Switch control
    void switchOn();
    void switchOff();

    // Read sensor values
    float readCurrent();
    float readVoltage();
    float readPower();

    // Public data (read-only)
    uint8_t alert_pin;
    uint8_t switch_pin;
    uint8_t i2c_addr;
    void (*AlertISR)(void);

private:
    Adafruit_INA260 ina260;  
    void attachAlertInterrupt();
};

#endif
