/// PORTED TO SFML3
#include "Player.hpp"

#include <iostream>

#include "Controls.hpp"

// hack to suppress sf::Sprite no default constructor error
const sf::Texture dummy_texture;

void Player::loadPlayerModel() {
  if(!PlayerTexture.loadFromFile("../assets/gfx/player.png")) {
    std::cerr << "Failed to load player model. Exiting...\n";
    exit(1);
  }
  PlayerSprite.setTexture(PlayerTexture, true);
}

void Player::placeStartPos() {
  PlayerSprite.setPosition({StartPosX, StartPosY});
  // std::clog << "Placed player at start position\n";
}

Player::Player(std::vector<std::unique_ptr<Projectile>>& pv,
               std::vector<std::unique_ptr<Enemy>>& enemies,
               std::vector<std::unique_ptr<Projectile>>& en)
: PlayerSprite(dummy_texture) {
  proj_vec_ptr = &pv;
  enemy_prj_vec_ptr = &en;
  enemies_vec_ptr = &enemies;
  loadPlayerModel();
  PlayerSprite.setOrigin({18, 0});
  placeStartPos();
  is_invuln = false;
}

sf::FloatRect Player::getPlayerBounds() {
  return PlayerSprite.getGlobalBounds();
}

sf::Vector2f Player::getPlayerPosition() {
  return PlayerSprite.getPosition();
}

bool Player::check_collision() {
  for (const auto& enemy : *enemies_vec_ptr) {
    if(enemy->getBounds().findIntersection(getPlayerBounds())) {
      return true;
    }
  }

  for (const auto& proj : *enemy_prj_vec_ptr) {
    if(proj->getProjBounds().findIntersection(getPlayerBounds())) {
      return true;
    }
  }
  return false;
}

void Player::updatePlayer(sf::RenderWindow& w, SoundManager& smg) {
  w.draw(PlayerSprite);

  if((is_invuln = invuln_clock.getElapsedTime().asMilliseconds() < 1500))
    PlayerSprite.setColor(sf::Color::Red);
  else  
    PlayerSprite.setColor(sf::Color::White);

  if(check_collision() && !is_invuln) {
    smg.playSound("player_hurt");
    --lives;
    placeStartPos();
    invuln_clock.restart();
  }  

}

bool Player::isMovingOutOfBnds() {
  return getPlayerPosition().x < 15 || getPlayerPosition().y < 15  ||
         getPlayerPosition().x > 785 || getPlayerPosition().y > 535;
}

void Player::move(float x, float y, const float& delta) {
  PlayerSprite.move({x * delta, y * delta});
  if (isMovingOutOfBnds())
    PlayerSprite.move({-x * delta, -y * delta}); // trolling
}

void Player::fire(SoundManager& smg) {
  smg.playSound("bullet_shot");
  proj_vec_ptr->emplace_back
  (std::make_unique<Bullet>(getPlayerPosition()));
  // std::clog << "Bullet stored in vector\n";
}

void Player::input_handler(const float& delta, 
                           SoundManager& smg) {
  using namespace sf::Keyboard;
  using namespace Controls;
  // fire
  if(isKeyPressed(Z_K)) {
    if(PlayerShootTimer.getElapsedTime().asMilliseconds() >= 125) {
      fire(smg);
      PlayerShootTimer.restart();
    }
  }

  // movement vector
  float MoveX = 0.0f;
  float MoveY = 0.0f;
  // vertical movement
  if(isKeyPressed(UP)){
    MoveY -= PlayerSpeed;
  }
  if(isKeyPressed(DOWN)) {
    MoveY += PlayerSpeed;
  }
  // horizontal movement
  if(isKeyPressed(LEFT)) {
    MoveX -= PlayerSpeed;
  }
  if(isKeyPressed(RIGHT)) {
    MoveX += PlayerSpeed;
  }

  // handle diagonal movement
  if(MoveX != 0.0 && MoveY != 0.0) {
    MoveX /= DiagSlowdown;
    MoveY /= DiagSlowdown; 
  }
  
  // slow down player if LShift is pressed
  if(isKeyPressed(SHIFT)) {
    MoveX /= 1.5f;
    MoveY /= 1.5f;
  }

  //after movement vector[x, y] is calculated, finally move player
  move(MoveX, MoveY, delta);
}

Player::~Player() {
  enemies_vec_ptr = nullptr;
  proj_vec_ptr = nullptr;
  enemy_prj_vec_ptr = nullptr;
}