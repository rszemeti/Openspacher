#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>
#include "HardwareInterface.h"

// Define a range structure for analog signals
struct Range {
    int start; // Starting value
    int end;   // Ending value
};

// Define the structure for a stage
struct Stage {
    const char* message;   // Log message for the stage
    float duration;        // Duration of the stage in seconds
    bool (*condition)();   // Optional condition pointer to exit the stage
    bool waterPumpState;   // Water pump state: true = ON, false = OFF
    bool blowerState;      // Blower state: true = ON, false = OFF
    bool glowState;        // Glow plug state: true = ON, false = OFF
    Range fanSpeed;        // Start and end values for fan speed
    Range fuelPump;        // Start and end values for fuel pump
};

// Define state names
enum State {
    IDLE,
    START,
    LARGE,  // Renamed from HIGH
    SMALL,  // Renamed from LOW
    SHUTDOWN
};

class StateMachine {
private:
    HardwareInterface& hardware;      // Reference to the hardware interface
    State currentState;               // Current state of the state machine
    unsigned long stageStartTime;     // Time when the current stage started
    int currentStageIndex;            // Current stage index within the state
    Stage* currentStages;             // Pointer to the stages of the current state
    int totalStages;                  // Total number of stages in the current state
    State nextState;                  // Next state to transition to
    bool runSignal;                   // Control signal for RUN

    // Helper methods
    void resetHandler(State state);   // Reset stages and setup for a given state
    void tickHandler();               // Tick through the current stage logic



public:
    // Constructor
    StateMachine(HardwareInterface& hw);

    // Initialization
    void init();

    // Control signal setters
    void setRunSignal(bool run);      // Set the RUN signal

    // State machine tick
    void tick();                      // Perform the state machine logic on each loop

    // Getters for current state
    State getCurrentState() const { return currentState; }

    // Helper functions for conditions
    static bool largeCondition();
    static bool smallCondition();
};

#endif // STATEMACHINE_H
