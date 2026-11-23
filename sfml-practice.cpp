#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Window Practice");
    window.setPosition({20, 20});


    sf::Texture texture;
    if (!texture.loadFromFile("assets/madeline.jpg")) {
        std::cerr << "Error: Could not load image from folder." << std::endl;
        return -1;
    }
    // sf::Sprite sprite(texture);

    sf::CircleShape circle(50);
    circle.setPosition({400, 20});
    circle.setFillColor(sf::Color(100, 250, 50));
    
    circle.setOutlineThickness(5);
    circle.setOutlineColor(sf::Color(250, 150, 100));

    sf::CircleShape cool_circle(50);
    cool_circle.setTexture(&texture);
    cool_circle.setTextureRect(sf::IntRect({50, 50}, {100, 100}));
    cool_circle.setPosition({200, 200});

    sf::RectangleShape rectangle({120, 50});
    rectangle.setFillColor(sf::Color(100, 200, 50));
    rectangle.setPosition({20, 20});

    sf::CircleShape regular_octagon(80, 8);
    regular_octagon.setFillColor(sf::Color(150, 100, 250));
    regular_octagon.setPosition({400, 200});

    sf::ConvexShape convex(5);

    convex.setPoint(0, {400, 500});
    convex.setPoint(1, {450, 510});
    convex.setPoint(2, {460, 530});
    convex.setPoint(3, {430, 540});
    convex.setPoint(4, {390, 520});

    sf::Font font;
    if (!font.openFromFile("fonts/arial.ttf")) {
        std::cerr << "Error: Could not load font from folder." << std::endl;
        return -1;
    }

    sf::Text text(font);

    text.setPosition({200, 300});

    text.setString("Hello World!");

    text.setCharacterSize(50);

    text.setFillColor(sf::Color::White);

    text.setStyle(sf::Text::Underlined);

    while (window.isOpen()) {
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