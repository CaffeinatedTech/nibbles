#include "engine.hpp"

void Engine::update() {
  // Update snake positions
  if (timeSinceLastMove.asSeconds() >= seconds(1.f / float(speed)).asSeconds()) {
    Rect lastSectionRect = snake.front().getShape().getGlobalBounds();
    Vector2f thisSectionPosition = Vector2f(lastSectionRect.left, lastSectionRect.top);
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

    // Update Score
    score += snake.size() + (applesEatenTotal + 1);
    scoreText.setString(to_string(score));
    FloatRect scoreTextBounds = scoreText.getLocalBounds();
    scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, -9));

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
      lastSectionRect = snake[s].getShape().getGlobalBounds();
      thisSectionPosition = Vector2f(lastSectionRect.left, lastSectionRect.top);
      snake[s].setPosition(lastSectionPosition);
      lastSectionPosition = thisSectionPosition;
    }

    // Collision detection - Apple
    if (snake[0].getShape().getGlobalBounds().intersects(apple.getSprite().getGlobalBounds())) {
      // We hit the apple, add more sections to the snake, increase speed and move the apple
// TODO - Check if its time for the next level
      applesEatenThisLevel += 1;
      applesEatenTotal += 1;
      applesEatenText.setString("apples " + to_string(applesEatenTotal));
      FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
      applesEatenText.setPosition(
          Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));

      bool beginningNewLevel = false;
      if (applesEatenThisLevel >= applesForNextLevel) {
        // Begin the next level if there are more,
        // otherwise, just stay on the last level and keep getting harder until we die.
        if (currentLevel < maxLevels) {
          beginningNewLevel = true;
          beginNextLevel();
        }
      }
      if (!beginningNewLevel) {
        sectionsToAdd += 4;
        // Removed the speed increase here for homework
        // speed++;
        moveApple();
      }
    }

    // Collision detection - Snake Body
    for (int s = 1; s < snake.size(); s++) {
      if (snake[0].getShape().getGlobalBounds().intersects(snake[s].getShape().getGlobalBounds())) {
        // Game Over
        currentGameState = GameState::GAMEOVER;
      }
    }

    // Collision detection - Wall
    for (auto & w : wallSections) {
      if (snake[0].getShape().getGlobalBounds().intersects(w.getShape().getGlobalBounds())) {
        // Game Over
        currentGameState = GameState::GAMEOVER;
      }
    }

    // Reset the last move timer.
    timeSinceLastMove = Time::Zero;
  } // END update snake positions
}

