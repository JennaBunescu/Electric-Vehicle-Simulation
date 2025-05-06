#include <iostream>
#include <string>
using namespace std;

// Parent Classes
class Vehicle {
public:
    string brand;
    int speed;

    Vehicle(string b, int s);
    virtual void display();
    void accelerate();
    void decelerate();
    virtual void honk();
    void changeLanes(string direction);
    void turnSignal(string direction);
};

Vehicle::Vehicle(string b, int s) {
    brand = b;
    speed = s;
}

void Vehicle::display() {
    cout << "Vehicle Brand: " << brand << ", Speed: " << speed << " km/h" << endl;
}

void Vehicle::accelerate() {
    speed += 5;
    cout << brand << " accelerated to " << speed << " km/h" << endl;
}

void Vehicle::decelerate() {
    speed -= 5;
    if (speed < 0) speed = 0;
    cout << brand << " decelerated to " << speed << " km/h" << endl;
}

void Vehicle::honk() {
    cout << brand << " honks!" << endl;
}

void Vehicle::changeLanes(string direction) {
    cout << brand << " changes lane to the " << direction << "." << endl;
}

void Vehicle::turnSignal(string direction) {
    cout << brand << " activates " << direction << " turn signal." << endl;
}

class RoadType {
public:
    string surface;
    int speedLimit;

    RoadType(string surf, int limit);
    virtual void display();
};

RoadType::RoadType(string surf, int limit) {
    surface = surf;
    speedLimit = limit;
}

void RoadType::display() {
    cout << "Road Surface: " << surface << ", Speed Limit: " << speedLimit << " km/h" << endl;
}

class Sign {
public:
    string text;
    string color;

    Sign(string t, string c);
    virtual void display();
};

Sign::Sign(string t, string c) {
    text = t;
    color = c;
}

void Sign::display() {
    cout << "Sign: " << text << " (" << color << ")" << endl;
}

// Vehicle Children
class Car : public Vehicle {
public:
    int doors;

    Car(string b, int s, int d);
    void display() override;
    void honk() override;
};

Car::Car(string b, int s, int d) : Vehicle(b, s) {
    doors = d;
}

void Car::display() {
    Vehicle::display();
    cout << "Number of Doors: " << doors << endl;
}

void Car::honk() {
    cout << brand << " honks!" << endl;
}

class Bicycle : public Vehicle {
public:
    bool hasBell;

    Bicycle(string b, int s, bool bell);
    void display() override;
    void honk() override;
};

Bicycle::Bicycle(string b, int s, bool bell) : Vehicle(b, s) {
    hasBell = bell;
}

void Bicycle::display() {
    Vehicle::display();
    cout << "Has Bell: " << (hasBell ? "Yes" : "No") << endl;
}

void Bicycle::honk() {
    if (hasBell)
        cout << brand << " rings the bell: Ring Ring!" << endl;
    else
        cout << brand << " has no bell to ring." << endl;
}

class Motorcycle : public Vehicle {
public:
    Motorcycle(string b, int s);
    void display() override;
    void honk() override;
};

Motorcycle::Motorcycle(string b, int s) : Vehicle(b, s) {}

void Motorcycle::display() {
    Vehicle::display();
}

void Motorcycle::honk() {
    cout << brand << " honks!" << endl;
}

class Truck : public Vehicle {
public:
    Truck(string b, int s);
    void display() override;
    void honk() override;
};

Truck::Truck(string b, int s) : Vehicle(b, s) {}

void Truck::display() {
    Vehicle::display();
}

void Truck::honk() {
    cout << brand << " honks!" << endl;
}

class Bus : public Vehicle {
public:
    int passengerCapacity;

    Bus(string b, int s, int passengers);
    void display() override;
    void honk() override;
};

Bus::Bus(string b, int s, int passengers) : Vehicle(b, s) {
    passengerCapacity = passengers;
}

void Bus::display() {
    Vehicle::display(); // Fix the infinite recursion!
    cout << "Passenger Capacity: " << passengerCapacity << endl;
}

void Bus::honk() {
    cout << brand << " honks!" << endl;
}

// int main() {
//     // Vehicles
//     Car myCar("Toyota", 120, 4);
//     Bicycle myBike("Trek", 25, true);
//     Motorcycle myMotorcycle("Harley-Davidson", 150);
//     Truck myTruck("Volvo", 90);
//     Bus myBus("Mercedes", 80, 50);

//     cout << "--- Vehicles ---" << endl;
//     myCar.display();
//     myBike.display();
//     myMotorcycle.display();
//     myTruck.display();
//     myBus.display();

//     cout << "\n--- Honking ---" << endl;
//     myCar.honk();
//     myBike.honk();
//     myMotorcycle.honk();
//     myTruck.honk();
//     myBus.honk();

//     cout << "\n--- Accelerate and Decelerate ---" << endl;
//     myCar.accelerate();
//     myBike.decelerate();

//     cout << "\n--- Changing Lanes ---" << endl;
//     myCar.changeLanes("left");
//     myMotorcycle.changeLanes("right");

//     cout << "\n--- Turn Signals ---" << endl;
//     myCar.turnSignal("left");
//     myTruck.turnSignal("right");

//     return 0;
// }
