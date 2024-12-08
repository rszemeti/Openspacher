#include "StateMachine.h"
#include "HardwareInterface.h"
#include "Stages.h"

// Global pointer for condition lambdas
static StateMachine* smInstance = nullptr;

State StateMachine::largeCondition() {
    if (smInstance) {
        int waterTemp = smInstance->hardware.getWaterTemp();

        if (waterTemp > OVERTEMP_THRESHOLD) {
            return State::SHUTDOWN;
        } else if (waterTemp > LARGE_TO_SMALL_THRESHOLD) {
            return State::SMALL;
        }
    }
    return State::LARGE;
}


State StateMachine::smallCondition() {
    if (smInstance) {
        int waterTemp = smInstance->hardware.getWaterTemp();

        if (waterTemp > OVERTEMP_THRESHOLD) {
            return State::SHUTDOWN;
        } else if (waterTemp < SMALL_TO_LARGE_THRESHOLD) {
            return State::LARGE;
        }
    }
    return State::SMALL;
}

// Linear interpolation helper
int linearInterpolate(int start, int end, unsigned long elapsedTime, unsigned long duration) {
    float progress = static_cast<float>(elapsedTime) / (duration * 1000);
    if (progress > 1.0) progress = 1.0; // Clamp progress
    return start + static_cast<int>((end - start) * progress);
}

// Constructor
StateMachine::StateMachine(HardwareInterface& hw) : hardware(hw), currentState(IDLE), stageStartTime(0), currentStageIndex(0),
                                                    currentStages(nullptr), totalStages(0), nextState(IDLE),
                                                    runSignal(false) {
    smInstance = this;
}

// Initialize the state machine
void StateMachine::init() {
    Serial.begin(9600);
    resetHandler(IDLE);
}

// Set the RUN signal
void StateMachine::setRunSignal(bool run) {
    if (runSignal != run) {
        runSignal = run;
        if (!runSignal && currentState != SHUTDOWN) {
            currentState = SHUTDOWN;
            resetHandler(SHUTDOWN);
        }
    }
}

// Tick the state machine
void StateMachine::tick() {
    if (currentState == IDLE && runSignal) {
        currentState = START;
        resetHandler(START);
    }

    tickHandler();
}

// Reset the stage handler for a given state
void StateMachine::resetHandler(State state) {
    switch (state) {
        case SHUTDOWN:
            currentStages = shutdownStages;
            totalStages = shutdownStagesCount;
            nextState = IDLE;
            break;
        case START:
            currentStages = startStages;
            totalStages = startStagesCount;
            nextState = LARGE; // Transition to LARGE after START
            break;
        case LARGE: // Renamed from HIGH
            currentStages = largeStages;
            totalStages = largeStagesCount;
            nextState = SMALL; // Transition to SMALL
            break;
        case SMALL: // Renamed from LOW
            currentStages = smallStages;
            totalStages = smallStagesCount;
            nextState = LARGE; // Transition to LARGE
            break;
        default:
            currentStages = nullptr;
            totalStages = 0;
            break;
    }
    currentStageIndex = 0;
    stageStartTime = millis();
}

void StateMachine::tickHandler() {
    if (currentStageIndex >= totalStages) {
        // Transition to the next state
        currentState = nextState;
        resetHandler(currentState);
        return;
    }

    Stage& currentStage = currentStages[currentStageIndex];

    // Log the stage message
    Serial.println(currentStage.message);

    // Set digital states via hardware abstraction
    hardware.setWaterPumpState(currentStage.waterPumpState);
    hardware.setBlowerState(currentStage.blowerState);
    hardware.setGlowState(currentStage.glowState);

    // Calculate elapsed time
    unsigned long elapsedTime = millis() - stageStartTime;

    // Interpolate analog values
    int interpolatedFanSpeed = linearInterpolate(currentStage.fanSpeed.start, currentStage.fanSpeed.end, elapsedTime, currentStage.duration);
    int interpolatedFuelPump = linearInterpolate(currentStage.fuelPump.start, currentStage.fuelPump.end, elapsedTime, currentStage.duration);

    // Write interpolated values to hardware
    hardware.setFanSpeed(interpolatedFanSpeed);
    hardware.setFuelPumpSpeed(interpolatedFuelPump);

    // Log interpolated values
    Serial.print("Fan Speed: ");
    Serial.println(interpolatedFanSpeed);
    Serial.print("Fuel Pump: ");
    Serial.println(interpolatedFuelPump);

    // Check if the stage is complete
    if (elapsedTime >= currentStage.duration * 1000) {
        currentStageIndex++;
        stageStartTime = millis();

        // Final stage logic
        if (currentStageIndex >= totalStages) {
            State transitionState = currentStage.condition ? currentStage.condition() : nextState;

            // Handle state transition
            if (transitionState != currentState) {
                Serial.println("Transitioning to next state.");
                currentState = transitionState;
                resetHandler(currentState);
            }
        }
    }
}
