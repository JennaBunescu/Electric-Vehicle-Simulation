#ifndef VEHICLE_H
#define VEHICLE_H
#include "components.h"
#include <iostream>
using namespace std;

class EV{
    private:

        float mass;
        float wheelRadius;
        float dragCoefficient;
        float frontalArea;
        float speed;
        Battery* battery;
        Motor* motor;

    public:

        EV(){

        }


        void accelerate(float throttle, float time);
        void brake(float force, float time);
        float getRangeEstimate();
        void updatePosition(float deltaTime);
};

#endif