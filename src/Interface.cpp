/// PORTED TO SFML3
#include "Interface.hpp"

#include "SFML/Window/Keyboard.hpp"

#include <cstdint>
#include <functional>

#include "GameState.hpp"
#include "Logger.hpp"

//////////////////////////////////////////
////////// INTERFACE DATA
InterfaceData::InterfaceData()
    : MainMenuFont("../assets/gfx/HussarBold.otf"),
      MainMenuTexture("../assets/gfx/menu_bg.png"),
      PauseMenuFont("../assets/gfx/dynapuff.ttf") {
  Logger::log_msg("Interface assets loaded successfully");
}

//////////////////////////////////////////
////////// BUTTON
Button::Button(const std::string text, const sf::Font &font_ref,
               const std::uint8_t font_size)
    : drawable(font_ref, text, font_size) {
  Logger::log_msg(text + " button constructed successfully");
}

bool Button::isSelected() { return m_selected; }
void Button::select() {
  drawable.setFillColor(sf::Color::Green);
  m_selected = true;
  Logger::log_msg(drawable.getString() + " is selected");
}
void Button::unselect() {
  drawable.setFillColor(sf::Color::White);
  m_selected = false;
  Logger::log_msg(drawable.getString() + " is unselected");
}
void Button::setFunc(const std::function<void(void)> fn) {
  m_btn_func = fn;
  Logger::log_msg(drawable.getString() + " added functionality");
}
void Button::press() {
  Logger::log_msg(drawable.getString() + " button pressed");
  m_btn_func();
}

///////////////////////////////////////////
////////// MENU
Menu::Menu(const InterfaceData& uidata) 
: GAME_STATE(GameState::getInstance()),
  m_uidata(uidata)
{
}

///////////////////////////////////////////
////////// MAIN MENU
// MainMenu::MainMenu(const InterfaceData& uidata) : Menu(uidata)
// {
//   m_buttons.at(Start) = new Button("Start", m_uidata.MainMenuFont, m_font_size);
//   m_buttons.at(Start)->drawable.setPosition({340, 200});
//   m_buttons.at(Start)->setFunc(
//       [this](void) { GAME_STATE.scene = GameState::LEVEL; });

//   m_buttons.at(Survival) =
//       new Button("Survival mode", m_uidata.MainMenuFont, m_font_size);
//   m_buttons.at(Survival)->drawable.setPosition({250, 250});
//   m_buttons.at(Survival)->setFunc(
//       [this](void) { GAME_STATE.scene = GameState::SURVIVAL; });

//   if (GAME_STATE.isMuted()) {
//     m_buttons.at(Audio) =
//         new Button("Audio: OFF", m_uidata.MainMenuFont, m_font_size);
//   } else {
//     m_buttons.at(Audio) =
//         new Button("Audio: ON", m_uidata.MainMenuFont, m_font_size);
//   }
//   m_buttons.at(Audio)->drawable.setPosition({300, 300});
//   m_buttons.at(Audio)->setFunc([this](void) {
//     GAME_STATE.toggleAudio();
//     if (GAME_STATE.isMuted())
//       m_buttons.at(Audio)->drawable.setString("Audio: OFF");
//     else
//       m_buttons.at(Audio)->drawable.setString("Audio: ON");
//   });

//   m_buttons.at(Quit) = new Button("Quit", m_uidata.MainMenuFont, m_font_size);
//   m_buttons.at(Quit)->drawable.setPosition({350, 350});
//   m_buttons.at(Quit)->setFunc([](void) { exit(0); });
//   Logger::log_msg("Constructed MainMenu");
// }

MainMenu::MainMenu(const InterfaceData& uidata) : Menu(uidata)
{
  m_buttons.emplace_back("Start", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Start).drawable.setPosition({340, 200});
  m_buttons.at(Start).setFunc(
      [this](void) { GAME_STATE.scene = GameState::LEVEL; });

  m_buttons.emplace_back("Survival mode", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Survival).drawable.setPosition({250, 250});
  m_buttons.at(Survival).setFunc(
      [this](void) { GAME_STATE.scene = GameState::SURVIVAL; });

  if (GAME_STATE.isMuted()) {
    m_buttons.emplace_back("Audio: OFF", m_uidata.MainMenuFont, m_font_size);
  } else {
    m_buttons.emplace_back("Audio: ON", m_uidata.MainMenuFont, m_font_size);
  }
  m_buttons.at(Audio).drawable.setPosition({300, 300});
  m_buttons.at(Audio).setFunc([this](void) {
    GAME_STATE.toggleAudio();
    if (GAME_STATE.isMuted())
      m_buttons.at(Audio).drawable.setString("Audio: OFF");
    else
      m_buttons.at(Audio).drawable.setString("Audio: ON");
  });

  m_buttons.emplace_back("Quit", m_uidata.MainMenuFont, m_font_size);
  m_buttons.at(Quit).drawable.setPosition({350, 350});
  m_buttons.at(Quit).setFunc([](void) { exit(0); });
  Logger::log_msg("Constructed MainMenu");
}

