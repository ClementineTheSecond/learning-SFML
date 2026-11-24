#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // open window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Window Practice");
    window.setPosition({20, 20});

    // initial visuals
    sf::Texture texture;
    if (!texture.loadFromFile("assets/madeline.jpg")) {
        std::cerr << "Error: Could not load image from folder." << std::endl;
        return -1;
    }
    // sf::Sprite sprite(texture);

    sf::CircleShape circle(50.f);
    circle.setPosition({400.f, 20.f});
    circle.setFillColor(sf::Color(100, 250, 50));
    
    circle.setOutlineThickness(5.f);
    circle.setOutlineColor(sf::Color(250, 150, 100));

    sf::CircleShape cool_circle(50.f);
    cool_circle.setTexture(&texture);
    cool_circle.setTextureRect(sf::IntRect({50, 50}, {100, 100}));
    cool_circle.setPosition({200.f, 200.f});

    sf::RectangleShape rectangle({120.f, 50.f});
    rectangle.setFillColor(sf::Color(100, 200, 50));
    rectangle.setPosition({20.f, 20.f});

    sf::CircleShape regular_octagon(80.f, 8);
    regular_octagon.setFillColor(sf::Color(150, 100, 250));
    regular_octagon.setPosition({400.f, 300.f});

    sf::ConvexShape convex(5);

    convex.setPoint(0, {400.f, 500.f});
    convex.setPoint(1, {450.f, 510.f});
    convex.setPoint(2, {460.f, 530.f});
    convex.setPoint(3, {430.f, 540.f});
    convex.setPoint(4, {390.f, 520.f});

    sf::Font font;
    if (!font.openFromFile("fonts/arial.ttf")) {
        std::cerr << "Error: Could not load font from folder." << std::endl;
        return -1;
    }

    sf::Text text(font);

    text.setPosition({400.f, 300.f});
    text.setString("Hello World!");
    text.setCharacterSize(50.f);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Underlined);
    text.setScale({-1.f, 1.2f});


    // clock stuff
    const sf::Time timePerTick = sf::seconds(1.f / 60.f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    // logic stuff
    bool directionLeft = false;

    // main loop
    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= timePerTick) {
            timeSinceLastUpdate -= timePerTick;

            // events
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window.close();
                    }
                }
            }

            // logic updates
            if (cool_circle.getPosition().x > 600) {
                directionLeft = true;
            } else if (cool_circle.getPosition().x < 100) {
                directionLeft = false;
            }
            if (directionLeft) {
                cool_circle.move({-1.f, 0.f});
            } else {
                cool_circle.move({1.f, 0.f});
            }
        }

        // rendering is OUTSIDE of the logic loop
        window.clear(sf::Color::Black);
        // window.draw(sprite);
        window.draw(circle);
        window.draw(cool_circle);
        window.draw(rectangle);
        window.draw(regular_octagon);
        window.draw(convex);
        window.draw(text);
        window.display();
    }
    return 0;
}