#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <memory>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Projectile.h"
#include "Enemy.h"
#include "Sound.h"
#include "Controls.h"

class Player {
private:
  sf::Texture PlayerTexture;
  sf::Sprite PlayerSprite;
  const float PlayerSpeed = 5.0f;
  const float DiagSlowdown = 1.414213f; 
  const float StartPosX = 350;
  const float StartPosY = 500;

  int lives = 3;
  
  // this made to avoid matryoshka when you pass address
  // to one function to pass it to another func etc
  std::shared_ptr<SoundManager> SndMgr;
  std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> ProjVec;
  std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> enemy_prj_vec;
  std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies_vec;

public:
  void loadPlayerModel();
  void placeStartPos();
  Player(std::shared_ptr<SoundManager> smg,
         std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> pv,
         std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enm_vec,
         std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> en_proj_vec);

  sf::Clock PlayerShootTimer; 

  sf::Vector2f getPlayerPosition(); 
  sf::Rect<float> getPlayerBounds(); 

  bool isMovingOutOfBnds();
  void move(float x, float y); 
  void fire();
  void kbInputHandler(sf::Keyboard& kb); 
  void player_die();

  void updatePlayer(sf::RenderWindow& w);
};

#endif