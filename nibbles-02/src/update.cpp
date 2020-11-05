#include "engine.hpp"

void Engine::update() {
  // Update snake positions
  if (timeSinceLastMove.asSeconds() >= seconds(1.f / float(speed)).asSeconds()) {
    Vector2f thisSectionPosition = snake[0].getPosition();
    Vector2f lastSectionPosition = thisSectionPosition;

    // Is there anything in the input queue?
    if (!directionQueue.empty()) {
      // Make sure we don't reverse direction
      switch (snakeDirection) {
        case Direction::UP:
          if (directionQueue.front() != Direction::DOWN) {
            snakeDirection = directionQueue.front();
          }
          break;
        case Direction::DOWN:
          if (directionQueue.front() != Direction::UP) {
            snakeDirection = directionQueue.front();
          }
          break;
        case Direction::LEFT:
          if (directionQueue.front() != Direction::RIGHT) {
            snakeDirection = directionQueue.front();
          }
          break;
        case Direction::RIGHT:
          if (directionQueue.front() != Direction::LEFT) {
            snakeDirection = directionQueue.front();
          }
          break;
      }
      directionQueue.pop_front();
    }

    // Do we need to grow the snake
    if (sectionsToAdd) {
      addSnakeSection();
      sectionsToAdd--;
    }

    // Update the snake's head position
    switch (snakeDirection) {
      case Direction::RIGHT:
        snake[0].setPosition(Vector2f(thisSectionPosition.x + 20, thisSectionPosition.y));
        break;
      case Direction::DOWN:
        snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y + 20));
        break;
      case Direction::LEFT:
        snake[0].setPosition(Vector2f(thisSectionPosition.x - 20, thisSectionPosition.y));
        break;
      case Direction::UP:
        snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y - 20));
        break;
    }

    // Update the snake tail positions
    for (int s = 1; s < snake.size(); s++) {
      thisSectionPosition = snake[s].getPosition();
      snake[s].setPosition(lastSectionPosition);
      lastSectionPosition = thisSectionPosition;
    }

    // update snake sections
    for (auto & s : snake) {
      s.update();
    }

    // Collision detection - Apple
    if (snake[0].getShape().getGlobalBounds().intersects(apple.getSprite().getGlobalBounds())) {
      // We hit the apple, add more sections to the snake, increase speed and move the apple
// TODO - increment score, apples eaten, add snake sections and check if its time for the next level

      sectionsToAdd += 4;
      speed++;
      moveApple();
    }

    // Reset the last move timer.
    timeSinceLastMove = Time::Zero;
  } // END update snake positions
}

