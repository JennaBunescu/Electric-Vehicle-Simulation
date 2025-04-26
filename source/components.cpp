#include <iostream>
#include "../headers/components.h"
using namespace std;


Battery::Battery(){
    Q_max = 100;
    Q_current = 100;
    V_max = 0;
    R_internal = 0;
    stateOfCharge = 100;
    stateOfHealth = 100;
    temperature = 0;
    voltage = 0;
    current = 0;
};

float Battery::get_SOC() {
    //the current state of charge in percent is the ratio of charge remaining and max charge capacity
    return (Q_current / Q_max) * 100.0;
}

//this function should be called every time the speed changes, 
//to register the previous speed and for how long it ran
//or another option is to update this function every fraction of a second and 
//use that time increment instead of total time, and keep updating 
//the current charge

/*void Battery::discharge(float speed, float time){
    float deltaQ = 0.01*speed*time;
    Q_current -= deltaQ;
}*/

// Option 2: This function should be called repeatedly every fraction of a second
// (e.g., every 0.1s), and it updates the battery charge incrementally.
void Battery::discharge(float speed) {
    const float delta_t = 0.1; // Time step in seconds (adjust as needed)
    float deltaQ = 0.01 * speed * delta_t;
    Q_current -= deltaQ;

    // Prevent battery from going below 0
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

