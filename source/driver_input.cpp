#include "driver_input.h"



DriverInput::DriverInput() : throttlePosition(0.0), brakePosition(0.0) {}

    float DriverInput ::get_throttle(){
        return throttlePosition;
    }
    float DriverInput::get_brake(){
        return brakePosition;
    }

void DriverInput::set_throttle(double intensity) {
    throttlePosition = (intensity > 1.0) ? 1.0 : (intensity < 0.0) ? 0.0 : intensity;
}

void DriverInput::set_brake(double intensity) {
    brakePosition = (intensity > 1.0) ? 1.0 : (intensity < 0.0) ? 0.0 : intensity;
}
