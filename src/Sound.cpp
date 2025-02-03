#include "Sound.h"

int SoundManager::loadSounds() {
	if (!(BltSound.sndBuf.loadFromFile("../assets/sfx/b_shot.ogg"))) {
		std::cerr << "Error loading b_shot.ogg\n";
		return 1;
	}
	BltSound.snd.setBuffer(BltSound.sndBuf);
	assert((BltSound.snd.getBuffer() != NULL));
	std::clog << "b_shot.ogg loaded succesfully\n";

	// there will be more sounds later
	return 0;
}

SoundManager::SoundManager() {
	if (loadSounds() == 0) { 
		std::clog << "All sounds loaded successfully!\n";
	} else {
		std::cerr << "Failed to load some sounds. Exiting...\n";
		exit(1);
	}
}

void SoundManager::playSound(const std::string& Name) {
	if (Name == "bullet_shot")
		BltSound.snd.play();
}

SoundManager::~SoundManager() {
  std::clog << "Sounds unloaded from stack\n";
}