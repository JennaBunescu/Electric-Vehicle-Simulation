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
        EV();
        EV(float wheelRadius);

        void update(float speed, float delta_t) {
            battery->discharge(speed, delta_t);
        }
                    // Copy constructor
    EV(const EV& other) : wheelRadius(other.wheelRadius) {
        // Copy other members
    }

    // Assignment operator
    EV& operator=(const EV& other) {
        if (this != &other) {
        wheelRadius = other.wheelRadius;
            // Copy other members
        }
        return *this;
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