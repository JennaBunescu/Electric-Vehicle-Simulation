#include "driver_input.h"

DriverInput::DriverInput() : throttlePosition(0.0), brakePosition(0.0) {}

void DriverInput::pressThrottle(double intensity) {
    throttlePosition = (intensity > 1.0) ? 1.0 : (intensity < 0.0) ? 0.0 : intensity;
}

void DriverInput::pressBrake(double intensity) {
    brakePosition = (intensity > 1.0) ? 1.0 : (intensity < 0.0) ? 0.0 : intensity;
}

void DriverInput::releaseThrottle() {
    throttlePosition = 0.0;
}

void DriverInput::releaseBrake() {
    brakePosition = 0.0;
}

double DriverInput::getThrottle() const{
    return throttlePosition;
}

double DriverInput::getBrake() const{ 
    return brakePosition; 
}
