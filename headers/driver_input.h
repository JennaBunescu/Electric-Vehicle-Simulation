#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H

class DriverInput{
private:

    float throttlePosition;  // 0.0 to 1.0
    float brakePosition;     // 0.0 to 1.0
    
public:
    DriverInput();
    float get_throttle();
    float get_brake();
    void pressThrottle(double intensity);
    void pressBrake(double intensity);
    void releaseThrottle();
    void releaseBrake();
    double getThrottle() const;
    double getBrake() const;
};

#endif