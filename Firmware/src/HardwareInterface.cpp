#include "HardwareInterface.h"
#include "HardwareConfig.h"

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

void HardwareInterface::setGlowState(bool state) {
    analogWrite(blowerPin, state ? GLOW_VOLTS_ON : GLOW_VOLTS_OFF);
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

// Interpolation function with extrapolation
int HardwareInterface::interpolate(int rawValue, const VoltageTempMapping mapping[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        int x1 = mapping[i].voltage;
        int y1 = mapping[i].temperature;
        int x2 = mapping[i + 1].voltage;
        int y2 = mapping[i + 1].temperature;

        // Check if rawValue is within the current range
        if (rawValue >= x1 && rawValue <= x2) {
            // Linear interpolation formula
            return y1 + (y2 - y1) * (rawValue - x1) / (x2 - x1);
        }
    }

    // Extrapolate before the first point
    if (rawValue < mapping[0].voltage) {
        int x1 = mapping[0].voltage;
        int y1 = mapping[0].temperature;
        int x2 = mapping[1].voltage;
        int y2 = mapping[1].temperature;
        return y1 + (y2 - y1) * (rawValue - x1) / (x2 - x1);
    }

    // Extrapolate after the last point
    if (rawValue > mapping[size - 1].voltage) {
        int x1 = mapping[size - 2].voltage;
        int y1 = mapping[size - 2].temperature;
        int x2 = mapping[size - 1].voltage;
        int y2 = mapping[size - 1].temperature;
        return y1 + (y2 - y1) * (rawValue - x1) / (x2 - x1);
    }

    return 0; // Default fallback
}

// Get water temperature
int HardwareInterface::getWaterTemp() {
    int rawValue = this->readSurfaceSensor();
    return interpolate(rawValue, waterTempMapping, sizeof(waterTempMapping) / sizeof(waterTempMapping[0]));
}

// Get flame temperature
int HardwareInterface::getFlameTemp() {
    int rawValue = this->readFlameSensor();
    return interpolate(rawValue, flameTempMapping, sizeof(flameTempMapping) / sizeof(flameTempMapping[0]));
}

// Get over-temperature value
int HardwareInterface::getOverTemp() {
    int rawValue = this->readOvertempSensor();
    return interpolate(rawValue, overTempMapping, sizeof(overTempMapping) / sizeof(overTempMapping[0]));
}
