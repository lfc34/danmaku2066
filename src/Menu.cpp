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
  w.draw(StartButton);
  w.draw(SurvivalMode);
  w.draw(Mute_Audio);
  w.draw(QuitButton);
  w.display();
}

Menu::Menu() {
  GameStarted = false;
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

PauseMenu::PauseMenu() {
  if(!(font.loadFromFile("../assets/gfx/dynapuff.ttf"))) {
    std::cerr << "Error loading font. Exiting...\n";
    exit(1);
  }
  set_button(continue_btn, "Continue game", font);
  continue_btn.setPosition(250, 250);

  set_button(quit_btn, "Quit game", font);
  quit_btn.setPosition(300, 300);
  selectButton(&continue_btn); 
  options_list = {&continue_btn, &quit_btn};
}

void PauseMenu::draw_menu(sf::RenderWindow& w) {
  w.clear();
  w.draw(continue_btn);
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

    switch(selected_opt) {
      case Continue:
        if(event.type == sf::Event::KeyReleased && event.key.code == Z_K)
          return 1;
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