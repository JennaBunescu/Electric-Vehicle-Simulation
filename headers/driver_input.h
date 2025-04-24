#ifndef DRIVER_INPUT_H
#define DRIVER_INPUT_H

class DriverInput{
private:

    double throttle;  // 0.0 to 1.0
    double brake;     // 0.0 to 1.0
    
public:
    DriverInput();
    void pressThrottle(double intensity);
    void pressBrake(double intensity);
    void releaseThrottle();
    void releaseBrake();
    double getThrottle() const;
    double getBrake() const;
};

#endif