#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include <Arduino.h>
#include "HardwareConfig.h"

// Define the HardwareInterface class
class HardwareInterface {
public:
    // Initialization
    void init();

    // Output controls
    void setFanSpeed(uint8_t speed);
    void setFuelPumpSpeed(uint8_t speed);
    void setWaterPumpState(bool state);
    void setBlowerState(bool state);
    void setGlowState(bool state);
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

    // Derived inputs
    int getWaterTemp();
    int getFlameTemp();
    int getOverTemp();

private:
    // Interpolation helper
    int interpolate(int rawValue, const VoltageTempMapping mapping[], size_t size);
};

#endif // HARDWARE_INTERFACE_H
