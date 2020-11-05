#ifndef NIBBLES_01_ENGINE_HPP
#define NIBBLES_01_ENGINE_HPP

#include "snakesection.hpp"
#include "apple.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>

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

    Time timeSinceLastMove;

public:
    enum Direction { UP, RIGHT, DOWN, LEFT };
    Engine();

    void input();
    void addDirection(int newDirection);
    void update();
    void draw();

    void newSnake();
    void addSnakeSection();

    void moveApple();

    // The main loop will be in the run function
    void run();

};


#endif //NIBBLES_01_ENGINE_HPP
