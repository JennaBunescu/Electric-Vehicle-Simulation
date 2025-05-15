#ifndef VEHICLE_H
#define VEHICLE_H
#include "components.h"
#include "driver_input.h"
#include <iostream>
using namespace std;

// // Forward declarations to avoid circular dependency between components and vehicle

class EV{
    private:
        bool on;
        float mass;
        float dragCoefficient;
        float frontalArea;
        Battery* battery;
        Motor* motor;
        bool obstacle;
        float wheelRadius = 0.3f; //Radius of the vehicle's wheels (meters)

    public:

        EV(Motor* motorPtr, Battery* batteryPtr);
        EV();

        EV(Battery* batt) : battery(batt) {}
        ~EV() { delete battery; }
        void update(float speed, float delta_t) {
            battery->discharge(speed, delta_t);
        }


        // void accelerate(float throttle, float time);
        // void brake(float force, float time);
        // float getRangeEstimate();
        // void updatePosition(float deltaTime);
        float get_wheelRadius();

        void powerOn();
        void powerOff();
        bool getOn();
        void setMass(float m);
        void setWheelRadius(float r);
        void setDragCoefficient(float c);
        void setFrontalArea(float a);
    };



#endif