#include <chrono>
#include <thread>

#include "Game.h"

int main() {
  Game game;
  game.menuLoop();

  //because if not, render fucks up
  std::this_thread::sleep_for(std::chrono::seconds(1)); 

  game.lvl1Loop();

  return 0;
}