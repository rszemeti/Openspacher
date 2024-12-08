#ifndef STAGES_H
#define STAGES_H

#include "StateMachine.h"

// Define shutdown stages
Stage shutdownStages[] = {
    {
        .message = "SHUTDOWN: Stage 1 - Initializing shutdown sequence.",
        .waterPumpState = false,
        .blowerState = false,
        .duration = 5.0,
        .condition = nullptr,
        .fanSpeed = {0, 0},
        .glowVolts = {0, 0},
        .fuelPump = {0, 0}
    },
    {
        .message = "SHUTDOWN: Stage 2 - Finalizing shutdown sequence.",
        .waterPumpState = false,
        .blowerState = false,
        .duration = 10.0,
        .condition = nullptr,
        .fanSpeed = {0, 0},
        .glowVolts = {0, 0},
        .fuelPump = {0, 0}
    }
};

// Define start stages
Stage startStages[] = {
    {
        .message = "START: Stage 1 - Initializing component A.",
        .waterPumpState = false,
        .blowerState = false,
        .duration = 2.0,
        .condition = nullptr,
        .fanSpeed = {1, 256},
        .glowVolts = {3, 120},
        .fuelPump = {25, 16}
    },
    // Add more stages here...
};

// Define large stages
Stage largeStages[] = {
    {
        .message = "LARGE: Stage 1 - Increasing power.",
        .waterPumpState = true,
        .blowerState = false,
        .duration = 5.0,
        .condition = nullptr,
        .fanSpeed = {50, 200},
        .glowVolts = {20, 100},
        .fuelPump = {10, 18}
    },
    // Add more stages here...
};

// Define small stages
Stage smallStages[] = {
    {
        .message = "SMALL: Stage 1 - Reducing power.",
        .waterPumpState = true,
        .blowerState = false,
        .duration = 5.0,
        .condition = nullptr,
        .fanSpeed = {256, 150},
        .glowVolts = {120, 80},
        .fuelPump = {16, 18}
    },
    // Add more stages here...
};

// Define the number of stages for each state
const int shutdownStagesCount = sizeof(shutdownStages) / sizeof(Stage);
const int startStagesCount = sizeof(startStages) / sizeof(Stage);
const int largeStagesCount = sizeof(largeStages) / sizeof(Stage);
const int smallStagesCount = sizeof(smallStages) / sizeof(Stage);

#endif // STAGES_H
