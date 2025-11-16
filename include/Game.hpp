/// PORTED TO SFML3 v

/// The main game class, WHICH IS TOTAL MESS RIGHT NOW
/// must have here: game state data
#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "AssetManager.hpp"
#include "GameState.hpp"
#include "Interface.hpp"

// Singleton
class Game {
private:
  GameState &GAME_STATE;

  TextureStorage textures;
  SoundBufferStorage sounds;
  InterfaceData uidata;
  PlayerData player_data;
  EnemyData enemy_data;

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

  void MainLoop();
  void pauseGame(sf::Clock &wave_timer, sf::Clock &spawn_timer);
  void showGameOverScreen(const sf::Font &font, int &score);
  void showFinalScoreScreen(int &f_score, int &plr_lives, const sf::Font &font);
  void lvl1Loop();
  void survival_loop();
  ~Game();
};
