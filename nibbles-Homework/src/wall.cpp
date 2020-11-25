#include "wall.hpp"

Wall::Wall(Vector2f position, Vector2f size) {
  wallShape.setSize(size);
  wallShape.setFillColor(Color::White);
  wallShape.setPosition(position);
}

RectangleShape Wall::getShape() {
  return wallShape;
}