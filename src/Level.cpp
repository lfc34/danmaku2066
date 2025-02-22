#include "Level.h"

Level::Level(std::string mus_path, std::string texture_path) {
	std::clog << "Loading music...\n";
	if(!(LvlMusic.openFromFile(mus_path))) {
		std::cerr << "Failed to load music. Exiting...\n";
    exit(1);
  }

	std::clog << "Loading background...\n";
	if(!(BgTexture.loadFromFile(texture_path))) {
    std::cerr << "Failed to load background. Exiting...\n";
    exit(1);
  }

	BgSprite.setTexture(BgTexture);
	BgSprite.setOrigin(0, SCREEN_HEIGHT);
}

void Level::drawLevel(sf::RenderWindow& w) {
	w.draw(BgSprite);
	if(!(BgSprite.getPosition().y > SCREEN_HEIGHT))
		BgSprite.move(0, 0.2f); 
}

void Level::playMusic()
{
	LvlMusic.setVolume(MUSIC_VOLUME);
	LvlMusic.play();
}

void Level::stop_music() {
	LvlMusic.pause();
}
