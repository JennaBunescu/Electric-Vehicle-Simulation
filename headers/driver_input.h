#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H

class DriverInput {
private:
    float throttlePosition;
    float brakePosition;

public:
    DriverInput();

    float get_throttle();
    float get_brake();
    
    void set_throttle(float intensity);  // Change from double to float
    void set_brake(float intensity);     // Change from double to float
};

#endif // DRIVER_INPUT_H
