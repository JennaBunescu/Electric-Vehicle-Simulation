#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <iostream>
#include "driver_input.h"
using namespace std;


class Battery{ //If it receives a signal from the controller, the battery transmits DC electrical energy to the motor.
//lithium ion battery
    private:
        double Q_max; //Q stands for "Charge." This variable represents the max amount of charge 
        double Q_now; 
        double V_max;
        double R_internal;
        double stateOfHealth;
        double temperature;
        double voltage;
        double current;
        double heatCapacity; // J/°C, thermal mass of battery
        double heatTransferCoeff; // W/°C, to environment


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

        void setCurrent(float I);
        void rechargeFromRegen(float deltaQ);

        void discharge(float speed, float delta_t);

        void charge(float V_applied, float time, bool &fullCharge);

        float updateTemperature(float delta_t, float ambientTemp);

        void degradeSOH(float delta_t);


};

class Motor {
private:
    float currentDemand;  // Current required by the motor in Amps
    float speed;          // Speed of the vehicle in km/h
    float powerRating;    // Power rating in kilowatts (kW)
    float maxCurrent;     // Max current the motor can draw (in Amps)
    float efficiency;     // Efficiency of the motor (between 0 and 1)
    float maxSpeed;       // Maximum motor speed (RPM)
    float maxTorque = 200;      //200.0f
    float maxBrakeTorque = 300; //300.0f
    float inertia = 10; //10.0f
    float wheelRadius = 0.3f; //0.3f
    float regenEfficiency = 0.5;
    float maxRegenPower = 100;

public:
    Motor();

    bool isRegenerating(DriverInput& input);

    void setMaxRegenPower(float power);

    float getMaxRegenPower() const;

    float updateSpeed(DriverInput& driverInput, Battery &battery, float deltaTime);
    void applyRegenerativeBraking(DriverInput &input, Battery& battery, float deltaTime);
    float calculateRegenPower(DriverInput &input);



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



#endif