void MainMenu::displayMenu(sf::RenderWindow &window) {
  using namespace sf::Keyboard;
  sf::Sprite menu_bg(m_uidata.MainMenuTexture);
  int opt = Start;
  m_buttons.at(opt).select(); // select Start button
  while (window.isOpen()) {
    // handle events
    while (const std::optional event = window.pollEvent()) {
      auto *keyReleased = event->getIf<sf::Event::KeyReleased>();
      if (event->is<sf::Event::Closed>()) {
        window.close();
        exit(0);
      }

      // picking an option
      if (event->is<sf::Event::KeyReleased>() &&
          keyReleased->code == Key::Down) {
        m_buttons.at(opt).unselect();
        opt++;
        if (opt > Quit)
          opt = Start;
        m_buttons.at(opt).select();
      } else if (event->is<sf::Event::KeyReleased>() &&
                 keyReleased->code == Key::Up) {
        m_buttons.at(opt).unselect();
        opt--;
        if (opt < Start)
          opt = Quit;
        m_buttons.at(opt).select();
      }

      if (event->is<sf::Event::KeyReleased>() &&
          keyReleased->code == Key::Enter) {
        m_buttons.at(opt).press();
        if (opt != Audio)
          return;
      }
    }

    // draw menu
    window.clear();
    window.draw(menu_bg);
    for (auto &btn : m_buttons) {
      window.draw(btn.drawable);
    }
    window.display();
  }
}

PauseMenu::PauseMenu(const InterfaceData& uidata) : Menu(uidata)
{
  m_buttons.emplace_back("Continue", m_uidata.PauseMenuFont, m_font_size);
  m_buttons.at(Continue).drawable.setPosition({290, 200});
  m_buttons.at(Continue).setFunc(
      [this](void) { GAME_STATE.scene = GameState::LEVEL; });

  if (GAME_STATE.isMuted()) {
    m_buttons.emplace_back("Audio: OFF", m_uidata.PauseMenuFont, m_font_size);
  } else {
    m_buttons.emplace_back("Audio: ON", m_uidata.PauseMenuFont, m_font_size);
  }
  m_buttons.at(Audio).drawable.setPosition({290, 250});
  m_buttons.at(Audio).setFunc([this](void) {
    GAME_STATE.toggleAudio();
    if (GAME_STATE.isMuted())
      m_buttons.at(Audio).drawable.setString("Audio: OFF");
    else
      m_buttons.at(Audio).drawable.setString("Audio: ON");
  });

  m_buttons.emplace_back("Return to menu", m_uidata.PauseMenuFont, m_font_size);
  m_buttons.at(BackToMenu).drawable.setPosition({230, 300});
  m_buttons.at(BackToMenu).setFunc([this](void) { GAME_STATE.scene = GameState::MENU; });

  m_buttons.emplace_back("Quit", m_uidata.PauseMenuFont, m_font_size);
  m_buttons.at(Quit).drawable.setPosition({340, 350});
  m_buttons.at(Quit).setFunc([](void) { exit(0); });
}

void PauseMenu::displayMenu(sf::RenderWindow &window) {
  using namespace sf::Keyboard;
  int opt = Continue;
  m_buttons.at(opt).select(); // select Start button
  while (window.isOpen()) {
    // handle events
    while (const std::optional event = window.pollEvent()) {
      auto *keyReleased = event->getIf<sf::Event::KeyReleased>();
      if (event->is<sf::Event::Closed>()) {
        window.close();
        exit(0);
      }

      // picking an option
      if (event->is<sf::Event::KeyReleased>() &&
          keyReleased->code == Key::Down) {
        m_buttons.at(opt).unselect();
        opt++;
        if (opt > Quit)
          opt = Continue;
        m_buttons.at(opt).select();
      } else if (event->is<sf::Event::KeyReleased>() &&
                 keyReleased->code == Key::Up) {
        m_buttons.at(opt).unselect();
        opt--;
        if (opt < Continue)
          opt = Quit;
        m_buttons.at(opt).select();
      }

      if (event->is<sf::Event::KeyReleased>() &&
          keyReleased->code == Key::Enter) {
        m_buttons.at(opt).press();
        if (opt != Audio)
          return;
      }
    }

    // draw menu
    window.clear();
    for (auto &btn : m_buttons) {
      window.draw(btn.drawable);
    }
    window.display();
  }
}