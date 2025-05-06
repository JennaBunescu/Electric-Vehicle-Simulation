#include "../headers/driver_input.h"

DriverInput::DriverInput() : throttlePosition(0.0f), brakePosition(0.0f) {}

float DriverInput::get_throttle() {
    return throttlePosition;
}

float DriverInput::get_brake() {
    return brakePosition;
}

void DriverInput::set_throttle(float intensity) {
    throttlePosition = (intensity > 1.0f) ? 1.0f : (intensity < 0.0f) ? 0.0f : intensity;
}

void DriverInput::set_brake(float intensity) {
    brakePosition = (intensity > 1.0f) ? 1.0f : (intensity < 0.0f) ? 0.0f : intensity;
}
