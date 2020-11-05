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
    }
  }
}

