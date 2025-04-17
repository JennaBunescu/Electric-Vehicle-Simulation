#ifndef VEHICLE_COMPONENTS_H
#define VEHICLE_COMPONENTS_H
#include <iostream>
using namespace std;


class Battery{ //If it receives a signal from the controller, the battery transmits DC electrical energy to the inverter and then uses it to drive the motor.
//lithium ion batteries, Nickel-Metal Hydride Batteries, Lead-acid Batteries
    private:
        double Q_max;
        double Q_current;
        double V_max;
        double R_internal;
        double stateOfCharge;
        double stateOfHealth;
        double temperature;
        double voltage;
        double current;

    float getSOC() {
        return (Q_current / Q_max) * 100.0;
    }



    public:




};

class Motor{
    private:
        double powerRating_kWh;
        double efficiency;
        double rpm;

    public:

};

class Charger{

    private:
        
        float maxPowerOutput;
        float efficiency;


    public:
    void startCharging(Battery* battery);
    void stopCharging();
};


class Display{


};

#endif