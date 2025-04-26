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


float Battery::get_SOC() {
    //the current state of charge in percent is the ratio of charge remaining and max charge capacity
    return (Q_current / Q_max) * 100.0;
}

//this function should be called repeatedly every fraction of a second
//and it updates the battery charge incrementally.
void Battery::discharge(float speed) {
    const float delta_t = 0.1; // Time step in seconds (adjust as needed)
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




////// Motor


//function to update motor's current demand based on driver's throttle and brake input
void Motor::updateMotor(DriverInput& driverInput, float batteryVoltage){

    //calculate current demand based on throttle (simplified)
    if (driverInput.getThrottlePos() > 0){

        //accelerating: motor needs more current to provide power
        float desiredPower = driverInput.getThrottlePos() * powerRating;//desired power in kW
        currentDemand = (desiredPower * 1000) / batteryVoltage; //convert to current in Amps

        if (currentDemand > maxCurrent){
            currentDemand = maxCurrent;  //limit to max current
        }

        //calculate torque (simplified)
        torque = desiredPower * 1000 / maxSpeed;  //in N*m

    } else if (driverInput.getBrakePos() > 0){
        //regenerative brakng
        float regenPower = driverInput.getBrakePos() * powerRating;
        currentDemand = -regenPower * 1000 / batteryVoltage; //negative current for regenerative braking

        //regenerative torque (simplified)
        torque = regenPower * 1000 / maxSpeed;

    } else {
        //no current demand (idle state)
        currentDemand = 0;
        torque = 0;
    }

    //calculate motor efficiency (could impact current demand)
    currentDemand /= efficiency;//adjust for efficiency losses
}

//function to update vehicle speed based on motor torque
void Motor::updateSpeed() {
    //simplified model: vehicle speed is proportional to motor torque
    speed = torque * 0.1f;  //this is just a simplified relation for demonstration
    
    if (speed > maxSpeed){
        speed = maxSpeed;  //limit speed to max motor speed
    }
}