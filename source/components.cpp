#include <iostream>
#include "../headers/driver_input.h"
#include "../headers/vehicle.h"
#include "../headers/components.h"
using namespace std;


//constructor that lets the user choose the values of the battery
Battery::Battery(float Q_max, float V_max, float R_internal, float heatCapacity, float stateOfHealth){
    this->Q_max = Q_max;
    this->Q_now = Q_max;
    this->V_max = V_max;
    this->R_internal = R_internal;
    this->stateOfHealth = stateOfHealth; 
    this->voltage = 0.9 * V_max; //nominal voltage is about 90% of the max voltage 
    this->current = 0; 
    this->heatCapacity = heatCapacity;
    this->heatTransferCoeff = 0.6; 
    this->temperature = 25; 
    this->totalTimeSeconds = 0;     
    this->totalDistanceKm = 0; 


};

//default constructor
Battery::Battery(){
    Q_max = 150; //Realistic Max Charge for an EV
    Q_now = 150; //Starting fully charged so Q_now = Q_max
    V_max = 420; //Fully charged state
    R_internal = 0.02; //Internal resistance in ohms typical for EVs
    stateOfHealth = 100; //New battery starts at full health
    voltage = 400; //Nominal voltage 
    current = 0; //No current flow initially
    heatCapacity = 1000; //Realistic thermal mass of battery
    heatTransferCoeff = 0.6; //Realistic heat transfer coefficient
    temperature = 25; //Room temperature in Celsius at the start
    totalTimeSeconds = 0; //by default starts at 0
    totalDistanceKm = 0; //by default starts at 0


};


//@brief function that returns the state of charge of the battery
float Battery::get_SOC(){
    //the current state of charge in percent is the ratio of charge remaining and max charge capacity
    return (Q_now / Q_max) * 100.0;
}


void Battery::setCurrent(float I) {
    current = I;
}

//@brief function that discharges the battery by modifyin the current amount of charge in the battery
//based on speed and time. Discharge rate is affected by temperature. This function is called every fraction of a second in main. 
//@param speed - the current speed of the car, delta_t - the change in time (which would be the interval between each frame)
void Battery::discharge(float speed, float delta_t){
    //The discharge rate coefficient
    float baseDischargeRate = 10; //0.01

    //Temperature factor adjustment
    float tempFactor = 1.0; //Base factor at reasonable temperatures 
    if (temperature < 0){
        tempFactor = 0.7;  //When temperature is below 0 C, discharge is 30% less effective
    } else if (temperature > 40.0){
        tempFactor = 1.2;  //There is 20% more discharge at high temperatures
    }

    //Calculate deltaQ (change in charge) based on the Base discharge rate, temperature factor
    //Assume a linear discharge rate proportional to speed and delta_t
    float deltaQ = baseDischargeRate * speed * delta_t * tempFactor /3600; //Divide by 3600 because delta_t is in seconds and we need Q_now in ampere - hours
    Q_now -= deltaQ; //Decrease the current charge level 

    //Current is negative when discharging (because the battery is supplying current to the motor)
    current = -deltaQ / delta_t;

    //Prevent battery from going below 0
    if (Q_now < 0) {
        Q_now = 0;
    }
}

//@brief function that charges the battery. This function is called when the EV is going through a charging station.
//@param V_applied - Charging station applies a voltage, delta_t - how long was this voltage applied for, isFull - indicates if the battery is full
void Battery::charge(float V_applied, float delta_t, bool &isFull){
    //The change in charge is the current applied (V_applied/R_internal) times the change in time (which will be every frame)
    float deltaQ = delta_t*V_applied/R_internal; 
    current = deltaQ / delta_t; //Update current. Current is positive for charging as the battery is a receiver of current.

    //Ensure that Q_now is capped at Q_max
    if(Q_now < Q_max){
        Q_now += deltaQ;
        if (Q_now >= Q_max){
            Q_now = Q_max;
        }
    } else{ //If battery is fully charged, don't change anything and just set isFull to true
        isFull = true;
        return;
    }

}

//@brief function that changes the temperature of the battery
//@param delta_t - time elapsed, ambientTemp - temperature of the environment
//@return temperature
float Battery::updateTemperature(float delta_t, float ambientTemp){

    //Q = I^2 * R * t
    float heatGenerated = current * current * R_internal * delta_t;

    //Q = h * (T_batt - T_ambient) * t
    float cooling = heatTransferCoeff * (temperature - ambientTemp) * delta_t;

    //The net heaet change 
    float netHeat = heatGenerated - cooling;

    //Temperature change = Q / C
    float deltaTemp = netHeat / heatCapacity;

    //Update temperature attribute value
    this->temperature += deltaTemp;

    return temperature;
}

//@brief update the battery's state of health based on usage
//@param delta_t - time elapsed (to update function every call)
void Battery::degradeSOH(float delta_t){
    //Check if the battery temperature is above the threshold (40°C)
    if (temperature > 40){
        //Decrease the state of health proportionally to by how much the temperature
        //exceeds 40°C and how long this condition lasts (delta_t).
        stateOfHealth -= 0.001 * delta_t * (temperature - 40);
        //Cap at zero
        if (stateOfHealth < 0) {
            stateOfHealth = 0;
        }
    }
}

