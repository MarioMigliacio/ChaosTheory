#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("Starting ChaosTheory...");
    sf::RenderWindow window(sf::VideoMode(800, 600), "ChaosTheory");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.display();
    }

    return 0;
}