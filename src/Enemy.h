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

class Enemy {
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
  void send_to_valhalla(sf::Sprite& sprite);
  
  /** @brief increases player score on current level
  *   @param takes reference to a current level score */
  void increase_score(unsigned int& score);
	virtual void updateEnemy(const float& delta) = 0;
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
	const float FallingSpeed = 1.2f;
	sf::Clock ShootTimer;
  // std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> player_pr_vec; 
  // std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> enemy_pr_vec;
  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr; 
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
	unsigned int hp = 10;

  // This two variables below represent the state of enemy movement
  MovePattern path;
  sf::Vector2f current_direction;

public:
  /** @brief constructs an enemy, places him in start position
      and gives current moving direction
      @param player projectile vector reference to check collision with player
      bullets, enemy projectile vector reference to shoot, struct that
      defines move pattern for enemy*/
  MoonStone(std::vector<std::unique_ptr<Projectile>>& player_prj_vec,
            std::vector<std::unique_ptr<Projectile>>& enm_prj_vec,
            MovePattern pattern);
	void updateEnemy(const float& delta) override;
  void enemyShoot() override;
  void enemy_move(const float& delta);
  sf::Rect<float> getBounds() override;
  sf::Sprite& getSprite() override;
  unsigned int& getHp() override;
  ~MoonStone();
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