//@brief function that adds to current charge level after regenerative braking was applied
//@param deltaQ - the amount of charge gained from regenerative braking
void Battery::rechargeFromRegen(float deltaQ){
    Q_now += deltaQ;
    //cap Q_now at Q_max
    if (Q_now > Q_max){ 
        Q_now = Q_max; 
    }
}


void Battery::degradeWithCycle(float deltaQ) {
    // Static variable to accumulate charge moved over multiple calls (across cycles)
    static float cycleCharge = 0;

    // Add the absolute value of deltaQ (charge used or replenished) to the cycle accumulator
    cycleCharge += abs(deltaQ);

    // When the accumulated charge reaches or exceeds the battery's max capacity,
    // we consider one full charge-discharge cycle completed
    if (cycleCharge >= Q_max) {
        // Decrease the state of health (SOH) by 2% per full cycle
        stateOfHealth -= 0.02f;

        // Prevent SOH from dropping below 0%
        if (stateOfHealth < 0) 
            stateOfHealth = 0;

        // Reset the cycle charge counter for the next cycle
        cycleCharge = 0;
    }
}



//setters
void Battery::set_Q_max(float Q){
    Q_max = Q;
}

void Battery::set_Q_current(float Q){
    Q_now = Q;
}

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


/////////////////////////////////////////////////////////////////////////////////////////
Motor::Motor(){
    maxSpeed = 100;
}

bool Motor::isRegenerating(DriverInput& input){
    return (speed > 0 && input.get_brake() > 0);
}


float Motor::calculateRegenPower(DriverInput &input, EV &vehicle) {
    if (!isRegenerating(input)) return 0.0f;

    // Basic model: regen torque proportional to braking
    float regenTorque = input.get_brake() * regenEfficiency * maxTorque;

    // Convert torque to power: P = torque × angular velocity
    float angularVelocity = speed;
    float power = regenTorque * angularVelocity; 

    if (power > maxRegenPower) {
        return maxRegenPower;
    } else {
        return power;
    }

}

void Motor::applyRegenerativeBraking(DriverInput &input, EV &vehicle, Battery& battery, float deltaTime) {
    if (!isRegenerating(input)) return;

    float regenPower = calculateRegenPower(input, vehicle);

    if (regenPower > 0.0f) {
        float regenVoltage = battery.get_V_max(); // or nominal voltage
        float regenCurrent = regenPower / regenVoltage;

        float deltaQ = regenCurrent * deltaTime;

        battery.rechargeFromRegen(deltaQ);  // custom method in Battery
        battery.setCurrent(regenCurrent);   // for monitoring/display
    }
}


float Motor::updateSpeed(DriverInput &input, EV &vehicle, Battery &battery, float deltaTime) {
    if (isRegenerating(input)) {
        applyRegenerativeBraking(input, vehicle, battery, deltaTime);
    }

    static float angularSpeed = 0;

    float throttle = input.get_throttle();
    float brake = input.get_brake();

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
    this->speed = vehicle.get_wheelRadius() * angularSpeed;

    // Clamp to max speed
    if (this->speed > maxSpeed) {
        this->speed = maxSpeed;
    }

    // Discharge battery based on current speed
    battery.discharge(speed, deltaTime);

    return speed;
}

void Motor:: setMaxRegenPower(float power) {
    maxRegenPower = power;
}

float Motor:: getMaxRegenPower() const {
    return maxRegenPower;
}

float Motor::updateTemperature(float delta_t, float ambientTemp) {
    // Calculate heat generated in the motor due to current and resistance (I^2 * R * t)
    float heatGenerated = current * current * R_internal * delta_t;

    // Calculate heat lost to the environment via convection (h * (T_motor - T_ambient) * t)
    float cooling = heatTransferCoeff * (temperature - ambientTemp) * delta_t;

    // Net heat change inside the motor
    float netHeat = heatGenerated - cooling;

    // Temperature change (deltaTemp = netHeat / heatCapacity)
    float deltaTemp = netHeat / heatCapacity;

    // Update motor temperature
    temperature += deltaTemp;

    return temperature;
}



///////////////////////////////////////////////////////////////////////////////////////////


// Charger::Charger() {
//     isCharging = false;
//     maxPowerOutput = 10000.0f; // watts, example max power output
//     efficiency = 0.9f; // 90% efficiency
// }

// void startCharging(Battery* battery) {
//     isCharging = true;
//     // simple charging logic
//     float chargingVoltage = battery->get_V_max();
//     float chargingCurrent = maxPowerOutput / chargingVoltage * efficiency;
//     battery->charge(chargingVoltage, 1.0f, isCharging); // 1 second step, update isCharging flag
// }

// void stopCharging(Battery* battery) {
//     isCharging = false;
// }