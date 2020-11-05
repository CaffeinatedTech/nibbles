#include "engine.hpp"

void Engine::input() {
  Event event{};

  while (window.pollEvent(event)) {
    // Window closed
    if (event.type == Event::Closed) {
      window.close();
    }

    // Handle Keyboard Input
    if (event.type == Event::KeyPressed) {
      // Quit
      if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
      }

      // Pause
      if (Keyboard::isKeyPressed(Keyboard::Pause)) {
        togglePause();
      }

      // New Game
      if (currentGameState == GameState::GAMEOVER) {
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
          startTheGame();
        }
      }
    }
  } // END while pollEvent
  // Directions
  if (Keyboard::isKeyPressed(Keyboard::Up)) {
    addDirection(Direction::UP);
  }
  else if (Keyboard::isKeyPressed(Keyboard::Down)) {
    addDirection(Direction::DOWN);
  }
  else if (Keyboard::isKeyPressed(Keyboard::Left)) {
    addDirection(Direction::LEFT);
  }
  else if (Keyboard::isKeyPressed(Keyboard::Right)) {
    addDirection(Direction::RIGHT);
  }
}

void Engine::addDirection(int newDirection) {
  if (directionQueue.empty()) {
    directionQueue.emplace_back(newDirection);
  }
  else {
    if (directionQueue.back() != newDirection) {
      directionQueue.emplace_back(newDirection);
    }
  }
}
