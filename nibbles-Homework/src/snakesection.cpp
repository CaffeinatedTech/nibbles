#include "snakesection.hpp"

SnakeSection::SnakeSection(Vector2f startPosition) {
  section.setSize(Vector2f(20, 20));
  section.setFillColor(Color::Green);
  section.setPosition(startPosition);
}

void SnakeSection::setPosition(Vector2f newPosition) {
  section.setPosition(newPosition);
}

RectangleShape SnakeSection::getShape() {
  return section;
}
