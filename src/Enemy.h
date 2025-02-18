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

struct EnemyPathWay {
  sf::Vector2f spawn_pos {};
  sf::Vector2f mid_point {};
  sf::Vector2f end_point {};
};

class Enemy {
public: 
  bool is_dead = false;
  /* because otherwise you can get hit by enemy sprite which
  *   hitbox left on the screen after death 
  * @brief Sends enemy sprite out of playable screen
  * @param sprite of any entity */
  void send_to_valhalla(sf::Sprite& sprite);
	virtual void updateEnemy() = 0;
  virtual void enemyShoot() = 0;
  virtual sf::Rect<float> getBounds() = 0;
  virtual sf::Sprite& getSprite() = 0;
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
  sf::Sprite& getSprite() override;
  unsigned int& getHp() override;
};

// WARNING!!! THIS CLASS IS FOR TESTS ONLY
class DummyEnemy {
private:
  const float speed = 300.0f; // for future delta time calc
  unsigned int hp = 5;
public: 
  sf::CircleShape enemy_shape;
  DummyEnemy();
  void move_enemy(sf::Vector2f current_direction, float& delta);
};

#endif