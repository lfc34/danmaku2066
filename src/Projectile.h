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

// the inherance in this file is called "Hierarchical inheritance"
class Projectile {
private:
	float Speed; 
	sf::CircleShape ProjShape;
  bool flewAway;
	
public:
  Projectile() = default;
	virtual void update() = 0;
  virtual const sf::Rect<float> getProjBounds() = 0;
  virtual const sf::CircleShape& getShape() = 0;
  virtual bool isFlewAway() = 0;
};

class Bullet : public Projectile {
private:
	// -15 because it moves up
	const float Speed = -15.0f;
	sf::CircleShape ProjShape;
  bool flewAway;

public: 	
	Bullet(sf::Vector2f ShootPos);
  bool isFlewAway() override;
  const sf::Rect<float> getProjBounds() override;
	void update() override;	
  const sf::CircleShape& getShape() override;
}; 

class Pebble : public Projectile {
	private: 
		const float Speed = 5.0f;
		sf::CircleShape ProjShape;
    bool flewAway;

	public: 
		Pebble(sf::Vector2f ShootPos);
    bool isFlewAway() override;
    const sf::Rect<float> getProjBounds() override;
    const sf::CircleShape& getShape() override;
		void update() override;
};

#endif