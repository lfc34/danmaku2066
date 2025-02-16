#include "Player.h"

// to avoid writing sf::Keyboard::Key:Blahblah
using namespace Controls; 

void Player::loadPlayerModel() {
  if(!PlayerTexture.loadFromFile("../assets/gfx/player.png")) {
    std::cerr << "Failed to load player model. Exiting...\n";
    exit(1);
  }
  PlayerSprite.setTexture(PlayerTexture);
}

void Player::placeStartPos() {
  PlayerSprite.setPosition(StartPosX, StartPosY);
  std::clog << "Placed player at start position\n";
}

Player::Player(std::shared_ptr<SoundManager> smg,             
               std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> pv,
               std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies,
               std::shared_ptr<std::vector<std::unique_ptr<Projectile>>> en) {
  SndMgr = smg;
  ProjVec = pv;
  enemy_prj_vec = en;
  enemies_vec = enemies;
  loadPlayerModel();
  PlayerSprite.setOrigin(25, 0);
  placeStartPos();
  is_invuln = false;
}

sf::Rect<float> Player::getPlayerBounds() {
  return PlayerSprite.getGlobalBounds();
}

sf::Vector2f Player::getPlayerPosition() {
  return PlayerSprite.getPosition();
}

void Player::game_over() {
  // hangs the game, will be properly implemented with game pause menu
  std::cout << "Game over!\n";
  sf::Keyboard kb;
  while (true) {
    if(kb.isKeyPressed(sf::Keyboard::Key::Escape))
      exit(0);
  }
}

bool Player::check_collision() {
  for (const auto& enemy : *enemies_vec) {
    if(enemy->getBounds().intersects(getPlayerBounds())) {
      PlayerSprite.setColor(sf::Color::Red);
      return true;
    }
  }

  for (const auto& proj : *enemy_prj_vec) {
    if(proj->getProjBounds().intersects(getPlayerBounds())) {
      PlayerSprite.setColor(sf::Color::Red);
      return true;
    }
  }
  return false;
}

void Player::updatePlayer(sf::RenderWindow& w) {
  w.draw(PlayerSprite);

  if((is_invuln = invuln_clock.getElapsedTime().asMilliseconds() < 1500))
    PlayerSprite.setColor(sf::Color::Blue);
  else  
    PlayerSprite.setColor(sf::Color::White);

  if(check_collision() && !is_invuln) {
    --lives;
    placeStartPos();
    invuln_clock.restart();
  }  

  if (lives <= 0) {
    game_over();
  }

}

bool Player::isMovingOutOfBnds() {
  return getPlayerPosition().x < 25 || getPlayerPosition().y < 0  ||
         getPlayerPosition().x > 775 || getPlayerPosition().y > 535;
}

void Player::move(float x, float y, const float& delta) {
  PlayerSprite.move(x * delta, y * delta);
  if (isMovingOutOfBnds())
    PlayerSprite.move(-x, -y); // trolling
}

void Player::fire() {
  SndMgr->playSound("bullet_shot");
  ProjVec->emplace_back
  (std::make_unique<Bullet>(getPlayerPosition()));
  // std::clog << "Bullet stored in vector\n";
}

int Player::kbInputHandler(sf::Keyboard& kb, const float& delta) {
  // pause the game
  if(kb.isKeyPressed(ESC)) {
    return 1;
  }

  // fire
  if(kb.isKeyPressed(Z_K)) {
    if(PlayerShootTimer.getElapsedTime().asMilliseconds() >= 125) {
      fire();
      PlayerShootTimer.restart();
    }
  }

  // movement vector
  float MoveX = 0.0f;
  float MoveY = 0.0f;
  // vertical movement
  if(kb.isKeyPressed(UP)){
    MoveY -= PlayerSpeed;
  }
  if(kb.isKeyPressed(DOWN)) {
    MoveY += PlayerSpeed;
  }
  // horizontal movement
  if(kb.isKeyPressed(LEFT)) {
    MoveX -= PlayerSpeed;
  }
  if(kb.isKeyPressed(RIGHT)) {
    MoveX += PlayerSpeed;
  }

  // handle diagonal movement
  if(MoveX != 0.0 && MoveY != 0.0) {
    MoveX /= DiagSlowdown;
    MoveY /= DiagSlowdown; 
  }

  //after movement vector[x, y] is calculated, finally move player
  move(MoveX, MoveY, delta);
  return 0;
}