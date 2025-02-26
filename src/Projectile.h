/// \file
/// This file contains projectile class and it's methods
/// like update projectile (allow them to move on screen)

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <iostream>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Projectile {
protected:
  sf::CircleShape ProjShape;
public:
  Projectile() = default;
  virtual void update(const float& delta) = 0;

  const sf::Rect<float> getProjBounds();
  const sf::CircleShape& getShape();
  bool isFlewAway();
};

class Bullet : public Projectile {
private:
	const float Speed = -900.0f; // speed in px/sec

public: 	
	Bullet(sf::Vector2f ShootPos);
	void update(const float& delta) override;	
}; 

class Pebble : public Projectile {
private: 
  const float Speed = 300.0f;

public: 
  Pebble(sf::Vector2f ShootPos);
  void update(const float& delta) override;
};

class Fireball : public Projectile {
private:
  const float Speed = 170.0f;
  float x_offset {};
public:
  Fireball(sf::Vector2f ShootPos, const float& x_direction);
  void update(const float& delta) override;
};

#endif