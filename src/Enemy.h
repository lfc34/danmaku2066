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
#include "Sound.h"

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

  float speed {};

  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr; 
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
	unsigned int hp {};

  // This two variables below represent the state of enemy movement
  MovePattern path;
  sf::Vector2f current_direction {};

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
	virtual void updateEnemy(const float& delta, SoundManager& smg) = 0;
  virtual void enemyShoot() = 0;
  
  void enemy_move(const float& delta);
  virtual void increase_score(unsigned int& score) = 0;
  void send_to_valhalla(sf::Sprite& sprite);
  sf::Rect<float> getBounds();
  sf::Sprite& getSprite();
  unsigned int& getHp();
  ~Enemy();
};

class Fairy : public Enemy {
private:
  unsigned int hp = 4;
public:
  /** @brief constructs an enemy, places him in start position
      and gives current moving direction
      @param player projectile vector reference to check collision with player
      bullets, enemy projectile vector reference to shoot, struct that
      defines move pattern for enemy*/
  Fairy(EnemyData& dt, MovePattern pattern, const sf::Vector2f& spawn_pos);
  void increase_score(unsigned int& score) override;
	void updateEnemy(const float& delta, SoundManager& smg) override;
  void enemyShoot() override;
};

class LizardKiller : public Enemy {
private:
  unsigned int hp = 30;
public:
  LizardKiller(EnemyData& dt, MovePattern pattern, 
               const sf::Vector2f& spawn_pos);
  void increase_score(unsigned int& score) override;
	void updateEnemy(const float& delta, SoundManager& smg) override;
  void enemyShoot() override;
};

class Skull : public Enemy {
private: 
  unsigned int hp = 6;
public:
  Skull(EnemyData& dt, MovePattern pattern, const sf::Vector2f spawn_pos);
  void increase_score(unsigned int& score) override;
  void updateEnemy(const float& delta, SoundManager& smg) override;
  void enemyShoot() override;
};

#endif