import time
from enum import Enum

# State definitions
class State(Enum):
    IDLE = 0
    START = 1
    HIGH = 2
    LOW = 3
    SHUTDOWN = 4

class StageHandler:
    def __init__(self, stages, next_state):
        """
        Initialize the stage handler.

        :param stages: List of dictionaries with 'function' and 'duration' or 'condition' keys.
        :param next_state: The state to transition to after all stages are complete.
        """
        self.stages = stages
        self.next_state = next_state
        self.current_stage_index = 0
        self.stage_start_time = None

    def tick(self):
        """Handle the execution of the current stage."""
        current_stage = self.stages[self.current_stage_index]

        # Check if the stage is starting for the first time
        if self.stage_start_time is None:
            self.stage_start_time = time.time()
            current_stage["function"]()

        # Handle stages with a fixed duration
        if "duration" in current_stage:
            elapsed_time = time.time() - self.stage_start_time
            if elapsed_time >= current_stage["duration"]:
                self._advance_stage()

        # Handle stages with a condition for completion
        if "condition" in current_stage and current_stage["condition"]():
            self._advance_stage()

        # Check if all stages are complete
        if self.current_stage_index >= len(self.stages):
            return self.next_state  # Return the next state to transition to

        return None  # No transition yet, continue processing current stages

    def _advance_stage(self):
        """Advance to the next stage and reset the stage start time."""
        self.current_stage_index += 1
        self.stage_start_time = None


class StateMachine:
    def __init__(self):
        self.current_state = State.IDLE
        self.control_signals = {
            "RUN": False,
            "WATER_TEMP": 20.0
        }
        self.handlers = {}

        self.state_actions = {
            State.IDLE: lambda: print("System is IDLE. Waiting for RUN to become True."),
            State.START: self.run_stage_handler,
            State.HIGH: self.run_stage_handler,
            State.LOW: self.run_stage_handler,
            State.SHUTDOWN: self.run_stage_handler
        }

    def init_start_handler(self):
        return StageHandler(
            stages=[
                {"function": lambda: print("START: Stage 1 - Initializing component A."), "duration": 2.0},
                {"function": lambda: print("START: Stage 2 - Initializing component B."), "duration": 2.0},
                {"function": lambda: print("START: Stage 3 - Initializing component C."), "duration": 2.0},
                {"function": lambda: print("START: Stage 4 - Power check."), "duration": 2.0},
                {"function": lambda: print("START: Stage 5 - System ready."), "duration": 2.0}
            ],
            next_state=State.HIGH
        )

    def init_high_handler(self):
        return StageHandler(
            stages=[
                {"function": lambda: print(f"HIGH: Stage 1 - Increasing power. Temp: {self.control_signals['WATER_TEMP']:.1f}°C"), "duration": 5.0},
                {"function": lambda: print(f"HIGH: Stage 2 - Monitoring temperature. Temp: {self.control_signals['WATER_TEMP']:.1f}°C"),
                 "condition": lambda: self.control_signals["WATER_TEMP"] > 65}
            ],
            next_state=State.LOW
        )

    def init_low_handler(self):
        return StageHandler(
            stages=[
                {"function": lambda: print(f"LOW: Stage 1 - Reducing power. Temp: {self.control_signals['WATER_TEMP']:.1f}°C"), "duration": 5.0},
                {"function": lambda: print(f"LOW: Stage 2 - Monitoring temperature. Temp: {self.control_signals['WATER_TEMP']:.1f}°C"),
                 "condition": lambda: self.control_signals["WATER_TEMP"] < 55}
            ],
            next_state=State.HIGH
        )

    def init_shutdown_handler(self):
        return StageHandler(
            stages=[
                {"function": lambda: print("SHUTDOWN: Stage 1 - Initializing shutdown sequence."), "duration": 5.0},
                {"function": lambda: print("SHUTDOWN: Stage 2 - Finalizing shutdown sequence."), "duration": 10.0}
            ],
            next_state=State.IDLE
        )

    def run_stage_handler(self):
        """Run the current stage handler."""
        # Check for immediate transition to SHUTDOWN if RUN goes low
        if not self.control_signals["RUN"] and self.current_state != State.SHUTDOWN:
            print("RUN is False. Transitioning to SHUTDOWN.")
            self.current_state = State.SHUTDOWN
            self.handlers[self.current_state] = self.init_shutdown_handler()
            return


        if self.current_state not in self.handlers:
            # Initialize the handler for this state
            if self.current_state == State.HIGH:
                self.handlers[self.current_state] = self.init_high_handler()
            elif self.current_state == State.LOW:
                self.handlers[self.current_state] = self.init_low_handler()
            elif self.current_state == State.START:
                self.handlers[self.current_state] = self.init_start_handler()
            elif self.current_state == State.SHUTDOWN:
                self.handlers[self.current_state] = self.init_shutdown_handler()

        # Run the handler
        next_state = self.handlers[self.current_state].tick()
        if next_state:
            print(f"{self.current_state.name} complete. Transitioning to {next_state.name}.")
            self.current_state = next_state

    def idle_transitions(self):
        if self.control_signals["RUN"]:
            return State.START
        return State.IDLE

    def run(self):
        """Main loop for the state machine."""
        state_transitions = {
            State.IDLE: self.idle_transitions,
            State.START: lambda: self.current_state,  # Controlled by handler
            State.HIGH: lambda: self.current_state,   # Controlled by handler
            State.LOW: lambda: self.current_state,    # Controlled by handler
            State.SHUTDOWN: lambda: self.current_state  # Controlled by handler
        }

        while True:
            # Perform the current state's action
            self.state_actions[self.current_state]()

            # Determine the next state
            if self.current_state == State.IDLE:
                next_state = state_transitions[self.current_state]()
                if next_state != self.current_state:
                    print(f"Transitioning from {self.current_state.name} to {next_state.name}")
                    self.current_state = next_state

            # Simulate timing
            time.sleep(0.5)
