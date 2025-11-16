/// PORTED TO SFML3
#pragma once

#include "Enemy.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

#include "GameState.hpp"
#include "AssetManager.hpp"

// some of the methods and variables are extremely stupid
// consider removing them before releasing game
class Player {
private:
  GameState& GAME_STATE;
  PlayerData& m_data;

  const float m_playerSpeed = 320.0f;
  // precisely calculated lmao
  const float m_diagSlowdown = 1.414213f; 
  const sf::Vector2f m_startPos = {400, 500};

  sf::Clock invuln_clock;
  bool m_isInvuln = false;
  
  // this made to avoid matryoshka when you pass address
  // to one function to pass it to another func etc
  std::vector<std::unique_ptr<Projectile>>* m_projVecPtr;
  // std::shared_ptr<sstd::vector<std::unique_ptr<Projectile>>> enemy_prj_vec;
  std::vector<std::unique_ptr<Projectile>>* m_enemyPrjVecPtr;
  // std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies_vec;
  std::vector<std::unique_ptr<Enemy>>* m_enemyVecPtr;

public:
  Player();
  int lives = 5;
  void loadPlayerModel();
  void placeStartPos();
  Player(std::vector<std::unique_ptr<Projectile>>& pv,
         std::vector<std::unique_ptr<Enemy>>& enm_vec,
         std::vector<std::unique_ptr<Projectile>>& en_proj_vec,
         PlayerData& pdata);

  ~Player();

  sf::Clock PlayerShootTimer; 

  sf::Vector2f getPlayerPosition(); 
  sf::FloatRect getPlayerBounds(); 

  bool isMovingOutOfBnds();
  void move(float x, float y, const float& delta); 
  void fire();
  // returns int because it's signal to potentially pause the game
  void input_handler(const float& delta); 
  void player_invuln();
  bool check_collision();

  void updatePlayer(sf::RenderWindow& w);
};