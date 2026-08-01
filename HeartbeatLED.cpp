#include "HeartbeatLED.h"

HeartbeatLED::HeartbeatLED(uint8_t ledPin)
    : pin(ledPin), state(false)
{}

void HeartbeatLED::begin() {
    pinMode(pin, OUTPUT);
    off();  // Default state: OFF
}

void HeartbeatLED::on() {
    digitalWrite(pin, LOW);
    state = true;
}

void HeartbeatLED::off() {
    digitalWrite(pin, HIGH);
    state = false;
}

void HeartbeatLED::toggle() {
    state = !state;
    digitalWrite(pin, state ? HIGH : LOW);
}

bool HeartbeatLED::isOn() {
    return state;
}
