#ifndef VEHICLE_H
#define VEHICLE_H
#include "components.h"
#include <iostream>
using namespace std;

//implementing inheritance by having all the components be inherited from EV
//in that way we can use the variable speed for all of them, and maybe other things too
class EV{
    private:
        bool on;
        float mass;
        float wheelRadius;
        float dragCoefficient;
        float frontalArea;
        Battery* battery;
        Motor* motor;
        bool obstacle;

    public:

        EV(Motor* motorPtr, Battery* batteryPtr);
        EV();


        // void accelerate(float throttle, float time);
        // void brake(float force, float time);
        // float getRangeEstimate();
        // void updatePosition(float deltaTime);

        void powerOn();
        void powerOff();
        bool getOn();
        void setMass(float m);
        void setWheelRadius(float r);
        void setDragCoefficient(float c);
        void setFrontalArea(float a);
    };

#endif