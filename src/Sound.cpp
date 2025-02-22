#include "Sound.h"

int SoundManager::load_snd(Sound& snd, const std::string& snd_path) {
	if (!(snd.sndBuf.loadFromFile(snd_path))) {
		std::cerr << "Error loading " << snd_path << "\n";
		return 1;
	}
	snd.snd.setBuffer(snd.sndBuf);
	assert((snd.snd.getBuffer() != NULL));
	std::clog << snd_path << " loaded succesfully\n";
	return 0;
}

int SoundManager::loadSounds() {
	load_snd(BltSound, "../assets/sfx/b_shot.ogg");
	load_snd(FairyDeath, "../assets/sfx/fairy_death.ogg");
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
	else if (Name == "fairy_death")
		FairyDeath.snd.play();
}

SoundManager::~SoundManager() {
  std::clog << "Sounds unloaded from stack\n";
}