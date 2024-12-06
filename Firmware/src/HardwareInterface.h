#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include <Arduino.h>

class HardwareInterface {
private:
    // Pin definitions
    static const uint8_t fanPin = 5;
    static const uint8_t fuelPumpPin = 6;
    static const uint8_t waterPumpPin = 7;
    static const uint8_t blowerPin = 8;
    static const uint8_t glowVoltsPin = 9;

    static const uint8_t flameSensePin = A0;
    static const uint8_t surfaceSensePin = A1;
    static const uint8_t overtempSensePin = A2;

    static const uint8_t fanCurrentSensePin = A3;
    static const uint8_t glowCurrentSensePin = A4;
    static const uint8_t waterPumpCurrentSensePin = A5;

    static const uint8_t waterPumpSpeedPin = 10;
    static const uint8_t powerCtlPin = 11;

public:
    // Initialize all hardware components
    void init();

    // Output controls
    void setFanSpeed(uint8_t speed);
    void setFuelPumpSpeed(uint8_t speed);
    void setWaterPumpState(bool state);
    void setBlowerState(bool state);
    void setGlowVoltage(uint8_t voltage);
    void setWaterPumpSpeed(uint8_t speed);
    void setPowerControl(bool state);

    // Input reading
    int readFlameSensor();
    int readSurfaceSensor();
    int readOvertempSensor();

    // Current sensing
    int readFanCurrent();
    int readGlowCurrent();
    int readWaterPumpCurrent();
};

#endif // HARDWARE_INTERFACE_H
