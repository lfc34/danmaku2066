#include "Projectile.h"

Bullet::Bullet(sf::Vector2f ShootPos) {
  ProjShape.setRadius(3.5);
	ProjShape.setPointCount(3);
	ProjShape.setFillColor(sf::Color::Yellow);
	ProjShape.setOrigin(1.75, 1.75); //center
	ProjShape.setPosition(ShootPos);
  flewAway = 0;
  // std::clog << "Bullet spawned\n";
}

void Bullet::update() {
  if (!flewAway)
	  ProjShape.move(0, Speed);

	// check if bullet flew away from screen here
  if (ProjShape.getPosition().y < 0)
    flewAway = true;
}

bool Bullet::isFlewAway() {
  return flewAway;
}

const sf::CircleShape& Bullet::getShape() {
  return ProjShape;
}

sf::Rect<float> Bullet::getProjBounds() {
  return ProjShape.getGlobalBounds();
}

Pebble::Pebble(sf::Vector2f ShootPos) {
	ProjShape.setRadius(4);
	ProjShape.setPointCount(6);
	ProjShape.setFillColor(sf::Color::White);
	ProjShape.setPosition(ShootPos);
	ProjShape.setOrigin(2, 2);
  flewAway = 0;
}

void Pebble::update() {
  if (!flewAway)
    ProjShape.move(0, Speed);
  // check out of screen here
}

bool Pebble::isFlewAway() {
  return flewAway;
}

sf::Rect<float> Pebble::getProjBounds() {
  return ProjShape.getGlobalBounds();
}

const sf::CircleShape& Pebble::getShape() {
  return ProjShape;
}