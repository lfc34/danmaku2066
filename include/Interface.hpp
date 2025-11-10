/// PORTED TO SFML3
#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cstdint>
#include <array>
#include <functional>

#include "GameState.hpp"

// loads all necessary assets for UI and gives access to them via public fields
struct InterfaceData {
  sf::Font MainMenuFont;
  sf::Font PauseMenuFont;
  sf::Texture MainMenuTexture;
  InterfaceData();
};

class Button {
private:
  bool m_selected = false;
  std::function<void(void)> m_btn_func;
  
public:
  sf::Text drawable;
  Button(const std::string text, const sf::Font &font_ref, const std::uint8_t font_size);
  Button(Button &) = delete;
  // Add functionality to button
  bool isSelected();
  void select();
  void unselect();
  // define button functionality
  void setFunc(const std::function<void(void)> fn);
  // call button function
  void press();
};

// all assets must be in InterfaceData, only functionality should be left here
// Singletone
class MainMenu {
private:
  GameState& GAME_STATE;
  const InterfaceData& m_uidata;
  const std::uint8_t m_font_size = 40;
  std::array<Button*, 4> m_buttons;
public:
  enum Option { Start, Survival, Audio, Quit };
  MainMenu(const InterfaceData& uidata);
  void displayMenu(sf::RenderWindow& window);
  ~MainMenu();
};

class PauseMenu {
private:
  GameState& GAME_STATE;
  const InterfaceData& m_uidata;
  const std::uint8_t m_font_size = 40;
  std::array<Button*, 4> m_buttons;
public:
  enum Option { Continue, Audio, BackToMenu, Quit };
  PauseMenu(const InterfaceData& uidata);
  void displayMenu(sf::RenderWindow &window);
  ~PauseMenu();
};