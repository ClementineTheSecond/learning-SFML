#include <SFML/Graphics.hpp>
#include <optional>

// This file was fully AI-generated
int main() {
    // SFML 3 VideoMode uses braces for dimensions
    sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        // SFML 3 event loop syntax
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}