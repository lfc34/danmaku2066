/// PORTED TO SFML3 v

/// The main game class, WHICH IS TOTAL MESS RIGHT NOW
/// must have here: game state data
#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameState.hpp"
#include "Interface.hpp"
#include "Sound.hpp"

// Singleton
class Game {
private:
  InterfaceData m_uidata;
  // GameData m_gdata;
  GameState &GAME_STATE;
  Game();

public:
  Game(const Game &) = delete;
  void operator=(const Game &) = delete;

  static Game &getInstance() {
    static Game instance;
    return instance;
  }

  sf::RenderWindow window;
  enum GameMode { Game_Level, Game_Survival };
  int game_mode{};
  SoundManager SndMgr;

  void MainLoop();
  void game_pause(sf::Clock &wave_timer, sf::Clock &spawn_timer);
  void game_over(const sf::Font &font, int &score);
  void win_screen(int &f_score, int &plr_lives, const sf::Font &font);
  void lvl1Loop();
  void survival_loop();
  // TODO: if possible, make it output message about either successful
  // or either bad exit from game (e.g. crash)
  ~Game();
};
