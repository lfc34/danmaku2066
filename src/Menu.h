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

#include "Controls.h"

class Menu {
private:
  const unsigned int FontSize {40};
  sf::Font Font;
  sf::Text StartButton;
  sf::Text QuitButton;
  enum Option { Start, Quit };
  std::vector<sf::Text*> MenuButtons;
  size_t SelectedOption;

public:
  bool GameStarted; //for braking the menu loop in main and start game

  void setButton(sf::Text& Btn, std::string s);
  void selectButton(sf::Text* Btn);
  void unselectButton(sf::Text* Btn);
  void drawMenu(sf::RenderWindow& w);
  Menu();
  void buttonSelector(sf::RenderWindow& w, sf::Keyboard& k);

};

class PauseMenu : public Menu {
private:
  const unsigned int FontSize {40};
  sf::Font font;
  sf::Text continue_btn;
  sf::Text quit_btn;
  enum Option { Continue, Quit };
  size_t selected_opt = Continue;

public:
  PauseMenu();
  void draw_menu(sf::RenderWindow& w);
  void set_button(sf::Text& btn, std::string s);
  int menu_loop(sf::Keyboard& kb);
};

#endif