import time
import threading
from StateMachine import StateMachine, State


def simulate_control_signals(sm):
    """Simulate dynamic changes in water temperature based on the current state."""
    while True:
        if sm.current_state == State.HIGH:
            sm.control_signals["WATER_TEMP"] += 2  # Simulate heating in HIGH mode
        elif sm.current_state == State.LOW:
            sm.control_signals["WATER_TEMP"] -= 2  # Simulate cooling in LOW mode
        elif sm.current_state == State.SHUTDOWN or sm.current_state == State.IDLE:
            sm.control_signals["WATER_TEMP"] -= 2  # Simulate rapid cooling during SHUTDOWN or steady cooldown in IDLE

        # Clamp temperature to realistic values
        sm.control_signals["WATER_TEMP"] = max(50, min(100, sm.control_signals["WATER_TEMP"]))

        # Display current temperature
        print(f"Current water temperature: {sm.control_signals['WATER_TEMP']:.1f}°C")
        time.sleep(1)


# Handle keyboard inputs
def handle_user_input(sm):
    """Capture and process user commands."""
    while True:
        command = input("Enter command (R: Run, S: Stop, W<num>: Set water temp): ").strip().upper()
        if command == "R":
            sm.control_signals["RUN"] = True
            print("RUN set to True")
        elif command == "S":
            sm.control_signals["RUN"] = False
            print("RUN set to False")
        elif command.startswith("W"):
            try:
                temp = float(command[1:])
                sm.control_signals["WATER_TEMP"] = temp
                print(f"WATER_TEMP set to {temp}°C")
            except ValueError:
                print("Invalid temperature value. Use W<num> (e.g., W60).")
        else:
            print("Invalid command. Use R, S, or W<num>.")

# Main
if __name__ == "__main__":
    # Create the state machine
    sm = StateMachine()

    # Start threads for control signal simulation and user input
    threading.Thread(target=simulate_control_signals, args=(sm,), daemon=True).start()
    threading.Thread(target=handle_user_input, args=(sm,), daemon=True).start()

    # Run the state machine
    sm.run()
