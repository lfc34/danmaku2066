/// PORTED TO SFML3
#include "Interface.hpp"

#include <cstdint>
#include <functional>
#include <iostream>

#include "GameState.hpp"
#include "Logger.hpp"
#include "SFML/Window/Keyboard.hpp"

//////////////////////////////////////////
////////// INTERFACE DATA
InterfaceData::InterfaceData()
: MainMenuFont("../assets/gfx/HussarBold.otf"),
  MainMenuTexture("../assets/gfx/menu_bg.png")
{
  Logger::log_msg("Interface assets loaded successfully");
}

//////////////////////////////////////////
////////// BUTTON
Button::Button(const std::string text, const sf::Font &font_ref, const std::uint8_t font_size) 
: m_text(text),
  m_font_ref(font_ref),
  m_font_size(font_size),
  drawable(m_font_ref, m_text, m_font_size)
{
  Logger::log_msg(m_text + " button constructed successfully");
}

bool Button::isSelected() {
  return m_selected;
}
void Button::select() {
  drawable.setFillColor(sf::Color::Green);
  m_selected = true;
  Logger::log_msg(m_text + " is selected");
}
void Button::unselect() {
  drawable.setFillColor(sf::Color::White);
  m_selected = false;
  Logger::log_msg(m_text + " is unselected");
}
void Button::setFunc(const std::function<void(void)> fn) {
  m_btn_func = fn;
  Logger::log_msg(m_text + " added functionality");
}
void Button::press() {
  Logger::log_msg(m_text + " button pressed");
  m_btn_func();
}

///////////////////////////////////////////
////////// MAIN MENU
MainMenu::MainMenu(const InterfaceData& uidata)
: m_uidata(uidata),
  GAME_STATE(GameState::getInstance())
{
  m_buttons.at(Start) = new Button("Start", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Start)->drawable.setPosition({340, 200});
  m_buttons.at(Start)->setFunc([this](void){ GAME_STATE.isStarted = true; });
  m_buttons.at(Survival) = new Button("Survival mode", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Survival)->drawable.setPosition({250, 250});
  m_buttons.at(Mute) = new Button("Audio: ON", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Mute)->drawable.setPosition({300, 300});
  m_buttons.at(Quit) = new Button("Quit", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Quit)->drawable.setPosition({350, 350});
  m_buttons.at(Quit)->setFunc([](void) {exit(0);});
  Logger::log_msg("Constructed MainMenu");
}

void MainMenu::displayMenu(sf::RenderWindow& window) {
  using namespace sf::Keyboard;
  sf::Sprite menu_bg(m_uidata.MainMenuTexture);
  int opt = Start;
  m_buttons.at(opt)->select(); // select Start button
  while (window.isOpen()) {
    // handle events
    while(const std::optional event = window.pollEvent()) {
      auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
      
      // picking an option
      if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Down) {
        m_buttons.at(opt)->unselect();
        opt++;
        if (opt > Quit) opt = Start;
        m_buttons.at(opt)->select();
      } else if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Up) {
        m_buttons.at(opt)->unselect();
        opt--;
        if (opt < Start) opt = Quit;
        m_buttons.at(opt)->select();
      }

      if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Enter) {
        m_buttons.at(opt)->press();
        return;
      }
    }

    // draw menu
    window.clear();
    window.draw(menu_bg);
    for (auto& btn: m_buttons) {
      window.draw(btn->drawable);
    }
    window.display();
  }
}

MainMenu::~MainMenu() {
  // delete buttons from memory
  for (auto& i : m_buttons) {
    delete i;
  }
}

// hack to suppress "no default constructor error"
const sf::Texture dummy_texture;
const sf::Font dummy_font;
const sf::Text dummy_text(dummy_font, "");

void Menu::set_button(sf::Text& Btn, std::string s, sf::Font& font) {
  Btn.setFont(font);
  Btn.setCharacterSize(FontSize);
  Btn.setFillColor(sf::Color::White);
  Btn.setString(s);
  MenuButtons.push_back(&Btn);
}

void Menu::selectButton(sf::Text* Btn) {
  Btn->setFillColor(sf::Color::Green);
}

void Menu::unselectButton(sf::Text* Btn) {
  Btn->setFillColor(sf::Color::White);
}

void Menu::drawMenu(sf::RenderWindow& w) {
  w.clear();
  w.draw(bg_img);
  w.draw(StartButton);
  w.draw(SurvivalMode);
  w.draw(Mute_Audio);
  w.draw(QuitButton);
  w.display();
}

