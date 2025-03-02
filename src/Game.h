#ifndef GAME_H
#define GAME_H

#include "Menu.h"
#include "Defaults.h"
#include "Player.h"
#include "Projectile.h"
#include "Level.h"
#include "Sound.h"
#include "Enemy.h"
#include "Controls.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <exception>
#include <memory>
#include <random>
#include <new>
#include <random>
#include <array>
#include <sys/types.h>
#include <map>

class Game {
public:
  enum GameMode {
    Game_Level,
    Game_Survival
  };
  int game_mode {};
  bool IS_GAME_MUTED = false;
  SoundManager SndMgr;
  sf::RenderWindow window;
  sf::Keyboard kb;

  Game();

  int menuLoop();
  void game_pause(sf::Clock& wave_timer, sf::Clock& spawn_timer);
  void game_over(sf::Font& font, int& score);
  void win_screen(int& f_score, int& plr_lives, sf::Font& font);
  void lvl1Loop();
  void survival_loop();
  // TODO: if possible, make it output message about either successful 
  // or either bad exit from game (e.g. crash)
  ~Game();
};

#endif