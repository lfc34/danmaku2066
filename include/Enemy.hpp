/// PORTED TO SFML3
#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include "AssetManager.hpp"
#include "GameState.hpp"
#include "Projectile.hpp"

/** Defines enemy movement pattern in 3-point system.
    Initializes as curve line pattern by default */
struct MovePattern {
  sf::Vector2f spawn_pos {0, 0};
  sf::Vector2f mid_point {200, 85};
  sf::Vector2f end_point {600, -85};
};
constexpr MovePattern curve {};
constexpr MovePattern op_curve {
  sf::Vector2f (800, 0),
  sf::Vector2f (-200, 150),
  sf::Vector2f (-400, -85)
};
constexpr MovePattern march {
  sf::Vector2f (0, 0),
  sf::Vector2f (0, 100),
  sf::Vector2f (0, -100)
};
constexpr MovePattern lr_line {
  sf::Vector2f (0, 50),
  sf::Vector2f (0,50),
  sf::Vector2f (200, 50)
};
constexpr MovePattern rl_line {
  sf::Vector2f (800, 50),
  sf::Vector2f (800, 40),
  sf::Vector2f (-200, 50)
};

/** Contains pointers to all needed vectors */
struct EnemyPtrs {
  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr;
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
};

class Enemy {
protected:
  GameState& GAME_STATE;
  EnemyData& data;
  sf::Sprite sprite;
	sf::Clock ShootTimer; // is good

  float speed {}; // why float?

  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr; 
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
	int hp {};

  // This two variables below represent the state of enemy movement
  MovePattern path;
  sf::Vector2f current_direction {};

public: 
  Enemy(EnemyData& ed, const std::string& name);
  enum EnemyState {
    ALIVE,
    DEAD,
    FLEW_AWAY
  };
  int state = ALIVE;
  
	virtual void updateEnemy(const float& delta) = 0;
  virtual void enemyShoot() = 0;
  virtual void increase_score(unsigned int& score) = 0;
  
  void enemy_move(const float& delta);
  void send_to_valhalla();
  int& getHp();
  sf::Rect<float> getBounds();
  sf::Sprite& getSprite();
  ~Enemy();
};

class Fairy : public Enemy {
private:
  int hp = 4;
public:
  /** @brief constructs an enemy, places him in start position
      and gives current moving direction
      @param player projectile vector reference to check collision with player
      bullets, enemy projectile vector reference to shoot, struct that
      defines move pattern for enemy*/
  Fairy(EnemyPtrs& dt, MovePattern pattern, const sf::Vector2f& spawn_pos, EnemyData& ed);
  void increase_score(unsigned int& score) override;
  void updateEnemy(const float& delta) override;
  void enemyShoot() override;
};

class LizardKiller : public Enemy {
private:
  int hp = 30;
public:
  LizardKiller(EnemyPtrs& dt, MovePattern pattern, 
               const sf::Vector2f& spawn_pos,EnemyData& ed);
  void increase_score(unsigned int& score) override;
	void updateEnemy(const float& delta) override;
  void enemyShoot() override;
};

class Skull : public Enemy {
private: 
  int hp = 6;
public:
  Skull(EnemyPtrs& dt, MovePattern pattern, const sf::Vector2f spawn_pos, EnemyData& ed);
  void increase_score(unsigned int& score) override;
  void updateEnemy(const float& delta) override;
  void enemyShoot() override;
};

class Boss {
private:
  TextureStorage& m_ts;
  std::vector<std::unique_ptr<Projectile>>* plr_prj_vec_ptr;
  std::vector<std::unique_ptr<Projectile>>* enm_prj_vec_ptr;
  float speed = 250.0f;
  
  sf::Clock shoot_timer;

  sf::Texture boss_idle;
  sf::Texture boss_flying;
  sf::Texture boss_shooting;
  sf::Sprite boss_sprite;

  
  int move_stage {};
public: 
  enum State {
    Flying,
    Shooting_PH1,
    Shooting_PH2,
    Shooting_PH3,
    Dying
  };
  int state {};
  int hp = 633;
  bool triggered = false;
  Boss(EnemyPtrs& dt, TextureStorage& ts);

  sf::Sprite& get_sprite();
  void trigger();
  void move(const float& delta, const float x_offset);
  void boss_shoot(); 
  int update_boss(const float& delta, float& phase_timer);
};