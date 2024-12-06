#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <Arduino.h>
#include "HardwareInterface.h"

// State definitions
enum State {
    IDLE,
    START,
    LARGE,   // Renamed from HIGH
    SMALL,   // Renamed from LOW
    SHUTDOWN
};

// Stage structure
struct Stage {
    void (*function)();       // Function pointer for the stage action
    float duration;           // Duration of the stage in seconds
    bool (*condition)();      // Optional condition pointer to exit the stage
};

class StateMachine {
private:
    HardwareInterface& hardware;  // Hardware interface object
    State currentState;           // Current state of the state machine
    unsigned long stageStartTime; // Time when the current stage started
    int currentStageIndex;        // Current stage index within the state
    Stage* currentStages;         // Pointer to the stages of the current state
    int totalStages;              // Total number of stages in the current state
    State nextState;              // Next state to transition to
    bool runSignal;               // Control signal for RUN

    // Stage definitions for each state
    Stage shutdownStages[2];
    Stage startStages[5];
    Stage largeStages[2];
    Stage smallStages[2];

    // Helper methods
    void resetHandler(State state); // Reset stages and setup for a given state
    void tickHandler();             // Tick through the current stage logic

    // Helper functions for conditions
    static bool largeCondition();
    static bool smallCondition();

public:
    StateMachine(HardwareInterface& hw); // Constructor

    // Initialization
    void init();

    // Control signal setters
    void setRunSignal(bool run); // Set the RUN signal

    // State machine tick
    void tick(); // Perform the state machine logic on each loop

    // Getters for current state
    State getCurrentState() const { return currentState; }
};

#endif // STATEMACHINE_H
