#include "../headers/driver_input.h"

DriverInput::DriverInput() {
    throttlePosition = 0;
    brakePosition = 0;
}

float DriverInput::get_throttle() {
    return throttlePosition;
}

float DriverInput::get_brake() {
    return brakePosition;
}

void DriverInput::set_throttle(float intensity) {
    if (intensity > 1) {
        throttlePosition = 1;
    } else if (intensity < 0) {
        throttlePosition = 0;
    } else {
        throttlePosition = intensity;
    }

}

void DriverInput::set_brake(float intensity) {
    if (intensity > 1) {
        brakePosition = 1;
    } else if (intensity < 0) {
        brakePosition = 0;
    } else {
        brakePosition = intensity;
    }
}
