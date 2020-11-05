#include "engine.hpp"

const sf::Time Engine::TimePerFrame = seconds(1.f/60.f);

Engine::Engine() {
  resolution = Vector2f(800, 600);
  window.create(VideoMode(resolution.x, resolution.y), "Nibbles", Style::Default);
  window.setFramerateLimit(FPS);

  newSnake();
}

// This is the initial snake created at the start of each level
void Engine::newSnake() {
  snake.clear();
  snake.emplace_back(Vector2f(100,100));
  snake.emplace_back(Vector2f(80,100));
  snake.emplace_back(Vector2f(60,100));
}

void Engine::addSnakeSection() {
  // Just add the section at the same position as the last section of the snake, it will follow normally
  Vector2f newSectionPosition = snake[snake.size() - 1].getPosition();
  snake.emplace_back(newSectionPosition);
}

void Engine::run() {

  // Main loop - Runs until the window is closed
  while (window.isOpen()) {
    input();
    draw();
  }
}