#include "Enemy.h"

Enemy::~Enemy() {
  // std::clog << "Enemy dies\n";
}

void Enemy::send_to_valhalla(sf::Sprite& sprite) {
  sprite.setPosition(-800, -600);
}

MoonStone::MoonStone(std::vector<std::unique_ptr<Projectile>>& plr_prj_vec, 
                     std::vector<std::unique_ptr<Projectile>>& enm_prj_vec,
                     MovePattern pattern) {
  plr_prj_vec_ptr = &plr_prj_vec;
  enm_prj_vec_ptr = &enm_prj_vec;
	if(!(EnemyTexture.loadFromFile("../assets/gfx/rock.png"))) {
    std::cerr << "Failed to load enemy texture. Exiting...\n";
    exit(1);
  }
  EnemySprite.setTexture(EnemyTexture);
	EnemySprite.setPosition(pattern.spawn_pos);
	EnemySprite.setOrigin(30, 30);
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE;
}

void MoonStone::enemy_move(const float& delta) {
  if(EnemySprite.getPosition().y >= path.mid_point.y)
    current_direction = path.end_point;
  EnemySprite.move(current_direction * FallingSpeed * delta);
}

void MoonStone::updateEnemy(const float& delta) {
  // checks for enemy<->bullet collision
  for(const auto& i : *plr_prj_vec_ptr) {
    if(EnemySprite.getGlobalBounds().intersects(i->getProjBounds()))
      this->hp--;
  }
  if(this->hp == 0)
    state = DEAD;
  if(getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;
  enemyShoot();
  enemy_move(delta);
}

sf::Sprite& MoonStone::getSprite() {
  return EnemySprite;
}

unsigned int& MoonStone::getHp() {
  return this->hp;
}

void MoonStone::enemyShoot() {
  if (ShootTimer.getElapsedTime().asMilliseconds() >= 200) {
    enm_prj_vec_ptr->push_back(std::make_unique<Pebble>
                      (EnemySprite.getPosition()));
    ShootTimer.restart();
  }
}

sf::Rect<float> MoonStone::getBounds() {
  return EnemySprite.getGlobalBounds();
}

MoonStone::~MoonStone() {
  plr_prj_vec_ptr = nullptr;
  enm_prj_vec_ptr = nullptr;
}