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

class Enemy {
public: 
	virtual void updateEnemy(const sf::Vector2f& direction) = 0;
  virtual void enemyShoot() = 0;
  virtual const sf::Sprite& getSprite() = 0;
  virtual ~Enemy();
};

class MoonStone : public Enemy {
private:
  sf::Texture EnemyTexture;
	sf::Sprite EnemySprite;
	const float FallingSpeed = 0.02f;
	unsigned int hp = 10;
	sf::Clock ShootTimer;
  std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> ProjVec; 
  enum EnemyState {
    moving_up,
    moving_down
  };

public:
  MoonStone(sf::Vector2f SpawnPos, 
            std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> Pvec);
	void updateEnemy(const sf::Vector2f& direction) override;
  void enemyShoot() override;
  const sf::Sprite& getSprite() override;
};

#endif