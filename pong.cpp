#include <SFML/Graphics.hpp>
#include <optional>
#include <random>
#include <cmath>

int main() {
    // randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distAngle(-45.0, 45.0);

    // window
    const sf::Vector2f windowSize = {500.f, 300.f};
    sf::RenderWindow window(sf::VideoMode(static_cast<sf::Vector2u>(windowSize)), "Pong");

    // visuals
    const sf::Vector2f ballSize = {10.f, 10.f};
    sf::RectangleShape ball(ballSize);
    ball.setPosition({245.f, 145.f}); // center of screen
    ball.setFillColor(sf::Color(255, 255, 255));

    const sf::Vector2f paddleSize = {10.f, 50.f}; 
    sf::RectangleShape leftPaddle(paddleSize);
    leftPaddle.setPosition({15.f, 125.f}); // center: 20, 150
    leftPaddle.setFillColor(sf::Color(255, 255, 255));
 
    sf::RectangleShape rightPaddle(paddleSize);
    rightPaddle.setPosition({475.f, 125.f}); // center: 480, 150
    rightPaddle.setFillColor(sf::Color(255, 255, 255));

    // physics
    sf::Vector2f ballVelocity = {200, 0}; // velocity is stored in cartesian

    sf::Vector2f ballPos = ball.getPosition();
    sf::Vector2f leftPaddlePos = leftPaddle.getPosition();
    sf::Vector2f rightPaddlePos = rightPaddle.getPosition();

    bool ballRight = true;


    // framerate
    const sf::Time dt = sf::seconds(1.f / 60.f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    // main loop
    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= dt) {
            timeSinceLastUpdate -= dt;

            // event loop
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();
            }

            // keyboard and logic
            // uses Scan instead of Key so that it works on non-qwerty layouts
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
                if (rightPaddlePos.y >= 5) { // greater than because down is positive
                    rightPaddlePos.y -= 5;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
                if (rightPaddlePos.y <= windowSize.y - paddleSize.y - 5) {
                    rightPaddlePos.y += 5;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
                 if (leftPaddlePos.y >= 5) { // greater than because down is positive
                    leftPaddlePos.y -= 5;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
                if (leftPaddlePos.y <= windowSize.y - paddleSize.y - 5) {
                    leftPaddlePos.y += 5;
                }
            }
            leftPaddle.setPosition(leftPaddlePos);
            rightPaddle.setPosition(rightPaddlePos);
            
            // ball logic
            ballPos += ballVelocity * dt.asSeconds(); // must convert time to seconds to scale velocity correctly
            ball.setPosition(ballPos);

            // collisions
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            if (const std::optional intersection = leftPaddle.getGlobalBounds().findIntersection(ballBounds)) {
                ballPos.x = leftPaddlePos.x + paddleSize.x;
                ballVelocity.x *= -1;
                ballVelocity *= 1.1f;
            }
            if (const std::optional intersection = rightPaddle.getGlobalBounds().findIntersection(ballBounds)) {
                ballPos.x = rightPaddlePos.x - ballSize.x;
                ballVelocity.x *= -1;
                ballVelocity *= 1.1f;
            }
            if (ballPos.y < 0 || ballPos.y > windowSize.y - ballSize.y) {
                ballVelocity.y *= -1;
            }
            
            // going off the screen
            if (ballPos.x < 0 || ballPos.x > windowSize.x - ballSize.x) {
                ballPos = windowSize / 2.f - ballSize / 2.f;
                sf::Angle randomAngle = sf::degrees(static_cast<float>(distAngle(gen))); // -45 to 45 deg
                ballVelocity = {200 * std::cos(randomAngle.asRadians()), 200 * std::sin(randomAngle.asRadians())};
                if (!ballRight) {
                    ballVelocity *= -1.f; // go left instead
                }
                ballRight = !ballRight;
            }
        }

        window.clear();
        window.draw(ball);
        window.draw(leftPaddle);
        window.draw(rightPaddle);
        window.display();
    }
    return 0;
}