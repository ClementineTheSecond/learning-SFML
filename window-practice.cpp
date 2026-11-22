#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({200, 200}), "Window Practice");

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
    return 0;
}