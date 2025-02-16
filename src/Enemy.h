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
  sf::Vector2f spawn_pos {};
  sf::Vector2f mid_point {};
  sf::Vector2f end_point {};
};

class Enemy {
public: 
  bool is_dead = false;
	virtual void updateEnemy() = 0;
  virtual void enemyShoot() = 0;
  virtual sf::Rect<float> getBounds() = 0;
  virtual const sf::Sprite& getSprite() = 0;
  virtual unsigned int& getHp() = 0;
  virtual ~Enemy();
};

class MoonStone : public Enemy {
private:
  sf::Texture EnemyTexture;
	sf::Sprite EnemySprite;
	const float FallingSpeed = 0.02f;
	sf::Clock ShootTimer;
  std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> player_pr_vec; 
  std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> enemy_pr_vec;
	unsigned int hp = 10;

  // This two variables above represent the state of enemy movement
  EnemyPathWay path;
  sf::Vector2f current_direction;

public:
  MoonStone(sf::Vector2f SpawnPos, 
            std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> Pvec,
            std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> OwnVec,
            EnemyPathWay p);
	void updateEnemy() override;
  void enemyShoot() override;
  sf::Rect<float> getBounds() override;
  const sf::Sprite& getSprite() override;
  unsigned int& getHp() override;
};

#endif