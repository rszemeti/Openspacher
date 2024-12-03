import time
from enum import Enum

# State definitions
class State(Enum):
    IDLE = 0
    FAN_START = 1
    FUEL_PUMP_START = 2
    BURNER_START = 3
    BURNER_RUN = 4

# Simulated sensor data
sensors = {
    "flame_temperature": 0  # Replace with actual sensor logic
}

# State parameters
state_params = {
    State.IDLE: {
        "runtime": 0,
        "fan_speed_start": 0, "fan_speed_end": 0,
        "glow_volts_start": 0, "glow_volts_end": 0,
        "fuel_pump_speed_start": 0, "fuel_pump_speed_end": 0,
        "water_pump_state": False, "blower_state": False
    },
    State.FAN_START: {
        "runtime": 5000,
        "fan_speed_start": 30, "fan_speed_end": 100,
        "glow_volts_start": 0, "glow_volts_end": 0,
        "fuel_pump_speed_start": 0, "fuel_pump_speed_end": 0,
        "water_pump_state": False, "blower_state": False
    },
    State.FUEL_PUMP_START: {
        "runtime": 5000,
        "fan_speed_start": 100, "fan_speed_end": 150,
        "glow_volts_start": 75, "glow_volts_end": 100,
        "fuel_pump_speed_start": 0, "fuel_pump_speed_end": 0,
        "water_pump_state": False, "blower_state": False
    },
    State.BURNER_START: {
        "runtime": 5000,
        "fan_speed_start": 150, "fan_speed_end": 200,
        "glow_volts_start": 100, "glow_volts_end": 100,
        "fuel_pump_speed_start": 20, "fuel_pump_speed_end": 100,
        "water_pump_state": True, "blower_state": True
    },
    State.BURNER_RUN: {
        "runtime": 10000,
        "fan_speed_start": 200, "fan_speed_end": 200,
        "glow_volts_start": 100, "glow_volts_end": 0,
        "fuel_pump_speed_start": 100, "fuel_pump_speed_end": 100,
        "water_pump_state": True, "blower_state": True
    }
}

# State transitions with conditions
state_transitions = {
    State.IDLE: [{"next_state": State.FAN_START, "condition": lambda: True}],
    State.FAN_START: [{"next_state": State.FUEL_PUMP_START, "condition": lambda: True}],
    State.FUEL_PUMP_START: [{"next_state": State.BURNER_START, "condition": lambda: True}],
    State.BURNER_START: [
        {"next_state": State.BURNER_RUN, "condition": lambda: sensors["flame_temperature"] >= 200},
        {"next_state": State.IDLE, "condition": lambda: sensors["flame_temperature"] < 200}
    ],
    State.BURNER_RUN: [{"next_state": State.IDLE, "condition": lambda: True}]
}

# Simulated hardware states
hardware_states = {
    "fan_speed": 0,
    "glow_volts": 0,
    "fuel_pump_speed": 0,
    "water_pump_state": False,
    "blower_state": False,
}

# Current state and time tracking
current_state = State.IDLE
last_state_transition_time = time.time()

def update_parameter(start_value, end_value, elapsed_time, runtime):
    """Calculate the new parameter value based on elapsed time."""
    if runtime == 0:  # Prevent division by zero
        return end_value
    increment = (end_value - start_value) * (elapsed_time / runtime)
    return min(start_value + increment, end_value)

def transition_to_next_state():
    """Transition to the next state based on conditions."""
    global current_state, last_state_transition_time
    if current_state in state_transitions:
        for transition in state_transitions[current_state]:
            if transition["condition"]():
                current_state = transition["next_state"]
                last_state_transition_time = time.time()
                return

def simulate_state():
    """Simulate the current state."""
    global hardware_states
    elapsed_time = (time.time() - last_state_transition_time) * 1000  # Convert to milliseconds
    state_config = state_params[current_state]

    # Update hardware states
    hardware_states["fan_speed"] = update_parameter(
        state_config["fan_speed_start"], state_config["fan_speed_end"], elapsed_time, state_config["runtime"]
    )
    hardware_states["glow_volts"] = update_parameter(
        state_config["glow_volts_start"], state_config["glow_volts_end"], elapsed_time, state_config["runtime"]
    )
    hardware_states["fuel_pump_speed"] = update_parameter(
        state_config["fuel_pump_speed_start"], state_config["fuel_pump_speed_end"], elapsed_time, state_config["runtime"]
    )

    # Clear screen and print hardware states
    print("\033[H\033[J", end="")  # Clear the screen and move the cursor to the top-left
    print(f"State: {current_state.name}")
    print(f"Fan Speed: {hardware_states['fan_speed']:.2f}")
    print(f"Glow Volts: {hardware_states['glow_volts']:.2f}")
    print(f"Fuel Pump Speed: {hardware_states['fuel_pump_speed']:.2f}")
    print(f"Water Pump: {'ON' if hardware_states['water_pump_state'] else 'OFF'}")
    print(f"Blower: {'ON' if hardware_states['blower_state'] else 'OFF'}")
    print(f"Flame Temperature: {sensors['flame_temperature']}°C")

    # Check for state transition
    if elapsed_time >= state_config["runtime"]:
        transition_to_next_state()

def main():
    """Main simulation loop."""
    while True:
        # Simulate flame temperature increasing over time
        sensors["flame_temperature"] += 10 if current_state == State.BURNER_START else -5
        sensors["flame_temperature"] = max(0, sensors["flame_temperature"])  # Keep temperature non-negative

        simulate_state()
        time.sleep(0.1)  # Simulate loop delay (100ms)

if __name__ == "__main__":
    main()
