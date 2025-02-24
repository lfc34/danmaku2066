#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "Controls.h"

class Menu {
private:
  const unsigned int FontSize {40};
  sf::Font Font;
  sf::Text StartButton;
  sf::Text SurvivalMode;
  sf::Text Mute_Audio;
  sf::Text QuitButton;
  enum Option { Start, Survival, Mute, Quit };
  std::vector<sf::Text*> MenuButtons;
  size_t SelectedOption;
  bool is_muted_game = false;

public:
  enum MenuReturn {
    START_GAME,
    MUTE_AUDIO,
    UNMUTE_AUDIO,
    QUIT
  };
  bool GameStarted; //for braking the menu loop in main and start game

  void set_button(sf::Text& Btn, std::string s, sf::Font& font);
  void selectButton(sf::Text* Btn);
  void unselectButton(sf::Text* Btn);
  void drawMenu(sf::RenderWindow& w);
  Menu();
  int menu_loop(sf::RenderWindow& w);

};

class PauseMenu : public Menu {
private:
  const unsigned int FontSize {40};
  sf::Font font;
  sf::Text continue_btn;
  sf::Text quit_btn;
  enum Option { Continue, Quit };
  std::vector<sf::Text*> options_list;
  size_t selected_opt = Continue;

public:
  PauseMenu();
  void draw_menu(sf::RenderWindow& w);
  int menu_loop(sf::RenderWindow& w);
};

#endif