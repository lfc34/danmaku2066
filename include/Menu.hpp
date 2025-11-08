/// PORTED TO SFML3
#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Menu {
private:
  sf::Texture bg_texture;
  sf::Sprite bg_img;
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
    SURVIVAL,
    MUTE_AUDIO,
    UNMUTE_AUDIO,
    QUIT
  };
  bool GameStarted; //for breaking the menu loop in main and start game

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
  sf::Text restart_btn;
  sf::Text mute_btn;
  sf::Text quit_btn;
  enum Option { Continue, Restart, Mute, Quit };
  std::vector<sf::Text*> options_list;
  size_t selected_opt = Continue;
  // Makes it possible to mute game via pause menu

public:
  PauseMenu(bool& is_muted_game);
  void draw_menu(sf::RenderWindow& w);
  int menu_loop(sf::RenderWindow& w);
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