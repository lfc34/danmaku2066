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

/** Defines enemy movement pattern in 3-point system.
    Initializes as curve line pattern by default */
struct MovePattern {
  sf::Vector2f spawn_pos {0, 0};
  sf::Vector2f mid_point {200, 200};
  sf::Vector2f end_point {400, -100};
};

struct EnemyData {
  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr;
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
};

class Enemy {
protected:
  sf::Texture EnemyTexture;
	sf::Sprite EnemySprite;
	sf::Clock ShootTimer;

  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr; 
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
	unsigned int hp {};

  // This two variables below represent the state of enemy movement
  MovePattern path;
  sf::Vector2f current_direction;

public: 
  enum EnemyState {
    ALIVE,
    DEAD,
    FLEW_AWAY
  };
  int state = ALIVE;
  /* because otherwise you can get hit by enemy sprite which
  *   hitbox left on the screen after death 
  * @brief Sends enemy sprite out of playable screen
  * @param sprite of any entity */
  
  /** @brief increases player score on current level
  *   @param takes reference to a current level score */
	virtual void updateEnemy(const float& delta) = 0;
  virtual void enemyShoot() = 0;
  virtual void enemy_move(const float& delta) = 0;
  
  void increase_score(unsigned int& score);
  void send_to_valhalla(sf::Sprite& sprite);
  sf::Rect<float> getBounds();
  sf::Sprite& getSprite();
  unsigned int& getHp();
  ~Enemy();
};

class MoonStone : public Enemy {
private:
  const float FallingSpeed = 1.2f;
  unsigned int hp = 4;
public:
  /** @brief constructs an enemy, places him in start position
      and gives current moving direction
      @param player projectile vector reference to check collision with player
      bullets, enemy projectile vector reference to shoot, struct that
      defines move pattern for enemy*/
  MoonStone(EnemyData& dt, MovePattern pattern, const sf::Vector2f& spawn_pos);
	void updateEnemy(const float& delta) override;
  void enemyShoot() override;
  void enemy_move(const float& delta) override;
};

class LizardKiller : public Enemy {
private:
  const float speed = 0.5f;
  unsigned int hp = 8;
public:
  LizardKiller(EnemyData& dt, MovePattern pattern, 
               const sf::Vector2f& spawn_pos);
	void updateEnemy(const float& delta) override;
  void enemyShoot() override;
  void enemy_move(const float& delta) override;
};

#endif