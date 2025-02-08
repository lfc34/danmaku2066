#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <memory>
#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>

#include "Projectile.h"

// TODO: add more point for more curve line
struct EnemyPathWay {
  sf::Vector2f mid_point;
  sf::Vector2f end_point;
};

class Enemy {
public: 
	virtual void updateEnemy() = 0;
  virtual void enemyShoot() = 0;
  virtual const sf::Sprite& getSprite() = 0;
  virtual void die() = 0;
  virtual unsigned int& getHp() = 0;
  virtual ~Enemy();
};

class MoonStone : public Enemy {
private:
  sf::Texture EnemyTexture;
	sf::Sprite EnemySprite;
	const float FallingSpeed = 0.02f;
	sf::Clock ShootTimer;
  std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> ProjVec; 
	unsigned int hp = 10;
  // This two variables above represent the state of enemy movement
  EnemyPathWay path;
  sf::Vector2f current_direction;

public:
  MoonStone(sf::Vector2f SpawnPos, 
            std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> Pvec,
            EnemyPathWay p);
	void updateEnemy() override;
  void enemyShoot() override;
  const sf::Sprite& getSprite() override;
  unsigned int& getHp() override;
  void die() override;
};

#endif