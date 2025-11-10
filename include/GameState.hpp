#pragma once
#include "Logger.hpp"

// Singleton
class GameState {
public:
  enum GS_CurrentScene {
    MENU,
    LEVEL,
    SURVIVAL,
    PAUSE
  };
  GS_CurrentScene scene = MENU;
  static GameState& getInstance() {
    static GameState instance;
    return instance;
  }
  void toggleAudio() {
    AUDIO_OFF = !AUDIO_OFF;
    if (AUDIO_OFF) Logger::log_msg("Audio is muted");
    else Logger::log_msg("Audio is on");
  }
  bool isMuted() {
    return AUDIO_OFF;
  }
  
  GameState(const GameState&) = delete;
  void operator=(const GameState&) = delete;
private:
  bool AUDIO_OFF = false;
  GameState(){};
}; 