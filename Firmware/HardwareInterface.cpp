#include "HardwareInterface.h"

// Initialize all hardware components
void HardwareInterface::init() {
    // Set digital output pins
    pinMode(fanPin, OUTPUT);
    pinMode(fuelPumpPin, OUTPUT);
    pinMode(waterPumpPin, OUTPUT);
    pinMode(blowerPin, OUTPUT);
    pinMode(glowVoltsPin, OUTPUT);
    pinMode(waterPumpSpeedPin, OUTPUT);
    pinMode(powerCtlPin, OUTPUT);

    // Default states: LOW or OFF
    digitalWrite(fanPin, LOW);
    digitalWrite(fuelPumpPin, LOW);
    digitalWrite(waterPumpPin, LOW);
    digitalWrite(blowerPin, LOW);
    digitalWrite(glowVoltsPin, LOW);
    digitalWrite(waterPumpSpeedPin, LOW);
    digitalWrite(powerCtlPin, LOW);
}

// Output controls
void HardwareInterface::setFanSpeed(uint8_t speed) {
    analogWrite(fanPin, speed);
}

void HardwareInterface::setFuelPumpSpeed(uint8_t speed) {
    analogWrite(fuelPumpPin, speed);
}

void HardwareInterface::setWaterPumpState(bool state) {
    digitalWrite(waterPumpPin, state ? HIGH : LOW);
}

void HardwareInterface::setBlowerState(bool state) {
    digitalWrite(blowerPin, state ? HIGH : LOW);
}

void HardwareInterface::setGlowVoltage(uint8_t voltage) {
    analogWrite(glowVoltsPin, voltage);
}

void HardwareInterface::setWaterPumpSpeed(uint8_t speed) {
    analogWrite(waterPumpSpeedPin, speed);
}

void HardwareInterface::setPowerControl(bool state) {
    digitalWrite(powerCtlPin, state ? HIGH : LOW);
}

// Input reading
int HardwareInterface::readFlameSensor() {
    return analogRead(flameSensePin);
}

int HardwareInterface::readSurfaceSensor() {
    return analogRead(surfaceSensePin);
}

int HardwareInterface::readOvertempSensor() {
    return analogRead(overtempSensePin);
}

// Current sensing
int HardwareInterface::readFanCurrent() {
    return analogRead(fanCurrentSensePin);
}

int HardwareInterface::readGlowCurrent() {
    return analogRead(glowCurrentSensePin);
}

int HardwareInterface::readWaterPumpCurrent() {
    return analogRead(waterPumpCurrentSensePin);
}
