#include "driver_input.h"

DriverInput::DriverInput() : throttle(0.0), brake(0.0) {}

void DriverInput::pressThrottle(double intensity) {
    throttle = (intensity > 1.0) ? 1.0 : (intensity < 0.0) ? 0.0 : intensity;
}

void DriverInput::pressBrake(double intensity) {
    brake = (intensity > 1.0) ? 1.0 : (intensity < 0.0) ? 0.0 : intensity;
}

void DriverInput::releaseThrottle() {
    throttle = 0.0;
}

void DriverInput::releaseBrake() {
    brake = 0.0;
}

double DriverInput::getThrottle() const{
    return throttle;
}

double DriverInput::getBrake() const{ 
    return brake; 
}
