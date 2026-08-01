#include "CurrentSense.h"

CurrentSense::CurrentSense(uint8_t i2c_addr, uint8_t oc_pin, uint8_t sw_pin, void (*isr)(void))
    : i2c_addr(i2c_addr), alert_pin(alert_pin), switch_pin(sw_pin), AlertISR(isr)
{}

bool CurrentSense::begin() {
    bool ok = ina260.begin(i2c_addr);

    pinMode(alert_pin, INPUT);
    attachAlertInterrupt();

    pinMode(switch_pin, OUTPUT);
    switchOff();

    return ok;
}

void CurrentSense::switchOn() {
    digitalWrite(switch_pin, HIGH);
}

void CurrentSense::switchOff() {
    digitalWrite(switch_pin, LOW);
}

void CurrentSense::attachAlertInterrupt() {
    attachInterrupt(digitalPinToInterrupt(alert_pin), AlertISR, RISING);
}

float CurrentSense::readCurrent() {
    return ina260.readCurrent();
}

float CurrentSense::readVoltage() {
    return ina260.readBusVoltage();
}

float CurrentSense::readPower() {
    return ina260.readPower();
}
