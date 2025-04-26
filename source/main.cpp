#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../headers/components.h"
#include "../headers/driver_input.h"
#include <SFML/Graphics.hpp>

//big thanks to chatgpt lol
//


using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Electric Vehicle Simulation");

    Battery battery;

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
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

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
        battery.discharge(1.0f);

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
    }

    return 0;
}
