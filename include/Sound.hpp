/// PORTED TO SFML3
// THIS IS BULLSHIT!@!!!!

#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class SoundManager {
private:
  struct Sound {
    public:
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
  bool is_muted = false;
  int load_snd(Sound &snd, const std::string &snd_path);
  int loadSounds();
  SoundManager();
  ~SoundManager();
  void playSound(const std::string &Name);
};