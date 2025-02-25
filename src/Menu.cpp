#include "Menu.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

using namespace Controls;

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

Menu::Menu() {
  GameStarted = false;
  if(!bg_texture.loadFromFile("../assets/gfx/menu_bg.png")) {
    std::cerr << "Failder to load menu bg image\n";
    exit(1);
  }
  bg_img.setTexture(bg_texture);
  if(!Font.loadFromFile("../assets/gfx/HussarBold.otf")) {
    std::cerr << "Failed to load font. Exiting...\n";
    exit(1);
  }
  set_button(StartButton, "Start", Font);
  set_button(SurvivalMode, "Survival Mode", Font);
  set_button(Mute_Audio, "Audio: ON", Font);
  set_button(QuitButton, "Quit", Font);

  StartButton.setPosition(340.0, 200.0); //somewhere in center for 800x600
  SurvivalMode.setPosition(250.0, 250.0); // a bit below
  Mute_Audio.setPosition(300.0, 300.0);
  QuitButton.setPosition(350.0, 350.0);
  selectButton(&StartButton);
  SelectedOption = Start;
}

int Menu::menu_loop(sf::RenderWindow& w) {
  sf::Event event;
  while(w.pollEvent(event)) {
    if (event.type == sf::Event::KeyReleased && event.key.code == UP) {
      unselectButton(MenuButtons.at(SelectedOption));
      if (SelectedOption == Start) 
        SelectedOption = Quit;
      else  
        --SelectedOption; 
      selectButton(MenuButtons.at(SelectedOption));
    } else if (event.type == sf::Event::KeyReleased && event.key.code == DOWN) {
      unselectButton(MenuButtons.at(SelectedOption));
      ++SelectedOption;
      if (SelectedOption > Quit) 
        SelectedOption = Start;
      selectButton(MenuButtons.at(SelectedOption));
    }
      
    if (event.type == sf::Event::KeyReleased && event.key.code == Z_K) {
      switch (SelectedOption) {
        case Start:
          //this break menu loop and allows to play the game
          return START_GAME;
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

PauseMenu::PauseMenu(bool& is_muted_game) {
  if(!(font.loadFromFile("../assets/gfx/dynapuff.ttf"))) {
    std::cerr << "Error loading font. Exiting...\n";
    exit(1);
  }
  set_button(continue_btn, "Continue game", font);
  continue_btn.setPosition(250, 200);

  set_button(restart_btn, "Restart level", font);
  restart_btn.setPosition(260,250);

  if (is_muted_game)
    set_button(mute_btn, "Audio: OFF", font);
  else 
    set_button(mute_btn, "Audio: ON", font);
  mute_btn.setPosition(300, 300);

  set_button(quit_btn, "Quit game", font);
  quit_btn.setPosition(290, 350);
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
  sf::Event event;
  while (w.pollEvent(event)) {
    if (event.type == sf::Event::KeyReleased && event.key.code == UP) {
      unselectButton(options_list.at(selected_opt));
      if (selected_opt == Continue)
        selected_opt = Quit;
      else
       --selected_opt;
      selectButton(options_list.at(selected_opt));
    } else if (event.type == sf::Event::KeyReleased && event.key.code == DOWN) {
      unselectButton(options_list.at(selected_opt));
      ++selected_opt;
      if (selected_opt > Quit)
        selected_opt = Continue;
      selectButton(options_list.at(selected_opt));
    }

    bool pressed_Z_K = (event.type == sf::Event::KeyReleased && event.key.code == Z_K);
    switch(selected_opt) {
      case Continue:
        if(pressed_Z_K)
          return 1;
        break;

      case Restart:
        if(pressed_Z_K)
          return 2;
        break;

      case Mute:
        if(pressed_Z_K) {
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
        if(event.type == sf::Event::KeyReleased && event.key.code == Z_K) {
          std::clog << "game exit\n";
          exit(1);
        }
        break;
    }
  }
  return 0;
}