#include <chrono>
#include <thread>

#include "Game.h"

int main() {
  Game game;
  if(game.menuLoop() == 0) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
    game.lvl1Loop();
  } else {
    std::clog << "Game exit\n";
    return 0;
  }

}