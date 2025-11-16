#include "AssetManager.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <filesystem>
#include <unordered_map>
#include "Logger.hpp"

TextureStorage::TextureStorage() {
  const std::filesystem::path tex_folder = "../assets/gfx/textures/";
  for (auto const& entry : std::filesystem::directory_iterator(tex_folder)) {
    if(!entry.is_regular_file()) continue; // skip directories and anything else
    auto path = entry.path();
    auto ext = path.extension().string();
    if (ext != ".png") continue; // skip non png files
    sf::Texture tex;
    if (!tex.loadFromFile(path.string())) {
      Logger::log_err("Failed to load " + path.string());
      continue;
    }

    std::string key = path.stem().string();

    textures.emplace(key,tex);
  }

}

const sf::Texture& TextureStorage::getData(const std::string& key) {
  return textures.at(key);
} 

SoundBufferStorage::SoundBufferStorage() {
  const std::filesystem::path sbuf_folder = "../assets/sfx/sounds/";
  for (auto const& entry : std::filesystem::directory_iterator(sbuf_folder)) {
    if(!entry.is_regular_file()) continue; // skip directories and anything else
    auto path = entry.path();
    auto ext = path.extension().string();
    if (ext != ".ogg") continue; // skip non ogg files
    sf::SoundBuffer sbuf;
    if (!sbuf.loadFromFile(path.string())) {
      Logger::log_err("Failed to load " + path.string());
      continue;
    }

    std::string key = path.stem().string();
    snd_bufs.emplace(key, sbuf);
  }
}

const sf::SoundBuffer& SoundBufferStorage::getData(const std::string& key) {
  return snd_bufs.at(key);
} 

PlayerData::PlayerData(TextureStorage& ts, SoundBufferStorage& sbs) 
: sprite(ts.getData("player_tex")),
  snd_hurt(sbs.getData("player_hurt")),
  snd_death(sbs.getData("player_death")),
  snd_fire(sbs.getData("bullet_fire"))
{
  Logger::log_msg("Player data loaded");
}

EnemyData::EnemyData(TextureStorage& ts, SoundBufferStorage& sbs) 
: snd_fairy_death(sbs.getData("fairy_death")),
  snd_lizard_death(sbs.getData("lizard_death"))
{
  sprites.emplace("fairy", ts.getData("fairy_tex")); 
  sprites.emplace("lizard", ts.getData("lizard_tex")); 
  sprites.emplace("skull", ts.getData("skull_tex")); 
  Logger::log_msg("Enemy data loaded");
}

sf::Sprite EnemyData::getSprite(const std::string& key) {
  return sprites.at(key);
}