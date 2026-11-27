#include <SFML/Graphics.hpp>
#include <optional>
#include <array>
#include <vector>
#include <deque>
#include <random>
#include <algorithm>

void quad(sf::VertexArray& vertices, sf::FloatRect rect, sf::Color color) {
    sf::Vector2f topLeft(rect.position.x, rect.position.y);
    sf::Vector2f topRight(rect.position.x + rect.size.x, rect.position.y);
    sf::Vector2f bottomLeft(rect.position.x, rect.position.y + rect.size.y);
    sf::Vector2f bottomRight(rect.position.x + rect.size.x, rect.position.y + rect.size.y);

    // tri 1
    vertices.append(sf::Vertex{topLeft, color});
    vertices.append(sf::Vertex{topRight, color});
    vertices.append(sf::Vertex{bottomLeft, color});

    // tri 2
    vertices.append(sf::Vertex{bottomLeft, color});
    vertices.append(sf::Vertex{topRight, color});
    vertices.append(sf::Vertex{bottomRight, color});
}

int main() {
    // random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> appleDropper(0.0, 1.0);

    // window
    constexpr sf::Vector2f windowSize = {300.f, 300.f};
    sf::RenderWindow window(sf::VideoMode(static_cast<sf::Vector2u>(windowSize)), "Snake");

    // visual nums
    constexpr int gridSize = 16; // maybe split this into x and y
    constexpr float cellSize = 16;
    constexpr float distToSideX = (windowSize.x - gridSize * cellSize) / 2.f;
    constexpr float distToSideY = (windowSize.y - gridSize * cellSize) / 2.f;

    // direction
    std::array<int, 2> lastDirection = {1, 0};
    std::array<int, 2> currentDirection = {1, 0};
    bool canChangeDirection = true;

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

    // visual data
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

    constexpr sf::Vector2u boardDimensions = {gridSize, gridSize};

    constexpr sf::Color headColor(75, 230, 14);
    constexpr sf::Color tailColor(63, 199, 10);
    constexpr sf::Color appleColor(217, 75, 50);

    sf::VertexArray snakeCells(sf::PrimitiveType::Triangles);

    quad(snakeCells, sf::FloatRect(
        {distToSideX + snakeHead[0] * cellSize, distToSideY + snakeHead[1] * cellSize},
        {cellSize - 1, cellSize - 1}
    ), headColor); // head
    quad(snakeCells, sf::FloatRect(
        {distToSideX + (snakeHead[0] - 1) * cellSize, distToSideY + snakeHead[1] * cellSize},
        {cellSize - 1, cellSize - 1}
    ), tailColor); // tail
    quad(snakeCells, sf::FloatRect(
        {distToSideX + (snakeHead[0] - 2) * cellSize, distToSideY + snakeHead[1] * cellSize},
        {cellSize - 1, cellSize - 1}
    ), tailColor); // tail

    sf::RectangleShape appleSquare({cellSize - 1.f, cellSize - 1.f});
    appleSquare.setPosition({distToSideX + apple[0] * cellSize, distToSideY + apple[1] * cellSize});
    appleSquare.setFillColor(appleColor);

    // time
    constexpr sf::Time dt = sf::seconds(1.f / 7.f);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        // event loop
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) window.close();
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (canChangeDirection) {
                        if (
                            (keyPressed->scancode == sf::Keyboard::Scancode::Right
                            || keyPressed->scancode == sf::Keyboard::Scancode::D)
                            && (lastDirection[0] != -1)) {
                            currentDirection = {1, 0};
                            canChangeDirection = false;
                        }
                        if (
                            (keyPressed->scancode == sf::Keyboard::Scancode::Up
                            || keyPressed->scancode == sf::Keyboard::Scancode::W)
                            && (lastDirection[1] != 1)) {
                            currentDirection = {0, -1};
                            canChangeDirection = false;
                        }
                        if (
                            (keyPressed->scancode == sf::Keyboard::Scancode::Left
                            || keyPressed->scancode == sf::Keyboard::Scancode::A)
                            && (lastDirection[0] != 1)) {
                            currentDirection = {-1, 0};
                            canChangeDirection = false;
                        }
                        if (
                            (keyPressed->scancode == sf::Keyboard::Scancode::Down
                            || keyPressed->scancode == sf::Keyboard::Scancode::S)
                            && (lastDirection[1] != -1)) {
                            currentDirection = {0, 1};
                            canChangeDirection = false;
                        }
                    }
                }
            }

        while (timeSinceLastUpdate >= dt) {
            timeSinceLastUpdate -= dt;

            canChangeDirection = true;

            lastDirection = currentDirection;

            // move snake
            snake.push_front({snakeHead[0] + currentDirection[0], snakeHead[1] + currentDirection[1]});
            snakeHead = snake.front(); // snake.front() doesn't change from here to the line above on the next move cycle, so no need to set it again there

            if (
                snakeHead[0] < 0
                || snakeHead[0] >= gridSize
                || snakeHead[1] < 0
                || snakeHead[1] >= gridSize
                || std::find(std::next(snake.begin()), snake.end(), snakeHead) != snake.end()) {
                // lose
                window.close();
            }

            nonsnakeBoard.erase(std::remove(nonsnakeBoard.begin(), nonsnakeBoard.end(), snake.front()), nonsnakeBoard.end());

            std::size_t oldSize = snakeCells.getVertexCount();

            if (snake.front() == apple) {
                if (snake.size() >= gridSize * gridSize) {
                    // win
                    window.close(); // temporary
                }
                apple = nonsnakeBoard[static_cast<int>(nonsnakeBoard.size() * appleDropper(gen))]; // random empty tile

                appleSquare.setPosition({distToSideX + apple[0] * cellSize, distToSideY + apple[1] * cellSize});

                snakeCells.resize(snakeCells.getVertexCount() + 6); // add space for another rectangle before shifting
            } else {
                nonsnakeBoard.push_back(snake.back());
                snake.pop_back();

                oldSize -= 6; // so it doesn't set values outside the vector
            }

            for (std::size_t i = oldSize; i > 0; i--) {
                snakeCells[i + 5] = snakeCells[i - 1]; // 6 vertices makes up 2 triangles
            }

            sf::Vector2f topLeft(distToSideX + snakeHead[0] * cellSize, distToSideY + snakeHead[1] * cellSize);
            sf::Vector2f topRight(distToSideX + (snakeHead[0] + 1) * cellSize - 1, distToSideY + snakeHead[1] * cellSize);
            sf::Vector2f bottomLeft(distToSideX + snakeHead[0] * cellSize, distToSideY + (snakeHead[1] + 1) * cellSize - 1);
            sf::Vector2f bottomRight(distToSideX + (snakeHead[0] + 1) * cellSize - 1, distToSideY + (snakeHead[1] + 1) * cellSize - 1);
            
            snakeCells[0] = sf::Vertex{topLeft, headColor};
            snakeCells[1] = sf::Vertex{topRight, headColor};
            snakeCells[2] = sf::Vertex{bottomLeft, headColor};
            snakeCells[3] = sf::Vertex{bottomLeft, headColor};
            snakeCells[4] = sf::Vertex{topRight, headColor};
            snakeCells[5] = sf::Vertex{bottomRight, headColor};

            for (int i = 6; i < 12; i++) {
                snakeCells[i].color = tailColor;
            }

        }

        window.clear();

        window.draw(grid);
        window.draw(snakeCells);
        window.draw(appleSquare);

        window.display();
    }
    return 0;
}