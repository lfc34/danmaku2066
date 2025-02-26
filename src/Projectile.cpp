#include "Projectile.h"

bool Projectile::isFlewAway() {
  return (ProjShape.getPosition().y > 600);
}

const sf::Rect<float> Projectile::getProjBounds() {
  return ProjShape.getGlobalBounds();
}

const sf::CircleShape& Projectile::getShape() {
  return ProjShape;
}

Bullet::Bullet(sf::Vector2f ShootPos) {
  ProjShape.setRadius(3.0f);
	ProjShape.setPointCount(3);
	ProjShape.setFillColor(sf::Color::Yellow);
	ProjShape.setOrigin(1.75, 1.75); //center
	ProjShape.setPosition(ShootPos);
}

void Bullet::update(const float& delta) {
  if (!(isFlewAway()))
	  ProjShape.move(0, Speed * delta);
}

Pebble::Pebble(sf::Vector2f ShootPos) {
	ProjShape.setRadius(4);
	ProjShape.setPointCount(6);
	ProjShape.setFillColor(sf::Color::White);
	ProjShape.setPosition(ShootPos);
	ProjShape.setOrigin(2, 2);
}

void Pebble::update(const float& delta) {
  if (!(isFlewAway()))
    ProjShape.move(0, Speed * delta);
}

Fireball::Fireball(sf::Vector2f ShootPos, const float& x_direction) {
  ProjShape.setRadius(12);
  ProjShape.setFillColor(sf::Color::Yellow);
  ProjShape.setPosition(ShootPos);
  ProjShape.setOrigin(6, 6);
  x_offset = x_direction;
}

void Fireball::update(const float& delta) {
  if (!(isFlewAway()))
    ProjShape.move(x_offset * delta, Speed * delta);
}