#include "../headers/vehicle.h"
#include "../headers/components.h"


EV::EV(){
    this->on = true;
}


EV::EV(float wheelRadius){
    this->wheelRadius = wheelRadius;
    this->on = true;
}

float EV::get_wheelRadius(){
    return wheelRadius;
}

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
bool EV::getOn() { return on; }

void EV::setMass(float m) { mass = m; }
void EV::setWheelRadius(float r) { wheelRadius = r; }
void EV::setDragCoefficient(float c) { dragCoefficient = c; }
void EV::setFrontalArea(float a) { frontalArea = a; }