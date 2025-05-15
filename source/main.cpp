#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "../headers/components.h"
#include "../headers/driver_input.h"
#include "../headers/vehicle.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>  // For sf::Clock
#include <SFML/Window.hpp>

using namespace std;

bool isMouseOver(const sf::RectangleShape &button, const sf::Vector2f &mousePos) {
    return button.getGlobalBounds().contains(mousePos);
}

// Helper function to create a button
void setupButton(sf::RectangleShape &button, sf::Vector2f size, sf::Vector2f position, sf::Color color) {
    button.setSize(size);
    button.setPosition(position);
    button.setFillColor(color);
}



// Function to initialize SFML assets like fonts, textures, and sprites
bool loadAssets(sf::Font &font, sf::Texture &carTexture, sf::Texture &roadTexture1,
                sf::Texture &roadTexture2, sf::Texture &roadTexture3, sf::Texture &batteryTexture, sf::Texture &uiBoxTexture) {
    if (!font.loadFromFile("./assets/Roboto.ttf")) {
        cout << "❌ Error loading font" << endl;
        return false;
    }
    
    if (!carTexture.loadFromFile("./assets/car1.png") || 
        !roadTexture1.loadFromFile("./assets/road1.png") || 
        !roadTexture2.loadFromFile("./assets/road2.png") || 
        !roadTexture3.loadFromFile("./assets/road3.png") ||
        !uiBoxTexture.loadFromFile("./assets/display.png") ||
        !batteryTexture.loadFromFile("./assets/battery1.png")) {
        cout << "❌ Error loading textures" << endl;
        return false;
    }
    return true;
}

// Function to create and position UI elements
void createUIElements(sf::Font &font, sf::Text &wheelValue, 
                      sf::Text &speedValue, sf::RectangleShape &wheelBox, sf::RectangleShape &speedBox) {

}



