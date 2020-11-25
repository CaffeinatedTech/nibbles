#include "engine.hpp"

Engine::Engine() {
  resolution = Vector2f(800, 600);
  window.create(VideoMode(resolution.x, resolution.y), "Nibbles", Style::Default);
  window.setFramerateLimit(FPS);
  maxLevels = 0;
  checkLevelFiles();

  startTheGame();

  mainFont.loadFromFile("assets/fonts/slant_regular.ttf");
  setupText(&titleText, mainFont, "NIBBLES", 28, Color::Blue);
  FloatRect titleTextBounds = titleText.getLocalBounds();
  titleText.setPosition(Vector2f(resolution.x / 2 - titleTextBounds.width / 2, -9));

  setupText(&currentLevelText, mainFont, "level 1", 28, Color::Blue);
  currentLevelText.setPosition(Vector2f(15, -9));
  FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();

  setupText(&applesEatenText, mainFont, "apples 0", 28, Color::Blue);
  applesEatenText.setPosition(
      Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));

  setupText(&scoreText, mainFont, to_string(score), 28, Color::Blue);
  FloatRect scoreTextBounds = scoreText.getLocalBounds();
  scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, -9));

  setupText(&gameOverText, mainFont, "GAME OVER", 72, Color::Yellow);
  FloatRect gameOverTextBounds = gameOverText.getLocalBounds();
  gameOverText.setPosition(Vector2f(resolution.x / 2 - gameOverTextBounds.width / 2, 100 ));
  gameOverText.setOutlineColor(Color::Black);
  gameOverText.setOutlineThickness(2);

  setupText(&pressEnterText, mainFont, "Press ENTER to try again", 38, Color::Green);
  FloatRect pressEnterTextBounds = pressEnterText.getLocalBounds();
  pressEnterText.setPosition(Vector2f(resolution.x / 2 - pressEnterTextBounds.width / 2, 200));
  pressEnterText.setOutlineColor(Color::Black);
  pressEnterText.setOutlineThickness(2);
}

void Engine::startTheGame() {
  score = 0;
  speed = 4;
  snakeDirection = Direction::RIGHT;
  timeSinceLastMove = Time::Zero;
  sectionsToAdd = 0;
  directionQueue.clear();
  wallSections.clear();
  applesEatenThisLevel = 0;
  applesEatenTotal = 0;
  currentLevel = 1;
  loadLevel(currentLevel);
  //newSnake();
  moveApple();
  currentGameState = GameState::RUNNING;
  lastGameState = currentGameState;
  currentLevelText.setString("level " + to_string(currentLevel));
  applesEatenText.setString("apples " + to_string(applesEatenTotal));
  FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
  applesEatenText.setPosition(
      Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));
  scoreText.setString(to_string(score));
  FloatRect scoreTextBounds = scoreText.getLocalBounds();
  scoreText.setPosition(Vector2f(resolution.x - scoreTextBounds.width - 15, -9));
}

/**
 * Increment the level number, load the next level and reset the snake
 */
void Engine::beginNextLevel() {
  currentLevel += 1;
  wallSections.clear();
  directionQueue.clear();
  speed += 2;
  snakeDirection = Direction::RIGHT;
  sectionsToAdd = 0;
  applesEatenThisLevel = 0;

  loadLevel(currentLevel);
  //newSnake();
  moveApple();
  currentLevelText.setString("level " + to_string(currentLevel));
  FloatRect currentLevelTextBounds = currentLevelText.getGlobalBounds();
  applesEatenText.setPosition(
      Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -9));
}

// This is the initial snake created at the start of each level
void Engine::newSnake() {
  snake.clear();
  snake.emplace_back(Vector2f(100, 100));
  snake.emplace_back(Vector2f(80, 100));
  snake.emplace_back(Vector2f(60, 100));
}

void Engine::addSnakeSection() {
  // Just add the section at the same position as the last section of the snake, it will follow normally
  Rect lastSectionRect = snake.back().getShape().getGlobalBounds();
  Vector2f newSectionPosition = Vector2f(lastSectionRect.left, lastSectionRect.top);
  snake.emplace_back(newSectionPosition);
}

