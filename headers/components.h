#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <iostream>
#include "driver_input.h"
using namespace std;


class Battery{ //If it receives a signal from the controller, the battery transmits DC electrical energy to the motor.
//lithium ion battery
    private:
        double Q_max;
        double Q_current;
        double V_max;
        double R_internal;
        double stateOfHealth;
        double temperature;
        double voltage;
        double current;

    //let's say we need formulas for:
    // 1. connecting the car's motion with a battery discharge
    //    lets assume for now that discharge is directly proportional to speed * time
    //    discharge means a decrease in Q_current
    //    so deltaQ = speed * time;
    //    
    // 2. connecting the charger's properties with the battery charge
    //    lets assume for now that the charge is proportional to the voltage provided and time
    //    so deltaQ = time * V/R;
    // 3. using regenerative braking? NO
    // 4. connecting the battery's voltage to the motor
    public:
        Battery();
        void set_Q_max(float Q);
        void set_Q_current(float Q);
        void set_V_max(float V);
        void set_R_internal(float R);
        void set_SOH(float SOH);
        void set_temp(float T);

        float get_SOC();
        float get_Q_max();
        float get_Q_current();
        float get_V_max();
        float get_R_internal();
        float get_SOH();
        float get_temp();

        //this function should be called every time the speed changes, 
        //to register the previous speed and for how long it ran

        //or another option is to update this function every fraction of a second and 
        //use that time increment instead of total time, and keep updating 
        //the current charge
        void discharge(float speed, float delta_t);

        void charge(float V_applied, float time);


};

class Motor {
private:
    float currentDemand;  // Current required by the motor in Amps
    float speed;          // Speed of the vehicle in km/h
    float powerRating;    // Power rating in kilowatts (kW)
    float maxCurrent;     // Max current the motor can draw (in Amps)
    float efficiency;     // Efficiency of the motor (between 0 and 1)
    float maxSpeed;       // Maximum motor speed (RPM)
    const float maxTorque = 200.0f;      //200.0f
    const float maxBrakeTorque = 300.0f; //300.0f
    const float inertia = 10.0f;; //10.0f
    const float wheelRadius = 0.3f;; //0.3f

public:
    Motor();

    float updateSpeed(DriverInput& driverInput, Battery &battery, float deltaTime);
    void encounterObstacle(){
        //change "force" throttle/brake of the driver input 
    }

};


class Charger{

    private:
        bool isCharging; //to track state
        float maxPowerOutput;
        float efficiency;


    public:

    void startCharging(Battery* battery);
    void stopCharging(Battery* battery);
};


class Display{

};

#endif