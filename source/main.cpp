#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../headers/components.h"
#include "../headers/driver_input.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  // For sf::Clock

//steps:
//- finish all classes (biggest part - there is a lot of interplay between classes)
//- add more classes maybe (? environment per chance)
//- incorporate file I/O by creating a graph of our battery SOC over time
//      here we could use vectors to log our SOC data over time
//- I'm also thinking of adding an "admin console" that would allow us
//      to set the different parameters in real time so we can see how the 
//      car would react to that

using namespace std;

int main() {
    // At the top of main(), before the loop:
    sf::Clock deltaClock;  // Tracks time between frames

    sf::RenderWindow window(sf::VideoMode(800, 600), "Electric Vehicle Simulation");

    Battery battery;
    DriverInput input;
    Motor motor;
    float vehicleSpeed = 0.0;

    // Load car image (bird's eye view)
    sf::Texture carTexture;
    if (!carTexture.loadFromFile("/Users/jennabunescu/Desktop/Project/Electric-Vehicle-Simulation/car1.png")) {
        cout << "❌ Error loading car image" << endl;
        return -1;
    }

    sf::Sprite carSprite(carTexture);
    carSprite.setScale(0.3f, 0.3f); // Make car smaller

    // Load road images (road1, road2, road3)
    sf::Texture roadTexture1, roadTexture2, roadTexture3;
    if (!roadTexture1.loadFromFile("road1.png") || !roadTexture2.loadFromFile("road2.png") || !roadTexture3.loadFromFile("road3.png")) {
        cout << "❌ Error loading road textures" << endl;
        return -1;
    }

    sf::Sprite roadSprite1(roadTexture1);
    sf::Sprite roadSprite2(roadTexture2);
    sf::Sprite roadSprite3(roadTexture3);

    // Scale road images to fit the window size
    roadSprite1.setScale(float(window.getSize().x) / roadTexture1.getSize().x, float(window.getSize().y) / roadTexture1.getSize().y);
    roadSprite2.setScale(float(window.getSize().x) / roadTexture2.getSize().x, float(window.getSize().y) / roadTexture2.getSize().y);
    roadSprite3.setScale(float(window.getSize().x) / roadTexture3.getSize().x, float(window.getSize().y) / roadTexture3.getSize().y);

    // Load battery image (make sure path is correct)
    sf::Texture batteryTexture;
    if (!batteryTexture.loadFromFile("./source/battery1.png")) {
        cout << "❌ Error loading battery texture from image. Check file path and format." << endl;
        return -1;
    } else {
        cout << "✅ Battery image loaded successfully!" << endl;
    }

    sf::Sprite batterySprite(batteryTexture);
    batterySprite.setColor(sf::Color(255, 255, 255));  // White for visibility
    batterySprite.setScale(0.3f, 0.3f);  // Scale the battery sprite

    // Load font for text
    sf::Font font;
    if (!font.loadFromFile("./Roboto.ttf")) {
        cout << "❌ Error loading font" << endl;
        return -1;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // Create a background for the battery SOC display (box)
    sf::RectangleShape batteryBox(sf::Vector2f(200, 100)); // Box for SOC, on the left
    batteryBox.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black

    // Main loop
    float roadYPosition = 0.0f;  // Starting position of the road texture

    while (window.isOpen()) {
        sf::Event event;
        float deltaTime = deltaClock.restart().asSeconds();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

                // --- HANDLE DRIVER INPUT ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            input.set_throttle(1.0f);  // Full throttle
        } else {
            input.set_throttle(0.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            input.set_brake(1.0f);  // Full brake
        } else {
            input.set_brake(0.0f);
        }

        // Update speed based on input and battery state
        vehicleSpeed = motor.updateSpeed(input, battery, deltaTime);

        // Move the road upwards (simulate driving)
        roadYPosition += 5.0f;  // Adjust speed by modifying this value
        if (roadYPosition >= window.getSize().y) {
            roadYPosition = 0;  // Reset road position to loop
        }

        // Set the position of the road textures (moving upwards)
        roadSprite1.setPosition(0, roadYPosition);
        roadSprite2.setPosition(0, roadYPosition - window.getSize().y);
        roadSprite3.setPosition(0, roadYPosition - 2 * window.getSize().y);

        // Simulate battery discharge
        // battery.discharge(1.0f, delta_t);

        // Update battery bar scale based on SOC
        float socScale = battery.get_SOC() / 100.0f;
        batterySprite.setScale(socScale * 0.3f, 0.3f);  // Scale width based on SOC and keep height constant

        // Center battery sprite in the black box
        float spriteWidth = batterySprite.getGlobalBounds().width;
        float spriteHeight = batterySprite.getGlobalBounds().height;
        batterySprite.setPosition(20, window.getSize().y / 2 - spriteHeight / 2); // Position battery box on the left side

        // Update text for SOC display
        text.setString("Battery SOC: " + to_string(static_cast<int>(battery.get_SOC())) + "%");
        text.setPosition(20, window.getSize().y / 2 + 30);  // Place text below the battery image

        // Clear window with sky blue background
        window.clear(sf::Color(135, 206, 235)); // Sky blue background

        // Draw the road
        window.draw(roadSprite1);
        window.draw(roadSprite2);
        window.draw(roadSprite3);

        // Draw the car, move it to the right side of the window
        carSprite.setPosition(window.getSize().x - carSprite.getGlobalBounds().width - 20, window.getSize().y / 2 - carSprite.getGlobalBounds().height / 2);
        window.draw(carSprite);

        // Draw the battery box and the battery image with SOC text
        window.draw(batteryBox);
        window.draw(batterySprite);
        window.draw(text);

        // Display everything
        window.display();

        // Delay for smoother movement
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS

    // Calculate deltaTime (in seconds)
    float deltaTime = deltaClock.restart().asSeconds();

    // // --- HANDLE DRIVER INPUT ---
    // DriverInput input;
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
    //     input.set_throttle(1.0f);  // Full throttle
    // } else {
    //     input.set_throttle(0.0f);
    // }

    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
    //     input.set_brake(1.0f);  // Full brake
    // } else {
    //     input.set_brake(0.0f);
    // }

    // // Get current battery voltage
    // float batteryVoltage = battery.get_voltage();  // Assuming you have this method

    // // Call updateMotor to compute new speed
    // angularSpeed = updateMotor(input, batteryVoltage, deltaTime, angularSpeed);

    // // (Optional) Use angularSpeed to influence visuals, e.g., road scroll speed
    // roadYPosition += angularSpeed * deltaTime * 50.0f;  // Adjust multiplier as needed

    // Delay for smoother movement (can remove if using vsync or deltaTime properly)
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    
    }



    return 0;
}
