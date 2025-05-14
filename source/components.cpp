#include <iostream>
#include "../headers/components.h"
#include "../headers/driver_input.h"
using namespace std;

///// Battery

Battery::Battery(){
    Q_max = 150;               // Max capacity in Ah (Ampere-hours)
    Q_now = 150;               // Starting fully charged so Q_now = Q_max
    V_max = 420;               // Max voltage (fully charged state)
    R_internal = 0.02;         // Internal resistance in ohms (typical for EVs)
    stateOfHealth = 100;       // New battery starts at full health
    temperature = 25;          // Room temperature in Celsius (adjust as needed)
    voltage = 400;             // Nominal voltage (fully charged at 420 V)
    current = 0;               // No current flow initially
    heatCapacity = 500.0; // J/°C, thermal mass of battery
    heatTransferCoeff = 0.05; // W/°C, to environment

};


float Battery::get_SOC(){
    //the current state of charge in percent is the ratio of charge remaining and max charge capacity
    return (Q_now / Q_max) * 100.0;
}

//this function should be called repeatedly every fraction of a second
//and it updates the battery charge incrementally.
//does temperature affect discharge?
void Battery::discharge(float speed, float delta_t){

    float deltaQ = 0.01 * speed * delta_t;
    Q_now -= deltaQ;
    current = -deltaQ / delta_t; //Update current. Current is negative for discharging.


    //prevent battery from going below 0
    if (Q_now < 0) {
        Q_now = 0;
    }
}

void Battery::charge(float V_applied, float deltaTime, bool &isFull){
    //the change in charge is the current applied (V_applied/R_internal) times the change in time (which will be every frame)
    float deltaQ = deltaTime*V_applied/R_internal; 
    current = deltaQ / deltaTime; //Update current. Current is positive for charging

    if(Q_now < Q_max){
        Q_now += deltaQ;
        if (Q_now >= Q_max) {
            Q_now = Q_max;
        }

    } else { // this will be displayed on the car's screen, so we will probably need ofstream here to write this in the display file with "fout"
        isFull = true;
        return;
    }

}

float Battery::updateTemperature(float delta_t, float ambientTemp) {

    // Heat generated: Q = I^2 * R * t
    float heatGenerated = current * current * R_internal * delta_t;

    // Cooling: Q = h * (T_batt - T_ambient) * t
    float cooling = heatTransferCoeff * (temperature - ambientTemp) * delta_t;

    // Net heat change
    float netHeat = heatGenerated - cooling;

    // Temperature change = Q / C
    float deltaTemp = netHeat / heatCapacity;

    this->temperature += deltaTemp;

    return temperature;
}


//setters
void Battery::set_Q_max(float Q){
    Q_max = Q;
}

void Battery::set_Q_current(float Q){
    Q_now = Q;
}

//we could also include a battery power output on the display
void Battery::set_V_max(float V){
    V_max = V;
}
void Battery::set_R_internal(float R){
    R_internal = R;
}

void Battery::set_SOH(float SOH){
    stateOfHealth = SOH;
}


//getters
float Battery::get_Q_max(){
    return Q_max;
}

float Battery::get_Q_current(){
    return Q_now;
}

float Battery::get_V_max(){
    return V_max;
}

float Battery::get_R_internal(){
    return R_internal;
}

float Battery::get_SOH(){
    return stateOfHealth;
}

float Battery::get_temp(){
    return temperature;
}
//regenerative - both when you brake and when you take foot off?
//modeling the engine - design the engine's circuit, analyze the circuit
//and use the formulas to find the motor actions

//spring mass damper model, when you press down you exert a force, then there will be a transducer
//and a variable resistor which would change the voltage sent to the control

//put the parameters in a file of a route
////// Motor
Motor::Motor(){
    maxSpeed = 100;
}

float Motor::updateSpeed(DriverInput &driverInput, Battery &battery, float deltaTime) {
    static float angularSpeed = 0;

    float throttle = driverInput.get_throttle();
    float brake = driverInput.get_brake();

    cout << "Throttle: " << throttle << " | Brake: " << brake << endl;

    float netTorque = 0.0;

    // Apply throttle torque
    if (throttle > 0) {
        float throttleTorque = throttle * maxTorque;
        netTorque += throttleTorque;
    }

    // Apply braking torque
    if (brake > 0) {
        float brakingTorque = brake * maxBrakeTorque;
        netTorque -= brakingTorque;
    }

    // Passive drag when neither throttle nor brake is pressed
    if (throttle == 0 && brake == 0) {
        float dragTorque = 0.8 * maxTorque; // You can tune this value
        netTorque -= dragTorque;
    }

    // Calculate angular acceleration
    float angularAcceleration = netTorque / inertia;
    angularSpeed += angularAcceleration * deltaTime;

    // Prevent negative angular speed (no reverse for now)
    if (angularSpeed < 0.0f) {
        angularSpeed = 0.0f;
    }

    // Convert to linear speed
    this->speed = wheelRadius * angularSpeed;

    // Clamp to max speed
    if (this->speed > maxSpeed) {
        this->speed = maxSpeed;
    }

    // Discharge battery based on current speed
    battery.discharge(speed, deltaTime);

    return speed;
}


void Charger::stopCharging(Battery* battery) {
    if (battery) {
        //battery->V_applied(0.0f); // TODO: Double check if this needs to be Q current set to 0 or current set to 0. If it is just "current" then we need a setCurrent function in the battery class
    }
    cout << "Charging stopped" << endl;
}
