/// PORTED TO SFML3
#pragma once

#include "Enemy.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Sound.hpp"

// some of the methods and variables are extremely stupid
// consider removing them before releasing game
class Player {
private:
  sf::Texture PlayerTexture;
  sf::Sprite PlayerSprite;
  const float PlayerSpeed = 320.0f;
  // precisely calculated lmao
  const float DiagSlowdown = 1.414213f; 
  const float StartPosX = 400;
  const float StartPosY = 500;

  sf::Clock invuln_clock;
  bool is_invuln = false;
  
  // this made to avoid matryoshka when you pass address
  // to one function to pass it to another func etc
  std::vector<std::unique_ptr<Projectile>>* proj_vec_ptr;
  // std::shared_ptr<sstd::vector<std::unique_ptr<Projectile>>> enemy_prj_vec;
  std::vector<std::unique_ptr<Projectile>>* enemy_prj_vec_ptr;
  // std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies_vec;
  std::vector<std::unique_ptr<Enemy>>* enemies_vec_ptr;

public:
  int lives = 100;
  void loadPlayerModel();
  void placeStartPos();
  Player(std::vector<std::unique_ptr<Projectile>>& pv,
         std::vector<std::unique_ptr<Enemy>>& enm_vec,
         std::vector<std::unique_ptr<Projectile>>& en_proj_vec);

  /** Destructor made only to avoid memory leaks.
      @brief set all object pointers to nullptr */
  ~Player();

  sf::Clock PlayerShootTimer; 

  sf::Vector2f getPlayerPosition(); 
  sf::FloatRect getPlayerBounds(); 

  bool isMovingOutOfBnds();
  void move(float x, float y, const float& delta); 
  void fire(SoundManager& smg);
  // returns int because it's signal to potentially pause the game
  void input_handler(const float& delta, SoundManager& smg); 
  void player_invuln();
  bool check_collision();

  void updatePlayer(sf::RenderWindow& w, SoundManager& smg);
};