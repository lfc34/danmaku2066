/// PORTED TO SFML3
#include "Player.hpp"
#include "GameState.hpp"
#include "Logger.hpp"

Player::Player(std::vector<std::unique_ptr<Projectile>> &pv,
               std::vector<std::unique_ptr<Enemy>> &enemies,
               std::vector<std::unique_ptr<Projectile>> &en,
               PlayerData& pdata)
: GAME_STATE(GameState::getInstance()),
  m_data(pdata)
{
  m_projVecPtr = &pv;
  m_enemyPrjVecPtr = &en;
  m_enemyVecPtr = &enemies;
  placeStartPos();
}

void Player::placeStartPos() {
  m_data.sprite.setPosition(m_startPos);
  Logger::log_clr("Placed player at start position");
}

sf::FloatRect Player::getPlayerBounds() {
  return m_data.sprite.getGlobalBounds();
}

sf::Vector2f Player::getPlayerPosition() {
  return m_data.sprite.getPosition();
}

bool Player::check_collision() {
  // with enemies
  for (const auto &enemy : *m_enemyVecPtr) {
    if (enemy->getBounds().findIntersection(getPlayerBounds())) {
      return true;
    }
  }

  // with enemy bullets
  for (const auto &proj : *m_enemyPrjVecPtr) {
    if (proj->getProjBounds().findIntersection(getPlayerBounds())) {
      return true;
    }
  }
  return false;
}

void Player::updatePlayer(sf::RenderWindow &w) {
  w.draw(m_data.sprite);

  if ((m_isInvuln = invuln_clock.getElapsedTime().asMilliseconds() < 1500))
    m_data.sprite.setColor(sf::Color::Red);
  else
    m_data.sprite.setColor(sf::Color::White);

  if (check_collision() && !m_isInvuln) {
    if (GAME_STATE.isMuted() == false)
      m_data.snd_hurt.play();
    --lives;
    placeStartPos();
    invuln_clock.restart();
  }
}

bool Player::isMovingOutOfBnds() {
  sf::Vector2f pos = getPlayerPosition();
  return pos.x < 15 || pos.y < 15 || pos.x > 785 || pos.y > 535;
}

void Player::move(float x, float y, const float &delta) {
  m_data.sprite.move({x * delta, y * delta});
  if (isMovingOutOfBnds())
    m_data.sprite.move({-x * delta, -y * delta}); // trolling
}

void Player::fire() {
  if (!GAME_STATE.isMuted()) m_data.snd_fire.play();
  m_projVecPtr->emplace_back(std::make_unique<Bullet>(getPlayerPosition()));
}

void Player::input_handler(const float &delta) {
  using namespace sf::Keyboard;
  // fire
  if (isKeyPressed(Key::Z)) {
    if (PlayerShootTimer.getElapsedTime().asMilliseconds() >= 125) {
      fire();
      PlayerShootTimer.restart();
    }
  }

  // movement vector
  float MoveX = 0.0f;
  float MoveY = 0.0f;
  // vertical movement
  if (isKeyPressed(Key::Up)) {
    MoveY -= m_playerSpeed;
  }
  if (isKeyPressed(Key::Down)) {
    MoveY += m_playerSpeed;
  }
  // horizontal movement
  if (isKeyPressed(Key::Left)) {
    MoveX -= m_playerSpeed;
  }
  if (isKeyPressed(Key::Right)) {
    MoveX += m_playerSpeed;
  }

  // handle diagonal movement
  if (MoveX != 0.0 && MoveY != 0.0) {
    MoveX /= m_diagSlowdown;
    MoveY /= m_diagSlowdown;
  }

  // slow down player if LShift is pressed
  if (isKeyPressed(Key::LShift)) {
    MoveX /= 1.5f;
    MoveY /= 1.5f;
  }

  // after movement vector[x, y] is calculated, finally move player
  move(MoveX, MoveY, delta);
}

Player::~Player() {
  m_enemyVecPtr = nullptr;
  m_projVecPtr = nullptr;
  m_enemyPrjVecPtr = nullptr;
}