// Main function
int main(){
    // Initialize window and clock
    sf::Clock deltaClock;
    // Initialize window in normal mode (not fullscreen)
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Electric Vehicle Simulation");  // Example size (1280x720)

    sf::RectangleShape togglePowerBtn, defaultBtn, runBtn, statsBtn, backBtn;
    setupButton(togglePowerBtn, {200, 50}, {1000, 20}, sf::Color::Green);
    setupButton(defaultBtn,     {200, 50}, {540, 300}, sf::Color::Yellow);
    setupButton(runBtn,         {200, 50}, {540, 400}, sf::Color::Cyan);
    setupButton(statsBtn,       {50, 100}, {10, 300}, sf::Color(100, 100, 250));
    setupButton(backBtn,        {150, 50}, {1060, 20}, sf::Color(200, 100, 100));

    // Load assets (font, textures, etc.)
    sf::Font font;
    sf::Texture carTexture, roadTexture1, roadTexture2, roadTexture3, batteryTexture, uiBoxTexture;
    if (!loadAssets(font, carTexture, roadTexture1, roadTexture2, roadTexture3, batteryTexture, uiBoxTexture)) {
        return -1;  // Exit if assets couldn't be loaded
    }
    // Setup textbox background
    sf::RectangleShape textBox;
    textBox.setSize(sf::Vector2f(200, 40));
    textBox.setPosition(100, 100);
    textBox.setFillColor(sf::Color::White);
    textBox.setOutlineColor(sf::Color::Black);
    textBox.setOutlineThickness(2);

    // Setup text
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(105, 105);

    // Input state
    std::string currentInput;
    bool isFocused = false;


    // Create sprites
    sf::Sprite carSprite(carTexture);
    carSprite.setScale(0.3f, 0.3f);
    carSprite.setPosition(800, 350);  // Move car further to the right and higher (+X, +Y)

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
    batterySprite.setPosition(20, 380); // Place battery a little higher

    sf::Sprite uiBoxSprite;
    uiBoxSprite.setTexture(uiBoxTexture);
    uiBoxSprite.setPosition(15, 490); // Adjust based on your layout
    uiBoxSprite.setScale(0.5f, 0.5f); // Optional: resize to fit

    // Initialize DriverInput, Motor, Battery
    DriverInput input;
    Motor motor;
    Battery battery;
    
    float vehicleSpeed = 0, ambientTemp = 25, batteryTemp = 0;

    // Create and initialize UI elements
    sf::Text wheelLabel, speedLabel, wheelValue, speedValue;
    sf::RectangleShape wheelBox, speedBox;
    // Main loop
    float roadYPosition = 0.0f;
    EV myEV;


    sf::RectangleShape button(sf::Vector2f(150, 60));
    button.setPosition(125.f, 70.f);
    button.setFillColor(sf::Color(100, 100, 250));

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("EV ON");
    buttonText.setCharacterSize(30);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f,
                         textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(
        button.getPosition().x + button.getSize().x / 2.0f,
        button.getPosition().y + button.getSize().y / 2.0f);



    bool evOn = true;

        // Track mouse button previous state
    bool mouseWasPressed = false;



    while (window.isOpen()) {
        cout << "Hello" << endl;
        sf::Event event;
        float deltaTime = deltaClock.restart().asSeconds();


        // Handle window events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        // Get current mouse left button state
        bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        sf::Vector2i mousePosI = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));

        // Check for mouse press edge (pressed now, but was not pressed before)
        if (mousePressed && !mouseWasPressed) {
            // Only toggle if click is inside button bounds
            if (button.getGlobalBounds().contains(mousePosF)) {
                evOn = !evOn;

                if (evOn) {
                    buttonText.setString("EV ON");
                    button.setFillColor(sf::Color(100, 100, 250));
                } else {
                    buttonText.setString("EV OFF");
                    button.setFillColor(sf::Color(200, 50, 50));
                }
                // Re-center text after string update
                sf::FloatRect textRect = buttonText.getLocalBounds();
                buttonText.setOrigin(textRect.left + textRect.width / 2.0f,
                                     textRect.top + textRect.height / 2.0f);
                buttonText.setPosition(
                    button.getPosition().x + button.getSize().x / 2.0f,
                    button.getPosition().y + button.getSize().y / 2.0f);
            }
        }
        // Update mouse previous state for next frame
        mouseWasPressed = mousePressed;

        if (evOn)
            window.clear(sf::Color(200, 200, 200));
        else
            window.clear(sf::Color::Black);



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
        roadYPosition += vehicleSpeed * deltaTime * 5;
        if (roadYPosition >= window.getSize().y) {
            roadYPosition = 0;
        }

        // Set positions of road textures
        roadSprite1.setPosition(0, roadYPosition);
        roadSprite2.setPosition(0, roadYPosition - window.getSize().y);
        roadSprite3.setPosition(0, roadYPosition - 2 * window.getSize().y);

        // Battery status alert
        sf::Text alertText;
        alertText.setFont(font);
        alertText.setCharacterSize(50);
        alertText.setFillColor(sf::Color::Red);
        alertText.setStyle(sf::Text::Bold);
        alertText.setPosition(100, 600); // Position near top center

        float soc = battery.get_SOC();  // Already used in your SOC bar

        if (soc > 90) {
            alertText.setString("⚠️ Battery Full!");
            window.draw(alertText);
        } else if (soc <= 10.0f) {
            alertText.setString("⚠️ Battery Low!");
            window.draw(alertText);
        }


        // Update SOC text
        sf::Text socText;
        socText.setFont(font);
        socText.setString("Battery SOC: " + to_string(static_cast<int>(battery.get_SOC())) + "%");
        socText.setCharacterSize(30);
        socText.setFillColor(sf::Color::Black);  // Set text color to black
        socText.setPosition(50, 1000); 

        // Update speed text
        sf::Text speedText;
        speedText.setFont(font);
        speedText.setString("Speed: " + to_string(static_cast<int>(vehicleSpeed)) + " m/s");
        speedText.setCharacterSize(30);
        speedText.setFillColor(sf::Color::Black);  // Set text color to black
        speedText.setPosition(50, 500);  // Place text overlapping uiBoxSprite

                // Update SOC text
        sf::Text wheelLabel;
        wheelLabel.setFont(font);
        wheelLabel.setString("Battery SOC: " + to_string(static_cast<int>(battery.get_SOC())) + "%");
        wheelLabel.setCharacterSize(30);
        wheelLabel.setFillColor(sf::Color::Black);  // Set text color to black
        wheelLabel.setPosition(50, 100); 


        // Update speed text
        sf::Text speedLabel;
        speedLabel.setFont(font);
        speedLabel.setString("Speed: " + to_string(static_cast<int>(vehicleSpeed)) + " m/s");
        speedLabel.setCharacterSize(30);
        speedLabel.setFillColor(sf::Color::Black);  // Set text color to black
        speedLabel.setPosition(50, 200);  // Place text overlapping uiBoxSprite

        // Clear window and redraw
        window.clear(sf::Color(135, 206, 235)); // Sky blue background
        if (evOn){
        window.draw(roadSprite1);
        window.draw(roadSprite2);
        window.draw(roadSprite3);
        window.draw(carSprite);
        window.draw(batterySprite);
        window.draw(socText);
        window.draw(speedText);
        window.draw(alertText);
        

        // Draw UI elements (wheel radius and speed)
        window.draw(wheelLabel);
        window.draw(wheelBox);
        window.draw(wheelValue);
        window.draw(speedLabel);
        window.draw(speedBox);
        window.draw(speedValue);
        window.draw(uiBoxSprite);  // This is your "box image"
        }

        window.draw(button);
        window.draw(buttonText);
        window.display();

        // Delay for smoother movement
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // ~60 FPS
    }

    return 0;
}

