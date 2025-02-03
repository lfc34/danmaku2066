/// \file
/// This file contains the declaration of sound manager  
/// which load all sounds and provide methods to play them

#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <memory> 
#include <cassert>
#include <stdexcept>
#include <string>

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

class SoundManager {
private:
	struct Sound {
		sf::SoundBuffer sndBuf;	
		sf::Sound snd;
	};

	Sound BltSound;

public:
	int loadSounds();
	SoundManager();
	~SoundManager();
	void playSound(const std::string& Name);
};

#endif