#include "engine.hpp"

void Engine::draw() {
  window.clear(Color::Black);

  // Draw snake sections
  for (auto & s : snake) {
    window.draw(s.getShape());
  }

  window.display();
}
