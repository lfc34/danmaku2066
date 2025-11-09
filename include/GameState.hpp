#pragma once

// Singletone
class GameState {
public:
  bool isStarted;
  static GameState& getInstance() {
    static GameState instance;
    return instance;
  }

  GameState(const GameState&) = delete;
  void operator=(const GameState&) = delete;
private:
  GameState(){};
}; 