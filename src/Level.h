#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>

#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Defaults.h"

class Level {
private:
	sf::Music LvlMusic;
	sf::Texture BgTexture;
	sf::Sprite BgSprite;
	
public:
	Level(std::string mus_path, std::string texture_path);
	void drawLevel(sf::RenderWindow& w);
	// TODO: in future the volume of music will be set in settings
	void playMusic();
	void stop_music();
};

#endif