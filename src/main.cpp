#include <chrono>
#include <thread>
#include <iostream>

#include "Game.hpp"

int main() {
  Game game;
  switch (game.menuLoop()) {
    case 0:
      std::this_thread::sleep_for(std::chrono::seconds(1));
      game.lvl1Loop();
      break;

    case 1:
      std::this_thread::sleep_for(std::chrono::seconds(1));
      game.survival_loop();
      break;
    
    case -1:
      std::clog << "Game exit\n";
      return 0;
  }

}