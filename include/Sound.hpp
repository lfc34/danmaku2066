/// PORTED TO SFML3
// THIS IS BULLSHIT!@!!!!
// ALL THE SOUNDS, sprites, fonts, ETC, used in game MUST be in GameData

#pragma once

#include "GameState.hpp"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class SoundManager {
private:
  GameState& GAME_STATE;
  struct Sound {
    Sound();
    sf::SoundBuffer sndBuf;
    sf::Sound snd;
  };

  Sound BltSound;
  Sound FairyDeath;
  Sound LizardDeath;
  Sound PlayerHurt;
  Sound PlayerDead;
  Sound GameWin;

public:
  int load_snd(Sound &snd, const std::string &snd_path);
  int loadSounds();
  SoundManager();
  void playSound(const std::string &Name);
};