#include "Enemy.h"

Enemy::~Enemy() {
  std::clog << "Enemy dies\n";
}

MoonStone::MoonStone(sf::Vector2f SpawnPos,
                     std::shared_ptr<std::vector<
                     std::unique_ptr<Projectile>>> Pvec) {
  ProjVec = Pvec;
	if(!(EnemyTexture.loadFromFile("../assets/gfx/rock.png"))) {
    std::cerr << "Failed to load enemy texture. Exiting...\n";
    exit(1);
  }
  EnemySprite.setTexture(EnemyTexture);
	EnemySprite.setPosition(SpawnPos);
	EnemySprite.setOrigin(30, 30);
  std::clog << "Enemy spawned\n";
}

void MoonStone::updateEnemy(const sf::Vector2f& direction) {
  EnemySprite.rotate(10);
  // TODO: implement correct logic (see enemy_move_logic diagram)
  sf::Vector2f current_direction = direction;
  if (EnemySprite.getPosition().y >= 300)
    current_direction.y = (0 - current_direction.y) - current_direction.y; 
  EnemySprite.move(FallingSpeed * current_direction);
}

const sf::Sprite& MoonStone::getSprite() {
  return EnemySprite;
}

void MoonStone::enemyShoot() {
  // ProjVec->push_back(Pebble(EnemySprite.getPosition()));
}