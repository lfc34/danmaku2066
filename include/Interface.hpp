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
  std::array<Button*, 4> m_buttons;
  const std::uint8_t m_font_size = 40;
public:
  enum Option { Start, Survival, Audio, Quit };
  MainMenu(const InterfaceData& uidata);
  void displayMenu(sf::RenderWindow& window);
  ~MainMenu();
};

class Menu {
private:
  sf::Texture bg_texture;
  sf::Sprite bg_img;
  const unsigned int FontSize{40};
  sf::Font Font;
  sf::Text StartButton;
  sf::Text SurvivalMode;
  sf::Text Mute_Audio;
  sf::Text QuitButton;
  enum Option { Start, Survival, Mute, Quit };
  std::vector<sf::Text *> MenuButtons;
  size_t SelectedOption;

public:
  enum MenuReturn { START_GAME, SURVIVAL, MUTE_AUDIO, UNMUTE_AUDIO, QUIT };
  bool GameStarted; // for breaking the menu loop in main and start game

  void set_button(sf::Text &Btn, std::string s, sf::Font &font);
  void selectButton(sf::Text *Btn);
  void unselectButton(sf::Text *Btn);
  void drawMenu(sf::RenderWindow &w);
  Menu();
  int menu_loop(sf::RenderWindow &w);
};

class PauseMenu : public Menu {
private:
  GameState& GAME_STATE;
  const unsigned int FontSize{40};
  sf::Font font;
  sf::Text continue_btn;
  sf::Text restart_btn;
  sf::Text mute_btn;
  sf::Text quit_btn;
  enum Option { Continue, Restart, Mute, Quit };
  std::vector<sf::Text *> options_list;
  size_t selected_opt = Continue;
  // Makes it possible to mute game via pause menu

public:
  PauseMenu();
  void draw_menu(sf::RenderWindow &w);
  int menu_loop(sf::RenderWindow &w);
};

// currently not present in game
// class Dialogue {
// private:
//   sf::Font font;
//   sf::Texture frame_texture;
//   sf::Sprite frame;
//   sf::Text char_name[2];
//   sf::Text frame_text;
// public:
//   enum DialogueState {
//     IN_DIALOGUE,
//     DIALOGUE_END
//   };
//   Dialogue();
//   int dialogue_loop(sf::RenderWindow& w);
// };