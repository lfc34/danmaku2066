#include "Menu.h"
#include <SFML/Window/Keyboard.hpp>

using namespace Controls;

void Menu::setButton(sf::Text& Btn, std::string s) {
  Btn.setFont(Font);
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
  w.draw(StartButton);
  w.draw(QuitButton);
}

Menu::Menu() {
  GameStarted = false;
  if(!Font.loadFromFile("../assets/gfx/HussarBold.otf")) {
    std::cerr << "Failed to load font. Exiting...\n";
    exit(1);
  }
  setButton(StartButton, "Start");
  setButton(QuitButton, "Quit");

  StartButton.setPosition(350.0, 250.0); //somewhere in center for 800x600
  QuitButton.setPosition(350.0, 300.0); // a bit below
  selectButton(&StartButton);
  SelectedOption = Start;
}

void Menu::buttonSelector(sf::RenderWindow& w, sf::Keyboard& k) {
  // the delay needed because game processes keypresses too fast 
  // and it's almost impossible to select needed button
  static sf::Clock kbDelayTimer;
  int delay = kbDelayTimer.getElapsedTime().asMilliseconds();
  if (k.isKeyPressed(UP) && delay > 300) {
    // std::clog << "Up was pressed; Val = " << SelectedOption << "\n";
    unselectButton(MenuButtons.at(SelectedOption));
    if (SelectedOption == Start) SelectedOption = Quit;
    else  
      --SelectedOption; 
    selectButton(MenuButtons.at(SelectedOption));
    delay = kbDelayTimer.restart().asMilliseconds();
  } else if (k.isKeyPressed(DOWN) && delay > 300) {
    // std::clog << "Down was pressed; Val = " << SelectedOption << "\n";
    unselectButton(MenuButtons.at(SelectedOption));
    ++SelectedOption;
    if (SelectedOption > Quit) SelectedOption = Start;
    selectButton(MenuButtons.at(SelectedOption));
    delay = kbDelayTimer.restart().asMilliseconds();
  }
    
  if (k.isKeyPressed(Z_K)) {
    switch (SelectedOption) {
      case Start:
        //this break menu loop and allow to play the game
        GameStarted = true; 
        break;
            
      case Quit:
        w.close();
        break;
    }
  }
}

PauseMenu::PauseMenu() {
  if(!(font.loadFromFile("../assets/gfx/HussarBold.otf"))) {
    std::cerr << "Error loading font. Exiting...\n";
    exit(1);
  }
  setButton(continue_btn, "Continue game");
  continue_btn.setPosition(100, 200);

  setButton(quit_btn, "Quit game");
  quit_btn.setPosition(100, 250);
  selectButton(&continue_btn); 
}

void PauseMenu::draw_menu(sf::RenderWindow& w) {
  w.clear();
  w.draw(continue_btn);
  w.draw(quit_btn);
  w.display();
}

int PauseMenu::menu_loop(sf::Keyboard& kb) {
  if (kb.isKeyPressed(DOWN)) {
    unselectButton(&continue_btn);
    selectButton(&quit_btn);
    selected_opt = Quit;
  } else if (kb.isKeyPressed(UP)) {
    unselectButton(&quit_btn);
    selectButton(&continue_btn);
    selected_opt = Continue;
  }

  switch(selected_opt) {
    case Continue:
      if(kb.isKeyPressed(Z_K))
        return 1;
      break;

    case Quit:
      if(kb.isKeyPressed(Z_K)) {
        std::clog << "game exit\n";
        exit(1);
      }
      break;
  }
  return 0;
}