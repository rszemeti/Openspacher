#include <Arduino.h>
#include <avr/wdt.h>

#define FAN_PIN 5
#define FUEL_PUMP_PIN 6
#define WATER_PUMP_PIN 7
#define BLOWER_PIN 8
#define GLOW_VOLTS_PIN 9

#define FLAME_SENSE A0
#define SURFACE_SENSE A1
#define OVERTEMP_SENSE A2

// Additional analog input pins
#define FAN_CURRENT_SENSE A3
#define GLOW_CURRENT_SENSE A4
#define WATER_PUMP_CURRENT_SENSE A5

// Additional digital output pins
#define WATER_PUMP_SPEED 10
#define POWER_CTL 11


// Global variable to store whether the reset was caused by the watchdog timer
volatile bool watchdogReset = false;

// Define structure for state parameters
struct StateParams {
  unsigned long runtime; // Runtime of the state in milliseconds
  int fan_speed_start;   // Fan speed at the start of the state
  int fan_speed_end;     // Fan speed at the end of the state
  int glow_volts_start;  // Glow voltage level at the start of the state
  int glow_volts_end;    // Glow voltage level at the end of the state
  int fuel_pump_speed_start; // Fuel pump speed at the start of the state
  int fuel_pump_speed_end;   // Fuel pump speed at the end of the state
  bool water_pump_state; // Water pump state at the start of the state (true = on, false = off)
  bool blower_state;     // Blower state at the start of the state (true = on, false = off)
  // Add more parameters as needed
};


// Define structure for current fan and fuel pump speeds
struct FanFuelPumpSpeeds {
  short fan_speed;
  short fuel_pump_speed;
  short glow_volts;
  bool blower_state;
  bool water_pump_state;
};

// Initialize current state
enum State {
  IDLE,
  FAN_START,
  FUEL_PUMP_START,
  BURNER_START,
  BURNER_RUN
};

enum State currentState = IDLE;

// Define state parameters using named initialization
struct StateParams stateParams[] = {
  {
    .runtime = 0,
    .fan_speed_start = 0, .fan_speed_end = 0,
    .glow_volts_start = 0, .glow_volts_end = 0,
    .fuel_pump_speed_start = 0, .fuel_pump_speed_end = 0,
    .water_pump_state = false,
    .blower_state = false
  }, // IDLE
  {
    .runtime = 5000,
    .fan_speed_start = 30, .fan_speed_end = 100,
    .glow_volts_start = 0, .glow_volts_end = 0,
    .fuel_pump_speed_start = 0, .fuel_pump_speed_end = 0,
    .water_pump_state = false,
    .blower_state = false
  }, // FAN_START
  {
    .runtime = 5000,
    .fan_speed_start = 100, .fan_speed_end = 150,
    .glow_volts_start = 75, .glow_volts_end = 100,
    .fuel_pump_speed_start = 0, .fuel_pump_speed_end = 0,
    .water_pump_state = false,
    .blower_state = false
  }, // GLOW_START
  {
    .runtime = 5000,
    .fan_speed_start = 150, .fan_speed_end = 200,
    .glow_volts_start = 100, .glow_volts_end = 100,
    .fuel_pump_speed_start = 20, .fuel_pump_speed_end = 100,
    .water_pump_state = true,
    .blower_state = true
  }, // BURNER_START (Turn on water pump and blower)
  {
    .runtime = 10000,
    .fan_speed_start = 200, .fan_speed_end = 0,
    .glow_volts_start = 100, .glow_volts_end = 0,
    .fuel_pump_speed_start = 100, .fuel_pump_speed_end = 0,
    .water_pump_state = false,
    .blower_state = false
  } // BURNER_RUN
};


// Initialize current fan and fuel pump speeds
struct FanFuelPumpSpeeds currentSpeeds = {0, 0};

// Variable to keep track of last state transition time
unsigned long lastStateTransitionTime = 0;

// Function prototypes
void updateState();
void resetWatchdog();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize pins
  pinMode(FAN_PIN, OUTPUT);
  pinMode(FUEL_PUMP_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);
  pinMode(BLOWER_PIN, OUTPUT);
  pinMode(GLOW_VOLTS_PIN, OUTPUT);

  // Set glow and fan pins low
  digitalWrite(GLOW_VOLTS_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  // Enable watchdog timer with a timeout of 2 seconds
  wdt_enable(WDTO_2S);

  // Set initial time for last state transition
  lastStateTransitionTime = millis();
}

void loop() {
  // Reset the watchdog timer to prevent it from triggering a reset
  resetWatchdog();
  
  // Check if enough time has passed for state transition
  if (millis() - lastStateTransitionTime >= 100) {
    updateState(currentSpeeds);
    lastStateTransitionTime = millis();
  }
}

void updateParameter(int startValue, int endValue, short* currentValue, int pin, unsigned long elapsedTime) {
  // Calculate the incremental change based on elapsed time
  int increment = (float)(endValue - startValue) / stateParams[currentState].runtime * elapsedTime;
  
  // Check if the calculated value exceeds the end value
  if (*currentValue + increment > endValue) {
    // Set the current value to the end value
    *currentValue = endValue;
  } else {
    // Update the current value
    *currentValue += increment;
  }
  
  // Write the current value to the corresponding pin
  analogWrite(pin, *currentValue);
}


void changeState(State newState, FanFuelPumpSpeeds& currentValues) {
  // Update current state
  currentState = newState;

/*
maybe use existing value to avoid any step chnages?
  // Update fan speed
  currentValues.fan_speed = stateParams[newState].fan_speed_start;

  // Update glow voltage
  currentValues.glow_volts = stateParams[newState].glow_volts_start;

  // Update fuel pump speed
  currentValues.fuel_pump_speed = stateParams[newState].fuel_pump_speed_start;
*/

  // Update blower state
  currentValues.blower_state = stateParams[newState].blower_state;

  // Update water pump state
  currentValues.water_pump_state = stateParams[newState].water_pump_state;

  // Reset the last state transition time
  lastStateTransitionTime = millis();
}

void updateState(FanFuelPumpSpeeds& currentValues) {
  unsigned long elapsedTime = millis() - lastStateTransitionTime;

  // Update fan speed
  updateParameter(stateParams[currentState].fan_speed_start, stateParams[currentState].fan_speed_end, &currentSpeeds.fan_speed, FAN_PIN, elapsedTime);

  // Update glow voltage
  updateParameter(stateParams[currentState].glow_volts_start, stateParams[currentState].glow_volts_end,  &currentSpeeds.glow_volts, GLOW_VOLTS_PIN, elapsedTime);

  // Update fuel pump speed
  updateParameter(stateParams[currentState].fuel_pump_speed_start, stateParams[currentState].fuel_pump_speed_end,  &currentSpeeds.fuel_pump_speed, FUEL_PUMP_PIN, elapsedTime);

  // Check if enough time has passed for state transition
  if (elapsedTime >= stateParams[currentState].runtime) {
    // Perform decision-making at the end of the state
    if (currentState == FAN_START) {
      // Perform checks
      if (true) {
        changeState(FUEL_PUMP_START, currentValues);
      } else {
        currentState = FAN_START; // State ????
      }
    }

    // Reset the last state transition time
    lastStateTransitionTime = millis();
  }
}


// Function to reset the watchdog timer
void resetWatchdog() {
  wdt_reset();
}

// Watchdog timer interrupt service routine
ISR(WDT_vect) {
  // Set the watchdogReset flag to true
  watchdogReset = true;
}
