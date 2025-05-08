#include "../headers/vehicle.h"

// void EV::accelerate(float throttle, float time) {

// }

// void EV::brake(float force, float time) {

// }

// float EV::getRangeEstimate() {

// }

// void EV::updatePosition(float deltaTime) {

// }



void EV::powerOn() { on = true; }
void EV::powerOff() { on = false; }
bool EV::getOn() const { return on; }

void EV::setMass(float m) { mass = m; }
void EV::setWheelRadius(float r) { wheelRadius = r; }
void EV::setDragCoefficient(float c) { dragCoefficient = c; }
void EV::setFrontalArea(float a) { frontalArea = a; }