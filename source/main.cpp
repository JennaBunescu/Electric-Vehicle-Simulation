#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../headers/components.h"
#include "../headers/driver_input.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  //for sf::Clock

using namespace std;

int main(){
    //Use Clock to track times between frames. This will be used for deltaTime later on in the code.
    sf::Clock deltaClock;

    //This creates a window of the specified parameters
    sf::RenderWindow window(sf::VideoMode(800, 600), "Electric Vehicle Simulation");

    //Initializing classes
    Battery battery;
    DriverInput input;
    Motor motor;
    float vehicleSpeed = 0.0;

    // Load car image (bird's eye view)
    sf::Texture carTexture;
    if (!carTexture.loadFromFile("./assets/car1.png")) {
        cout << "❌ Error loading car image" << endl;
        return -1;
    }

    sf::Sprite carSprite(carTexture);
    carSprite.setScale(0.3f, 0.3f); // Make car smaller

    // Load road images (road1, road2, road3)
    sf::Texture roadTexture1, roadTexture2, roadTexture3;
    if (!roadTexture1.loadFromFile("./assets/road1.png") || !roadTexture2.loadFromFile("./assets/road2.png") || !roadTexture3.loadFromFile("./assets/road3.png")) {
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
    if (!batteryTexture.loadFromFile("./assets/battery1.png")) {
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

    // Create a background box for the speed display (box)
    sf::RectangleShape speedBox(sf::Vector2f(200, 50)); // Box for speed display
    speedBox.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black

    // Create a text object for speed
    sf::Text speedText;
    speedText.setFont(font);
    speedText.setCharacterSize(24);
    speedText.setFillColor(sf::Color::White);

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
            cout << "Throttle: " << input.get_throttle() << endl; // Debug output
        } else {
            input.set_throttle(0.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            input.set_brake(1.0f);  // Full brake
            cout << "Brake: " << input.get_brake() << endl; // Debug output
        } else {
            input.set_brake(0.0f);
        }

        // Update speed based on input and battery state
        vehicleSpeed = motor.updateSpeed(input, battery, deltaTime);
        cout << "Current Speed: " << vehicleSpeed << endl; // Debug output

        // Move the road upwards (simulate driving)
        roadYPosition += vehicleSpeed * deltaTime;  // Adjust road speed based on car speed
        if (roadYPosition >= window.getSize().y) {
            roadYPosition = 0;  // Reset road position to loop
        }

        // Set the position of the road textures (moving upwards)
        roadSprite1.setPosition(0, roadYPosition);
        roadSprite2.setPosition(0, roadYPosition - window.getSize().y);
        roadSprite3.setPosition(0, roadYPosition - 2 * window.getSize().y);

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

        // Update speed text
        speedText.setString("Speed: " + to_string(static_cast<int>(vehicleSpeed)) + " m/s");
        speedText.setPosition(20, window.getSize().y / 2 - 50);  // Position it below the battery

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

        // Draw speed box and speed text
        window.draw(speedBox);
        window.draw(speedText);

        // Display everything
        window.display();

        // Delay for smoother movement
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }

    return 0;
}
