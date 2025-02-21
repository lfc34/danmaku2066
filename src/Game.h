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

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <exception>
#include <memory>
#include <random>
#include <new>

class Game 
{
public:
  sf::RenderWindow window;
  sf::Keyboard kb;

  Game();

  int menuLoop();
  void lvl1Loop();
  void game_pause();
  void game_over(sf::Font& font, unsigned int& score);
  // TODO: if possible, make it output message about either successful 
  // or either bad exit from game (e.g. crash)
  ~Game();
};

#endif