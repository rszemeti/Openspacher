#include "StateMachine.h"
#include "HardwareInterface.h"


HardwareInterface hw;
StateMachine sm(hw);

void setup() {
      
    hw.init();
    sm.init();

}

void loop() {

    sm.tick();
    delay(1000);

}

