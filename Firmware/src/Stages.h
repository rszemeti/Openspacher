#ifndef STAGES_H
#define STAGES_H

#include "StateMachine.h"

#define FAN_SPEED_OFF 0
#define FAN_SPEED_VERY_SMALL 20
#define FAN_SPEED_SMALL 50
#define FAN_SPEED_MEDIUM 80
#define FAN_SPEED_LARGE 200

#define FUEL_PUMP_OFF 0         // Milliliters per hour
#define FUEL_PUMP_LOW 270
#define FUEL_PUMP_MEDIUM 480
#define FUEL_PUMP_HIGH 620  

#define GLOW_VOLTS_ON 8.2       // Volts
#define GLOW_VOLTS_OFF 0

#define LARGE_TO_SMALL_THRESHOLD 85
#define SMALL_TO_LARGE_THRESHOLD 72
#define OVERTEMP_THRESHOLD 90


// Define shutdown stages
Stage shutdownStages[] = {
    {
        .message = "SHUTDOWN: Stage 1",
        .duration = 10.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_SMALL, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "SHUTDOWN: Stage 2",
        .duration = 10.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_LARGE, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "SHUTDOWN: Stage 3",
        .duration = 100.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = false,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_LARGE, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "SHUTDOWN: Stage 4 - Complete",
        .duration = 1.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = false,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_OFF, FAN_SPEED_OFF},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    }
};

// Define start stages
Stage startStages[] = {
    {
        .message = "START: Stage 1 (0)",
        .duration = 5.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = false,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_OFF, FAN_SPEED_SMALL},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "START: Stage 2 (5)",
        .duration = 30.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = false,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_OFF, FAN_SPEED_OFF},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "START: Stage 3 (35)",
        .duration = 3.0,
        .condition = nullptr,
        .waterPumpState = false,
        .blowerState = false,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_SMALL, FAN_SPEED_SMALL},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "START: Stage 4 (38)",
        .duration = 4.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_SMALL, FAN_SPEED_SMALL},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_OFF}
    },
    {
        .message = "START: Stage 5 (42)",
        .duration = 6.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_SMALL, FAN_SPEED_VERY_SMALL},
        .fuelPump = {FUEL_PUMP_OFF, FUEL_PUMP_LOW}
    },
    {
        .message = "START: Stage 6 (48)",
        .duration = 6.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_VERY_SMALL, FAN_SPEED_VERY_SMALL},
        .fuelPump = {FUEL_PUMP_LOW, FUEL_PUMP_LOW}
    },
    {
        .message = "START: Stage 7 (54)",
        .duration = 14.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_SMALL, FAN_SPEED_MEDIUM},
        .fuelPump = {FUEL_PUMP_LOW, FUEL_PUMP_LOW}
    },
    {
        .message = "START: Stage 8 (68)",
        .duration = 37.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_MEDIUM, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_LOW, FUEL_PUMP_MEDIUM}
    },
    {
        .message = "START: Stage 9 (105)",
        .duration = 15.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = true,
        .fanSpeed = {FAN_SPEED_MEDIUM, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_MEDIUM, FUEL_PUMP_HIGH}
    },
    {
        .message = "START: Stage 10 (120)",
        .duration = 10.0,
        .condition = &StateMachine::largeCondition,
        .waterPumpState = true,
        .blowerState = true,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_LARGE, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_HIGH, FUEL_PUMP_HIGH}
    }
};

// Define transition from large to small
Stage smallStages[] = {
    {
        .message = "SMALL: Stage 1 - Reducing power.",
        .duration = 5.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = false,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_LARGE, FAN_SPEED_SMALL},
        .fuelPump = {FUEL_PUMP_LOW, FUEL_PUMP_LOW}
    },
    {
        .message = "SMALL: Stage 2 - Low power.",
        .duration = 60.0,
        .condition = &StateMachine::smallCondition,
        .waterPumpState = true,
        .blowerState = false,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_LARGE, FAN_SPEED_SMALL},
        .fuelPump = {FUEL_PUMP_LOW, FUEL_PUMP_LOW}
    }
};

// Define transition from small to large
Stage largeStages[] = {
    {
        .message = "LARGE: Stage 1 - Increasing power.",
        .duration = 5.0,
        .condition = nullptr,
        .waterPumpState = true,
        .blowerState = false,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_SMALL, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_HIGH, FUEL_PUMP_HIGH}
    },
    {
        .message = "LARGE: Stage 2 - Full Power.",
        .duration = 60.0,
        .condition = &StateMachine::largeCondition,
        .waterPumpState = true,
        .blowerState = false,
        .glowState = false,
        .fanSpeed = {FAN_SPEED_LARGE, FAN_SPEED_LARGE},
        .fuelPump = {FUEL_PUMP_HIGH, FUEL_PUMP_HIGH}
    }
};


// Define the number of stages for each state
const int shutdownStagesCount = sizeof(shutdownStages) / sizeof(Stage);
const int startStagesCount = sizeof(startStages) / sizeof(Stage);
const int largeStagesCount = sizeof(largeStages) / sizeof(Stage);
const int smallStagesCount = sizeof(smallStages) / sizeof(Stage);

#endif // STAGES_H
