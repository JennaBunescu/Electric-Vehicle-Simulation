#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include "../headers/components.h"
#include "../headers/driver_input.h"
#include <SFML/Graphics.hpp>

using namespace std;

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    Battery battery1;

    sf::Clock printClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        if (printClock.getElapsedTime().asSeconds() > 1) {
            cout << "Battery SOC: " << battery1.get_SOC() << "%" << endl;
            printClock.restart();
        }

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
