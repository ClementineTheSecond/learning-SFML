#include <SFML/Graphics.hpp>
#include <optional>
#include <array>
#include <vector>
#include <deque>
#include <random>
#include <algorithm>

int main() {
    // random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> appleDropper(0.0, 1.0);

    // window
    constexpr sf::Vector2f windowSize = {300.f, 300.f};
    sf::RenderWindow window(sf::VideoMode(static_cast<sf::Vector2u>(windowSize)), "Snake");

    // grid
    constexpr int gridSize = 16; // maybe split this into x and y
    constexpr float cellSize = 16;
    constexpr float distToSideX = (windowSize.x - gridSize * cellSize) / 2.f;
    constexpr float distToSideY = (windowSize.y - gridSize * cellSize) / 2.f;

    sf::VertexArray grid(sf::PrimitiveType::Lines);

    for (int i = 0; i <= gridSize; i++) {
        float pos = i * cellSize;
        
        // vertical line
        grid.append({{pos + distToSideX, distToSideY}, sf::Color::White});
        grid.append({{pos + distToSideX, gridSize * cellSize + distToSideY}, sf::Color::White});
        
        // horizontal line
        grid.append({{distToSideX, pos + distToSideY}, sf::Color::White});
        grid.append({{gridSize * cellSize + distToSideX, pos + distToSideY}, sf::Color::White});
    }

    constexpr sf::Vector2i boardDimensions = {gridSize, gridSize};

    // direction
    std::array<int, 2> lastDirection = {1, 0};
    std::array<int, 2> currentDirection = {1, 0};

    // snake
    std::deque<std::array<int, 2>> snake = {{4, 8}, {3, 8}, {2, 8}}; // uses a deque since frequent front insertion

    std::array<int, 2> snakeHead = {4, 8};

    std::array<int, 2> apple = {12, 8};

    std::array<int, 2> target;

    std::vector<std::array<int, 2>> nonsnakeBoard;

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            target = {i, j};
            if (std::find(snake.begin(), snake.end(), target) == snake.end()) {
                nonsnakeBoard.push_back({i, j});
            }
        }
    }

    // time
    constexpr sf::Time dt = sf::seconds(1.f / 5.f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate >= dt) {
            timeSinceLastUpdate -= dt;

            // event loop
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();
            }

            currentDirection = lastDirection;

            // keyboard presses
            if (
                (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
                && (lastDirection[0] != -1)) {
                currentDirection = {1, 0};
            }
            if (
                (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
                && (lastDirection[1] != 1)) {
                currentDirection = {0, -1};
            }
            if (
                (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
                && (lastDirection[0] != 1)) {
                currentDirection = {-1, 0};
            }
            if (
                (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
                && (lastDirection[1] != -1)) {
                currentDirection = {0, 1};
            }

            // move snake
            snake.push_front({snakeHead[0] + currentDirection[0], snakeHead[1] + currentDirection[1]});
            snakeHead = snake.front(); // snake.front() doesn't change from here to the line above on the next move cycle, so no need to set it again there

            nonsnakeBoard.erase(std::remove(nonsnakeBoard.begin(), nonsnakeBoard.end(), snake.front()), nonsnakeBoard.end());

            if (snake.front() == apple) {
                if (snake.size() >= gridSize * gridSize) {
                    // win
                }
                apple = nonsnakeBoard[static_cast<int>(nonsnakeBoard.size() * appleDropper(gen))]; // random empty tile
            } else {
                nonsnakeBoard.push_back(snake.back());
                snake.pop_back();
            }
        }

        window.clear();
        window.draw(grid);

        window.display();
    }
    return 0;
}