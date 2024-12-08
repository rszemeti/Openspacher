#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H
#include <Arduino.h>

// Pin Definitions
const int fanPin = 5;
const int fuelPumpPin = 6;
const int waterPumpPin = 7;
const int blowerPin = 8;
const int glowVoltsPin = 9;
const int waterPumpSpeedPin = 10;
const int powerCtlPin = 11;

// Additional Analog Pins
const int surfaceSensePin = A0;
const int overtempSensePin = A1;
const int flameSensePin = A2;
const int fanCurrentSensePin = A3;
const int glowCurrentSensePin = A4;
const int waterPumpCurrentSensePin = A5;

// Custom Mapping Struct
struct VoltageTempMapping {
    int voltage;   // Input voltage
    int temperature; // Corresponding temperature
};

// Voltage-Temperature Mappings (Fixed-Size Arrays)
const VoltageTempMapping waterTempMapping[] = {
    {0, 20},   // 0V corresponds to 20°C
    {128, 50}, // Midpoint 128 corresponds to 50°C
    {250, 100} // Max voltage corresponds to 100°C
};

const VoltageTempMapping flameTempMapping[] = {
    {0, 100},  // 0V corresponds to 100°C
    {128, 300}, // Midpoint 128 corresponds to 300°C
    {250, 600}  // Max voltage corresponds to 600°C
};

const VoltageTempMapping overTempMapping[] = {
    {0, 50},   // 0V corresponds to 50°C
    {128, 75}, // Midpoint 128 corresponds to 75°C
    {250, 150} // Max voltage corresponds to 150°C
};

#endif // HARDWARE_CONFIG_H
