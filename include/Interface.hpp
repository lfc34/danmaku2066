/// PORTED TO SFML3
#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "AssetManager.hpp"
#include "SFML/Audio/Sound.hpp"

#include <cstdint>
#include <functional>

#include "GameState.hpp"

// loads all necessary assets for UI and gives access to them via public fields
struct InterfaceData {
  sf::Font MainMenuFont;
  sf::Font PauseMenuFont;
  sf::Texture MainMenuTexture;
  sf::Sound winScreenSound;
  InterfaceData(SoundBufferStorage& sbuf);
};

class Button {
private:
  bool m_selected = false;
  std::function<void(void)> m_btn_func;
  
public:
  sf::Text drawable;
  Button(const std::string text, const sf::Font &font_ref, const std::uint8_t font_size);
  // Add functionality to button
  bool isSelected();
  void select();
  void unselect();
  // define button functionality
  void setFunc(const std::function<void(void)> fn);
  // call button function
  void press();
};

class Menu {
protected:
  GameState& GAME_STATE;
  const std::uint8_t m_font_size = 40; // default
  const InterfaceData& m_uidata;
  std::vector<Button> m_buttons;

public:
  Menu(const InterfaceData& uidata); 
  virtual void displayMenu(sf::RenderWindow& window) = 0;
};

class MainMenu : public Menu {
public:
  enum Option { Start, Survival, Audio, Quit };
  MainMenu(const InterfaceData& uidata);
  void displayMenu(sf::RenderWindow& window) override;
};

class PauseMenu : public Menu {
public:
  enum Option { Continue, Audio, BackToMenu, Quit };
  PauseMenu(const InterfaceData& uidata);
  void displayMenu(sf::RenderWindow &window) override;
};