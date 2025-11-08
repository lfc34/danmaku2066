/// PORTED TO SFML3
#include "Projectile.hpp"

void accel_proj(float& speed, float accel) {
  speed += accel;
}

void slow_proj(float &speed, float slowdown) {
  if (speed > 100.0f)
    speed -= slowdown;
}

bool Projectile::isFlewAway() {
  return (ProjShape.getPosition().y > 600 || ProjShape.getPosition().y < -5);
}

const sf::Rect<float> Projectile::getProjBounds() {
  return ProjShape.getGlobalBounds();
}

const sf::CircleShape& Projectile::getShape() {
  return ProjShape;
}

Bullet::Bullet(sf::Vector2f ShootPos) {
  ProjShape.setRadius(4.2f);
	ProjShape.setPointCount(3);
	ProjShape.setFillColor(sf::Color::Yellow);
	ProjShape.setOrigin({1.75, 1.75}); //center
	ProjShape.setPosition(ShootPos);
}

void Bullet::update(const float& delta) {
  if (!(isFlewAway())) 
	  ProjShape.move({0, Speed * delta});
}

Pebble::Pebble(sf::Vector2f ShootPos) {
	ProjShape.setRadius(4);
	ProjShape.setPointCount(6);
	ProjShape.setFillColor(sf::Color::White);
	ProjShape.setPosition(ShootPos);
	ProjShape.setOrigin({2, 2});
}

void Pebble::update(const float& delta) {
  if (!(isFlewAway())) {
    ProjShape.move({0, Speed * delta});
    accel_proj(Speed, 10.0f);
  }
}

Fireball::Fireball(sf::Vector2f ShootPos, const float& x_direction) {
  ProjShape.setRadius(12);
  ProjShape.setFillColor(sf::Color::Yellow);
  ProjShape.setOutlineThickness(2);
  ProjShape.setOutlineColor(sf::Color::Red);
  ProjShape.setPosition(ShootPos);
  ProjShape.setOrigin({6, 6});
  x_offset = x_direction;
}

void Fireball::update(const float& delta) {
  if (!(isFlewAway())) {
    ProjShape.move({x_offset * delta, Speed * delta});
    slow_proj(Speed, 5.0f);
    slow_proj(x_offset, 1.0f);
  }
}

Flameshard::Flameshard(sf::Vector2f ShootPos) {
  ProjShape.setRadius(5);
  ProjShape.setFillColor(sf::Color::Yellow);
  ProjShape.setOutlineThickness(2);
  ProjShape.setOutlineColor(sf::Color::Red);
  ProjShape.setPosition(ShootPos);
  ProjShape.setOrigin({1.5, -3});
}

void Flameshard::update(const float& delta) {
  if (!(isFlewAway())) {
    ProjShape.move({0, delta * Speed});
    ProjShape.rotate(sf::degrees(10));
  }
}