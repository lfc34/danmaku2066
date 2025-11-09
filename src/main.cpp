#include "Game.hpp"
#include "Logger.hpp"

int main() {
  Logger::status();
  Game& game = Game::getInstance();
  game.MainLoop();
}