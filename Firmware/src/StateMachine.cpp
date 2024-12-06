#include "StateMachine.h"

// Global pointer for condition lambdas
static StateMachine* smInstance = nullptr;

// Helper condition functions
bool StateMachine::largeCondition() {
    return smInstance && smInstance->hardware.readSurfaceSensor() > 65.0;
}

bool StateMachine::smallCondition() {
    return smInstance && smInstance->hardware.readSurfaceSensor() < 55.0;
}

// Constructor
StateMachine::StateMachine(HardwareInterface& hw) : hardware(hw), currentState(IDLE), stageStartTime(0), currentStageIndex(0),
                                                    currentStages(nullptr), totalStages(0), nextState(IDLE),
                                                    runSignal(false) {
    smInstance = this;

    // Initialize stages for SHUTDOWN
    shutdownStages[0] = {[]() { Serial.println("SHUTDOWN: Stage 1 - Initializing shutdown sequence."); }, 5.0, nullptr};
    shutdownStages[1] = {[]() { Serial.println("SHUTDOWN: Stage 2 - Finalizing shutdown sequence."); }, 10.0, nullptr};

    // Initialize stages for START
    startStages[0] = {[]() { Serial.println("START: Stage 1 - Initializing component A."); }, 2.0, nullptr};
    startStages[1] = {[]() { Serial.println("START: Stage 2 - Initializing component B."); }, 2.0, nullptr};
    startStages[2] = {[]() { Serial.println("START: Stage 3 - Initializing component C."); }, 2.0, nullptr};
    startStages[3] = {[]() { Serial.println("START: Stage 4 - Power check."); }, 2.0, nullptr};
    startStages[4] = {[]() { Serial.println("START: Stage 5 - System ready."); }, 2.0, nullptr};

    // Initialize stages for LARGE (was HIGH)
    largeStages[0] = {[]() { Serial.print("LARGE: Stage 1 - Increasing power. Temp: "); Serial.println(smInstance->hardware.readSurfaceSensor()); }, 5.0, nullptr};
    largeStages[1] = {[]() { Serial.print("LARGE: Stage 2 - Monitoring temperature. Temp: "); Serial.println(smInstance->hardware.readSurfaceSensor()); },
                      0.0, largeCondition};

    // Initialize stages for SMALL (was LOW)
    smallStages[0] = {[]() { Serial.print("SMALL: Stage 1 - Reducing power. Temp: "); Serial.println(smInstance->hardware.readSurfaceSensor()); }, 5.0, nullptr};
    smallStages[1] = {[]() { Serial.print("SMALL: Stage 2 - Monitoring temperature. Temp: "); Serial.println(smInstance->hardware.readSurfaceSensor()); },
                      0.0, smallCondition};
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
            totalStages = sizeof(shutdownStages) / sizeof(Stage);
            nextState = IDLE;
            break;
        case START:
            currentStages = startStages;
            totalStages = sizeof(startStages) / sizeof(Stage);
            nextState = LARGE; // Transition to LARGE after START
            break;
        case LARGE: // Renamed from HIGH
            currentStages = largeStages;
            totalStages = sizeof(largeStages) / sizeof(Stage);
            nextState = SMALL; // Transition to SMALL
            break;
        case SMALL: // Renamed from LOW
            currentStages = smallStages;
            totalStages = sizeof(smallStages) / sizeof(Stage);
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

// Tick through the current stage logic
void StateMachine::tickHandler() {
    if (currentStageIndex >= totalStages) {
        currentState = nextState;
        resetHandler(currentState);
        return;
    }

    Stage& currentStage = currentStages[currentStageIndex];

    // Check if the stage is timed
    if (currentStage.duration > 0.0) {
        if (millis() - stageStartTime >= currentStage.duration * 1000) {
            currentStage.function();
            currentStageIndex++;
            stageStartTime = millis();
        }
    }
    // Check if the stage has a condition
    else if (currentStage.condition && currentStage.condition()) {
        currentStage.function();
        currentStageIndex++;
        stageStartTime = millis();
    }
}
