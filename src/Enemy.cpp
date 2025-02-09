#include "Enemy.h"

Enemy::~Enemy() {
  std::clog << "Enemy dies\n";
}

// wow, that's quite big. Impressive
MoonStone::MoonStone(sf::Vector2f SpawnPos,
                     std::shared_ptr<std::vector<
                     std::unique_ptr<Projectile>>> Pvec,
                     std::shared_ptr<std::vector<
                     std::unique_ptr<Projectile>>> OwnVec,
                     EnemyPathWay p) {
  player_pr_vec = Pvec;
  enemy_pr_vec = OwnVec;
	if(!(EnemyTexture.loadFromFile("../assets/gfx/rock.png"))) {
    std::cerr << "Failed to load enemy texture. Exiting...\n";
    exit(1);
  }
  EnemySprite.setTexture(EnemyTexture);
	EnemySprite.setPosition(SpawnPos);
	EnemySprite.setOrigin(30, 30);
  path = p;
  current_direction = path.mid_point;
  std::clog << "Enemy spawned\n";
}

void MoonStone::updateEnemy() {
  // checks for enemy<->bullet collision
  for (const auto& i : *player_pr_vec) {
    if (EnemySprite.getGlobalBounds().intersects(i->getProjBounds()))
      this->hp--;
  }
  if (this->hp == 0)
    die();
  EnemySprite.rotate(10);
  enemyShoot();
  if (EnemySprite.getPosition() == path.mid_point)
    current_direction = path.end_point;
  EnemySprite.move(FallingSpeed * current_direction);
}

const sf::Sprite& MoonStone::getSprite() {
  return EnemySprite;
}

unsigned int& MoonStone::getHp() {
  return this->hp;
}

void MoonStone::die() {
  EnemySprite.setColor(sf::Color::Transparent);
}

void MoonStone::enemyShoot() {
  if (ShootTimer.getElapsedTime().asMilliseconds() >= 200) {
    enemy_pr_vec->push_back(std::make_unique<Pebble>
                      (EnemySprite.getPosition()));
    ShootTimer.restart();
  }
}

sf::Rect<float> MoonStone::getBounds() {
  return EnemySprite.getGlobalBounds();
}