void Engine::moveApple() {
  // Find a location to place the apple
  // Must not be inside the snake, or a wall

  // Divide the field into sections the size of the apple - remove 2 to exclude the exterior walls
  Vector2f appleResolution = Vector2f(resolution.x / 20 - 2, resolution.y / 20 - 2);
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
    for (auto & s : snake) {
      if (s.getShape().getGlobalBounds()
      .intersects(Rect<float>(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
        badLocation = true;
        break;
      }
    }

    // Check if it is in the walls
    for (auto & w : wallSections) {
      if (w.getShape().getGlobalBounds()
      .intersects(Rect<float>(newAppleLocation.x, newAppleLocation.y, 20, 20))) {
        badLocation = true;
        break;
      }
    }

  } while (badLocation);
  apple.setPosition(newAppleLocation);
}

void Engine::togglePause() {
  if (currentGameState == GameState::RUNNING) {
    lastGameState = currentGameState;
    currentGameState = GameState::PAUSED;
  }
  else if (currentGameState == GameState::PAUSED) {
    currentGameState = lastGameState;
  }
}

void Engine::setupText(Text *textItem, const Font &font, const String &value, int size, Color colour) {
  textItem->setFont(font);
  textItem->setString(value);
  textItem->setCharacterSize(size);
  textItem->setFillColor(colour);
}

/**
 * Check the levels manifest file and make sure that we can open each level file.
 * Add good level file names to the 'levels' vector and increment the maxLevels.
 */
void Engine::checkLevelFiles() {
  // Load the levels manifest file
  ifstream levelsManifest ("assets/levels/levels.txt");
  ifstream testFile;
  for (string manifestLine; getline(levelsManifest, manifestLine);) {
    // Check that we can open the level file
    testFile.open("assets/levels/" + manifestLine);
    if (testFile.is_open()) {
      // The level file opens, lets add it to the list of available levels
      levels.emplace_back("assets/levels/" + manifestLine);
      testFile.close();
      maxLevels ++;
    }
  }
}

/**
 * Load a level from a file and check for 'x' characters to add wall sectinos
 * @param levelNumber int - the number of the level to load.
 */
void Engine::loadLevel(int levelNumber) {
  string levelFile = levels[levelNumber - 1];
  ifstream level (levelFile);
  string line;
  snake.clear();
  Vector2f newHead;
  vector<Vector2f> newSnakeBody;
  if (level.is_open()) {
    for (int y = 0; y < 30; y++) {
      getline(level, line);
      for (int x = 0; x < 40; x++) {
        if (line[x] == 'x') {
          wallSections.emplace_back(Wall(Vector2f(x * 20, y * 20), Vector2f(20, 20)));
        }
        else if (line[x] == 'R') {
          snakeDirection = Direction::RIGHT;
        }
        else if (line[x] == 'D') {
          snakeDirection = Direction::DOWN;
        }
        else if (line[x] == 'L') {
          snakeDirection = Direction::LEFT;
        }
        else if (line[x] == 'U') {
          snakeDirection = Direction::UP;
        }
        else if (line[x] == 'h') {
          newHead = Vector2f(x * 20, y * 20);
        }
        else if (line[x] == 't') {
          newSnakeBody.emplace_back(Vector2f(x, y));
        }
      }
    }
  }
  level.close();
  // Build the new snake
  bool addThisSection = false;
  snake.emplace_back(newHead);
  while (!newSnakeBody.empty()) {
    for (int i = 0; i < newSnakeBody.size(); i++) {
      addThisSection = false;
      // Check if this sections is beside last section
      if (abs(newSnakeBody[i].x - snake.back().getShape().getGlobalBounds().left / 20) == 1) {
        if (newSnakeBody[i].y == snake.back().getShape().getGlobalBounds().top / 20) {
          addThisSection = true;
        }
      }
      // Check if this section is directly above or below the last section
      else if (abs(newSnakeBody[i].y - snake.back().getShape().getGlobalBounds().top / 20) == 1) {
        if (newSnakeBody[i].x == snake.back().getShape().getGlobalBounds().left / 20) {
          addThisSection = true;
        }
      }
      if (addThisSection) {
        snake.emplace_back(Vector2f(newSnakeBody[i].x * 20, newSnakeBody[i].y * 20));
        newSnakeBody.erase(newSnakeBody.begin()+i);
      }
    }
  }
}

void Engine::run() {
  Clock clock;

  // Main loop - Runs until the window is closed
  while (window.isOpen()) {
    Time dt = clock.restart();

    if (currentGameState == GameState::PAUSED || currentGameState == GameState::GAMEOVER) {
      // If we are paused, then check for input (so we can un-pause), then just continue to the next loop
      input();

      // Draw the gameover screen
      if (currentGameState == GameState::GAMEOVER) {
        draw();
      }

      sleep(milliseconds(2)); // sleep so we don't peg the CPU
      continue;
    }

    timeSinceLastMove += dt;

    input();
    update();
    draw();
  }
}