Menu::Menu() 
: bg_img(dummy_texture), 
StartButton(dummy_text),
SurvivalMode(dummy_text),
Mute_Audio(dummy_text),
QuitButton(dummy_text)
{
  GameStarted = false;
  if(!bg_texture.loadFromFile("../assets/gfx/menu_bg.png")) {
    std::cerr << "Failed to load menu bg image\n";
    exit(1);
  }
  bg_img.setTexture(bg_texture, true);
  if(!Font.openFromFile("../assets/gfx/HussarBold.otf")) {
    std::cerr << "Failed to load font. Exiting...\n";
    exit(1);
  }
  set_button(StartButton, "Start", Font);
  set_button(SurvivalMode, "Survival Mode", Font);
  set_button(Mute_Audio, "Audio: ON", Font);
  set_button(QuitButton, "Quit", Font);

  StartButton.setPosition({340.0, 200.0}); //somewhere in center for 800x600
  SurvivalMode.setPosition({250.0, 250.0}); // a bit below
  Mute_Audio.setPosition({300.0, 300.0});
  QuitButton.setPosition({350.0, 350.0});
  selectButton(&StartButton);
  SelectedOption = Start;
}

int Menu::menu_loop(sf::RenderWindow& w) {
  using namespace sf::Keyboard;
  while(const std::optional event = w.pollEvent()) {
    auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
    if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Up) {
      unselectButton(MenuButtons.at(SelectedOption));
      if (SelectedOption == Start) 
        SelectedOption = Quit;
      else  
        --SelectedOption; 
      selectButton(MenuButtons.at(SelectedOption));
    } else if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Down) {
      unselectButton(MenuButtons.at(SelectedOption));
      ++SelectedOption;
      if (SelectedOption > Quit) 
        SelectedOption = Start;
      selectButton(MenuButtons.at(SelectedOption));
    }
      
    if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Enter) {
      switch (SelectedOption) {
        case Start:
          //this break menu loop and allows to play the game
          return START_GAME;
          break;

        case Survival:
          return SURVIVAL;
          break;

        case Mute:
          if (is_muted_game == false) {
            Mute_Audio.setString("Audio: OFF");
            is_muted_game = true;
            return MUTE_AUDIO;
          } else {
            Mute_Audio.setString("Audio: ON");
            is_muted_game = false;
            return UNMUTE_AUDIO; 
          }
          break;
              
        case Quit:
          w.close();
          break;
      }
    }
  }
  return -1;
}

PauseMenu::PauseMenu(bool& is_muted_game)
: continue_btn(dummy_text),
  restart_btn(dummy_text),
  mute_btn(dummy_text),
  quit_btn(dummy_text) {
  if(!(font.openFromFile("../assets/gfx/dynapuff.ttf"))) {
    std::cerr << "Error loading font. Exiting...\n";
    exit(1);
  }
  set_button(continue_btn, "Continue game", font);
  continue_btn.setPosition({250, 200});

  set_button(restart_btn, "Restart level", font);
  restart_btn.setPosition({260,250});

  if (is_muted_game)
    set_button(mute_btn, "Audio: OFF", font);
  else 
    set_button(mute_btn, "Audio: ON", font);
  mute_btn.setPosition({300, 300});

  set_button(quit_btn, "Quit game", font);
  quit_btn.setPosition({290, 350});
  selectButton(&continue_btn); 
  options_list = {&continue_btn, &restart_btn, &mute_btn, &quit_btn};
}

void PauseMenu::draw_menu(sf::RenderWindow& w) {
  w.clear();
  w.draw(continue_btn);
  w.draw(restart_btn);
  w.draw(mute_btn);
  w.draw(quit_btn);
  w.display();
}

int PauseMenu::menu_loop(sf::RenderWindow& w) {
  using namespace sf::Keyboard;
  while (const std::optional event = w.pollEvent()) {
    auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
    if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Up) {
      unselectButton(options_list.at(selected_opt));
      if (selected_opt == Continue)
        selected_opt = Quit;
      else
       --selected_opt;
      selectButton(options_list.at(selected_opt));
    } else if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Down) {
      unselectButton(options_list.at(selected_opt));
      ++selected_opt;
      if (selected_opt > Quit)
        selected_opt = Continue;
      selectButton(options_list.at(selected_opt));
    }

    bool pressed_ret = (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Enter);
    switch(selected_opt) {
      case Continue:
        if(pressed_ret)
          return 1;
        break;

      case Restart:
        if(pressed_ret)
          return 2;
        break;

      case Mute:
        if(pressed_ret) {
          if (mute_btn.getString() == "Audio: OFF") {
            mute_btn.setString("Audio: ON");
            return 3;
          } else {  
            mute_btn.setString("Audio: OFF");
            return 4;
          }
        }
        break;

      case Quit:
        if(pressed_ret) {
          std::clog << "game exit\n";
          exit(1);
        }
        break;
    }
  }
  return 0;
}

// Currently not present in game
// Dialogue::Dialogue() {
//   if(!font.openFromFile("../assets/gfx/dynapuff.ttf")) {
//     std::clog << "Failed to load font\n";
//     exit(1);
//   }
//   if(!frame_texture.loadFromFile("../assets/gfx/frame.png")) {
//     std::clog << "Failed to load frame bg\n";
//     exit(1);
//   }
//   frame.setTexture(frame_texture);
//   frame.setPosition({0, 400});

// }