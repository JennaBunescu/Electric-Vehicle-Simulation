#include <iostream>
#include "../headers/components.h"
#include "../headers/driver_input.h"
using namespace std;



///// Battery

Battery::Battery() {
    Q_max = 150;               // Max capacity in Ah (for a 60 kWh battery at 420 V)
    Q_current = 150;           // Starting fully charged
    V_max = 420;               // Max voltage (fully charged state)
    R_internal = 0.02;         // Internal resistance in ohms (typical for EVs)
    stateOfHealth = 100;       // New battery starts at full health
    temperature = 25;          // Room temperature in Celsius (adjust as needed)
    voltage = 400;             // Nominal voltage (fully charged at 420 V)
    current = 0;               // No current flow initially
};


float Battery::get_SOC(){
    //the current state of charge in percent is the ratio of charge remaining and max charge capacity
    return (Q_current / Q_max) * 100.0;
}

//this function should be called repeatedly every fraction of a second
//and it updates the battery charge incrementally.
void Battery::discharge(float speed, float delta_t){
    float deltaQ = 0.01 * speed * delta_t;
    Q_current -= deltaQ;

    //prevent battery from going below 0
    if (Q_current < 0) {
        Q_current = 0;
    }
}

void Battery::charge(float V_applied, float time){
    float deltaQ = 0.01*V_applied/R_internal;
    if(Q_current < Q_max){
        Q_current += deltaQ;
        if (Q_current >= Q_max) {
            Q_current = Q_max;
        }

    } else { // this will be displayed on the car's screen, so we will probably need ofstream here to write this in the display file with "fout"
        cout << "BATTERY FULL" << endl;
        return;
    }

}

//setters
void Battery::set_Q_max(float Q){
    Q_max = Q;
}

void Battery::set_Q_current(float Q){
    Q_current = Q;
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

void Battery::set_temp(float T){
    temperature = T;
}

//getters
float Battery::get_Q_max(){
    return Q_max;
}

float Battery::get_Q_current(){
    return Q_current;
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
    static float angularSpeed = 0.0f;

    float throttle = driverInput.get_throttle();
    cout << "From motor class: throttle: " << driverInput.get_throttle() <<endl; 
    float brake = driverInput.get_brake();

    //apply throttle torque
    if (throttle > 0){
        float torque = throttle * maxTorque;
        float angularAcceleration = torque / inertia;
        angularSpeed += angularAcceleration * deltaTime;
    }

    //apply braking torque
    if (brake > 0){
        float brakeTorque = brake * maxBrakeTorque;
        float angularDeceleration = brakeTorque / inertia;
        angularSpeed -= angularDeceleration * deltaTime;
    }

    //apply passive drag when neither throttle nor brake are applied
    if (throttle == 0 && brake == 0) {
        float dragTorque = 0.05f * maxTorque;
        float angularDeceleration = dragTorque / inertia;
        angularSpeed -= angularDeceleration * deltaTime;
    }
    //clamp angularSpeed to 0 to avoid going backward
    if (angularSpeed < 0){
        angularSpeed = 0;
    cout << "Angular speed: " << angularSpeed << endl;
    }

        //convert to linear speed
    this->speed = wheelRadius * angularSpeed;

    cout << "This speed: " << this->speed << endl;
    //limit speed to maxSpeed
    if (this->speed > maxSpeed){
        this->speed = maxSpeed;
    }

    cout << "Speed: " << speed << " m/s\n";

    battery.discharge(speed, deltaTime);  //each time the speed updates, update SOC

    return speed;
}


void Charger::stopCharging(Battery* battery) {
    if (battery) {
        battery->set_Q_current(0.0f); // TODO: Double check if this needs to be Q current set to 0 or current set to 0. If it is just "current" then we need a setCurrent function in the battery class
    }
    cout << "Charging stopped" << endl;
}
