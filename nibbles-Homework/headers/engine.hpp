#ifndef NIBBLES_ENGINE_HPP
#define NIBBLES_ENGINE_HPP

#include "snakesection.hpp"
#include "apple.hpp"
#include "wall.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <fstream>
#include <stdlib.h>

using namespace sf;
using namespace std;

class Engine {
private:
    // Window
    Vector2f resolution;
    RenderWindow window;
    const unsigned int FPS = 60;

    vector<SnakeSection> snake;

    int snakeDirection;
    deque<int> directionQueue; // queue for direction key presses
    int speed;
    int sectionsToAdd; // how many sections to add to the snake
    int applesEatenThisLevel;
    int applesEatenTotal;
    const unsigned int applesForNextLevel = 3;
    unsigned long long int score;

    Apple apple;

    vector<Wall> wallSections;
    int currentLevel;
    int maxLevels;
    vector<String> levels;

    Font mainFont;
    Text titleText;
    Text applesEatenText;
    Text currentLevelText;
    Text scoreText;
    Text gameOverText;
    Text pressEnterText;

    Time timeSinceLastMove;

    int currentGameState;
    int lastGameState;  // For storing the last state the game was in when pausing.

public:
    enum Direction { UP, RIGHT, DOWN, LEFT };
    enum GameState { RUNNING, PAUSED, GAMEOVER };
    Engine();

    void input();
    void addDirection(int newDirection);
    void update();
    void draw();

    static void setupText(Text *textItem, const Font &font, const String &value, int size, Color colour);

    void newSnake();
    void addSnakeSection();

    void moveApple();
    void checkLevelFiles();
    void loadLevel(int levelNumber);

    void beginNextLevel();
    void startTheGame();

    void togglePause();

    // The main loop will be in the run function
    void run();

};


#endif //NIBBLES_ENGINE_HPP
