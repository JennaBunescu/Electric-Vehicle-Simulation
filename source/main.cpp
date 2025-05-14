#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../headers/components.h"
#include "../headers/driver_input.h"
#include "../headers/vehicle.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  // For sf::Clock

using namespace std;

// Function to initialize SFML assets like fonts, textures, and sprites
bool loadAssets(sf::Font &font, sf::Texture &carTexture, sf::Texture &roadTexture1,
                sf::Texture &roadTexture2, sf::Texture &roadTexture3, sf::Texture &batteryTexture) {
    if (!font.loadFromFile("./Roboto.ttf")) {
        cout << "❌ Error loading font" << endl;
        return false;
    }
    
    if (!carTexture.loadFromFile("./assets/car1.png") || 
        !roadTexture1.loadFromFile("./assets/road1.png") || 
        !roadTexture2.loadFromFile("./assets/road2.png") || 
        !roadTexture3.loadFromFile("./assets/road3.png") ||
        !batteryTexture.loadFromFile("./assets/battery1.png")) {
        cout << "❌ Error loading textures" << endl;
        return false;
    }
    return true;
}

// Function to create and position UI elements
void createUIElements(sf::Font &font, sf::Text &wheelLabel, sf::Text &speedLabel, sf::Text &wheelValue, 
                      sf::Text &speedValue, sf::RectangleShape &wheelBox, sf::RectangleShape &speedBox) {
    wheelLabel.setString("Wheel Radius:");
    speedLabel.setString("Max Speed:");

    wheelLabel.setFont(font);
    speedLabel.setFont(font);
    wheelValue.setFont(font);
    speedValue.setFont(font);

    wheelLabel.setCharacterSize(20);
    speedLabel.setCharacterSize(20);
    wheelValue.setCharacterSize(20);
    speedValue.setCharacterSize(20);

    wheelBox.setSize(sf::Vector2f(120, 30));
    speedBox.setSize(sf::Vector2f(120, 30));

    wheelLabel.setPosition(20, 500);  // Adjust position based on window size
    speedLabel.setPosition(20, 550);  // Adjust position based on window size
    wheelValue.setPosition(160, 500);
    speedValue.setPosition(160, 550);

    wheelBox.setFillColor(sf::Color::White);
    speedBox.setFillColor(sf::Color::White);
}

// Main function
int main() {
    // Initialize window and clock
    sf::Clock deltaClock;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Electric Vehicle Simulation", sf::Style::Fullscreen);
    
    // Load assets (font, textures, etc.)
    sf::Font font;
    sf::Texture carTexture, roadTexture1, roadTexture2, roadTexture3, batteryTexture;
    if (!loadAssets(font, carTexture, roadTexture1, roadTexture2, roadTexture3, batteryTexture)) {
        return -1;  // Exit if assets couldn't be loaded
    }

    // Create sprites
    sf::Sprite carSprite(carTexture);
    carSprite.setScale(0.3f, 0.3f);

    sf::Sprite roadSprite1(roadTexture1), roadSprite2(roadTexture2), roadSprite3(roadTexture3);
    roadSprite1.setScale(float(window.getSize().x) / roadTexture1.getSize().x, 
                         float(window.getSize().y) / roadTexture1.getSize().y);
    roadSprite2.setScale(float(window.getSize().x) / roadTexture2.getSize().x, 
                         float(window.getSize().y) / roadTexture2.getSize().y);
    roadSprite3.setScale(float(window.getSize().x) / roadTexture3.getSize().x, 
                         float(window.getSize().y) / roadTexture3.getSize().y);

    // Create battery sprite
    sf::Sprite batterySprite(batteryTexture);
    batterySprite.setScale(0.3f, 0.3f);
    
    // Initialize DriverInput, Motor, Battery
    DriverInput input;
    Motor motor;
    Battery battery;
    
    float vehicleSpeed = 0, ambientTemp = 25, batteryTemp = 0;

    // Create and initialize UI elements
    sf::Text wheelLabel, speedLabel, wheelValue, speedValue;
    sf::RectangleShape wheelBox, speedBox;
    createUIElements(font, wheelLabel, speedLabel, wheelValue, speedValue, wheelBox, speedBox);

    // Main loop
    float roadYPosition = 0.0f;
    EV myEV;

    while (window.isOpen()) {
        sf::Event event;
        float deltaTime = deltaClock.restart().asSeconds();

        // Handle window events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Handle driver inputs
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            myEV.powerOff();
            // Reset input parameters
        }

        // Update vehicle speed and battery temperature
        vehicleSpeed = motor.updateSpeed(input, battery, deltaTime);
        batteryTemp = battery.updateTemperature(deltaTime, ambientTemp);

        // Move the road upwards (simulate driving)
        roadYPosition += vehicleSpeed * deltaTime;
        if (roadYPosition >= window.getSize().y) {
            roadYPosition = 0;
        }

        // Set positions of road textures
        roadSprite1.setPosition(0, roadYPosition);
        roadSprite2.setPosition(0, roadYPosition - window.getSize().y);
        roadSprite3.setPosition(0, roadYPosition - 2 * window.getSize().y);

        // Update battery sprite scale based on SOC
        float socScale = battery.get_SOC() / 100.0f;
        batterySprite.setScale(socScale * 0.3f, 0.3f);

        // Update SOC text
        sf::Text socText;
        socText.setFont(font);
        socText.setString("Battery SOC: " + to_string(static_cast<int>(battery.get_SOC())) + "%");
        socText.setCharacterSize(24);
        socText.setFillColor(sf::Color::White);
        socText.setPosition(20, window.getSize().y / 2 + 30);

        // Update speed text
        sf::Text speedText;
        speedText.setFont(font);
        speedText.setString("Speed: " + to_string(static_cast<int>(vehicleSpeed)) + " m/s");
        speedText.setCharacterSize(24);
        speedText.setFillColor(sf::Color::White);
        speedText.setPosition(20, window.getSize().y / 2 - 50);

        // Clear window and redraw
        window.clear(sf::Color(135, 206, 235)); // Sky blue background
        window.draw(roadSprite1);
        window.draw(roadSprite2);
        window.draw(roadSprite3);
        window.draw(carSprite);
        window.draw(batterySprite);
        window.draw(socText);
        window.draw(speedText);

        // Draw UI elements (wheel radius and speed)
        window.draw(wheelLabel);
        window.draw(wheelBox);
        window.draw(wheelValue);
        window.draw(speedLabel);
        window.draw(speedBox);
        window.draw(speedValue);

        window.display();

        // Delay for smoother movement
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }

    return 0;
}
