#ifndef NIBBLES_01_ENGINE_HPP
#define NIBBLES_01_ENGINE_HPP

#include "snakesection.hpp"
#include "apple.hpp"
#include "wall.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <fstream>

using namespace sf;
using namespace std;

class Engine {
private:
    // Window
    Vector2f resolution;
    RenderWindow window;
    const unsigned int FPS = 60;
    static const Time TimePerFrame;

    vector<SnakeSection> snake;

    int snakeDirection;
    deque<int> directionQueue; // queue for direction key presses
    int speed;
    int sectionsToAdd; // how many sections to add to the snake

    Apple apple;

    vector<Wall> wallSections;
    int currentLevel;
    int maxLevels;
    vector<String> levels;

    Time timeSinceLastMove;

    int currentGameState;
    int lastGameState; // For storing the last state the game was in when pausing.

public:
    enum Direction { UP, RIGHT, DOWN, LEFT };
    enum GameState { RUNNING, PAUSED, GAMEOVER };
    Engine();

    void input();
    void addDirection(int newDirection);
    void update();
    void draw();

    void newSnake();
    void addSnakeSection();

    void moveApple();
    void checkLevelFiles();
    void loadLevel(int levelNumber);

    void startTheGame();

    void togglePause();

    // The main loop will be in the run function
    void run();

};


#endif //NIBBLES_01_ENGINE_HPP
