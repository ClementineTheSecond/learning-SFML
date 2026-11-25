#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    // window
    constexpr sf::Vector2f windowSize = {200.f, 200.f};
    sf::RenderWindow window(sf::VideoMode(static_cast<sf::Vector2u>(windowSize)), "Snake");

    // logic
    constexpr sf::Vector2i boardDimensions = {16, 16};

    // time
    constexpr sf::Time dt = sf::seconds(1.f / 60.f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= dt) {
            timeSinceLastUpdate -= dt;

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