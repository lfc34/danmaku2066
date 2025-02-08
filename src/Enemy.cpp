#include "Enemy.h"

Enemy::~Enemy() {
  std::clog << "Enemy dies\n";
}

MoonStone::MoonStone(sf::Vector2f SpawnPos,
                     std::shared_ptr<std::vector<
                     std::unique_ptr<Projectile>>> Pvec,
                     EnemyPathWay p) {
  ProjVec = Pvec;
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
  for (const auto& i : *ProjVec) {
    if (EnemySprite.getGlobalBounds().intersects(i->getProjBounds()))
      this->hp--;
  }
  if (this->hp == 0)
    die();
  EnemySprite.rotate(10);
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
  // ProjVec->push_back(Pebble(EnemySprite.getPosition()));
}