#pragma once
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <unordered_map>

// General idea: store raw data in one instance,
// and items dependant on them in separate struct

struct TextureStorage {
  TextureStorage();
  std::unordered_map<std::string, sf::Texture> textures;
  const sf::Texture& getData(const std::string& key);
};

struct SoundBufferStorage {
  SoundBufferStorage();
  std::unordered_map<std::string, sf::SoundBuffer> snd_bufs;
  const sf::SoundBuffer& getData(const std::string& key);
};

// single owner
struct PlayerData {
  PlayerData(TextureStorage& ts, SoundBufferStorage& sbs);
  sf::Sprite sprite;
  sf::Sound snd_hurt;
  sf::Sound snd_death;
  sf::Sound snd_fire;
};

// shared across
struct EnemyData {
  EnemyData(TextureStorage& ts, SoundBufferStorage& sbs);

  sf::Sound snd_fairy_death;
  sf::Sound snd_lizard_death;

  std::unordered_map<std::string, sf::Sprite> sprites;
  // return a copy; all the monsters can't own single sprite, right?
  sf::Sprite getSprite(const std::string& key);
};