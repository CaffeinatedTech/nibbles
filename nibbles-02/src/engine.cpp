#include "engine.hpp"

const sf::Time Engine::TimePerFrame = seconds(1.f/60.f);

Engine::Engine() {
  resolution = Vector2f(800, 600);
  window.create(VideoMode(resolution.x, resolution.y), "Nibbles", Style::Default);
  window.setFramerateLimit(FPS);

  speed = 2;
  snakeDirection = Direction::RIGHT;

  timeSinceLastMove = Time::Zero;

  sectionsToAdd = 0;
  newSnake();
  moveApple();
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

void Engine::moveApple() {
  // Find a location to place the apple
  // Must not be inside the snake, or on a wall

  // Divide the field into sections the size of the apple - remove 2 to exclude the exterior walls
  Vector2f appleResolution = Vector2f(resolution.x / 20 -2, resolution.y / 20 - 2);
  Vector2f newAppleLocation;
  bool badLocation = false;
  srand(time(nullptr));
  // Loop until we find a valid location
  do {
    badLocation = false;
    // Generate a random location
    newAppleLocation.x = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)appleResolution.x)) * 20;
    newAppleLocation.y = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)appleResolution.y)) * 20;

    // Check if it is in the snake
    for (auto &s : snake) {
      if (s.getShape().getGlobalBounds().intersects(Rect<float>(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
        badLocation = true;
        break;
      }
    }

// TODO - Add check for apple on wall sections

  } while (badLocation);
  apple.setPosition(newAppleLocation);
}

void Engine::run() {
  Clock clock;

  // Main loop - Runs until the window is closed
  while (window.isOpen()) {
    Time dt = clock.restart();
    timeSinceLastMove += dt;

    input();
    update();
    draw();
  }
}