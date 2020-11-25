#ifndef NIBBLES_SNAKESECTION_HPP
#define NIBBLES_SNAKESECTION_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class SnakeSection {
private:
    RectangleShape section;

public:
    explicit SnakeSection(Vector2f startPosition);

    void setPosition(Vector2f newPosition);

    RectangleShape getShape();

};


#endif //NIBBLES_SNAKESECTION_HPP
