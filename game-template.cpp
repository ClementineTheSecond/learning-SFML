#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    sf::RenderWindow window(sf::VideoMode({200, 200}), "Game Template");

    const sf::Time timePerTick = sf::seconds(1.f / 60.f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= timePerTick) {
            timeSinceLastUpdate -= timePerTick;

            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();
            }
        }

        window.clear();
        // window.draw();
        window.display();
    }
    return 0;
}