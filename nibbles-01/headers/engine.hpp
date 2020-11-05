#ifndef NIBBLES_01_ENGINE_HPP
#define NIBBLES_01_ENGINE_HPP

#include "snakesection.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

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

public:
    Engine();

    void input();

    void draw();

    void newSnake();
    void addSnakeSection();

    // The main loop will be in the run function
    void run();

};


#endif //NIBBLES_01_ENGINE_HPP
