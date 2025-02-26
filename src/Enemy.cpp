#include "Enemy.h"
#include "Sound.h"

void Enemy::send_to_valhalla(sf::Sprite& sprite) {
  sprite.setPosition(-800, -600);
}

sf::Sprite& Enemy::getSprite() {
  return EnemySprite;
}

unsigned int& Enemy::getHp() {
  return this->hp;
}

sf::Rect<float> Enemy::getBounds() {
  return EnemySprite.getGlobalBounds();
}

Enemy::~Enemy() {
  plr_prj_vec_ptr = nullptr;
  enm_prj_vec_ptr = nullptr;
}

Fairy::Fairy(EnemyData& dt, MovePattern pattern, 
                     const sf::Vector2f& spawn_pos) 
{
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
	if(!(EnemyTexture.loadFromFile("../assets/gfx/fairy.png"))) {
    std::cerr << "Failed to load enemy texture. Exiting...\n";
    exit(1);
  }
  EnemySprite.setTexture(EnemyTexture);
	EnemySprite.setPosition(spawn_pos);
	EnemySprite.setOrigin(30, 30);
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE;
}

void Fairy::enemy_move(const float& delta) {
  if(EnemySprite.getPosition().y >= path.mid_point.y)
    current_direction = path.end_point;
  EnemySprite.move(current_direction * FallingSpeed * delta);
}

void Fairy::updateEnemy(const float& delta, SoundManager& smg) {
  // checks for enemy<->bullet collision
  for(const auto& i : *plr_prj_vec_ptr) {
    if(EnemySprite.getGlobalBounds().intersects(i->getProjBounds()))
      this->hp--;
  }
  if(this->hp == 0) {
    smg.playSound("fairy_death");
    state = DEAD;
  }
  if(getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;
  enemyShoot();
  enemy_move(delta);
}


void Fairy::enemyShoot() {
  if (ShootTimer.getElapsedTime().asMilliseconds() >= 200) {
    enm_prj_vec_ptr->push_back(std::make_unique<Pebble>
                      (EnemySprite.getPosition()));
    ShootTimer.restart();
  }
}

LizardKiller::LizardKiller(EnemyData& dt, MovePattern pattern, 
                           const sf::Vector2f& spawn_pos)
{
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
	if(!(EnemyTexture.loadFromFile("../assets/gfx/lizard.png"))) {
    std::cerr << "Failed to load enemy texture. Exiting...\n";
    exit(1);
  }
  EnemySprite.setTexture(EnemyTexture);
	EnemySprite.setPosition(spawn_pos);
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE; 
}

void LizardKiller::enemyShoot() {
  sf::Vector2f left_arm ((EnemySprite.getPosition().x + 10.0f),
                         (EnemySprite.getPosition().y + 60.0f));
  sf::Vector2f right_arm ((EnemySprite.getPosition().x + 75.0f),
                          (EnemySprite.getPosition().y + 60.0f));
  if (ShootTimer.getElapsedTime().asMilliseconds() >= 500) {
    enm_prj_vec_ptr->push_back(std::make_unique<Fireball>
                      (right_arm, 30));
    enm_prj_vec_ptr->push_back(std::make_unique<Fireball>
                      (left_arm, -30));
    ShootTimer.restart();
  }
}

void LizardKiller::enemy_move(const float& delta) {
  if(EnemySprite.getPosition().y >= path.mid_point.y)
    current_direction = path.end_point;
  EnemySprite.move(current_direction * speed * delta);
}

void LizardKiller::updateEnemy(const float& delta, SoundManager& smg) {
  for(const auto& i : *plr_prj_vec_ptr) {
    if(EnemySprite.getGlobalBounds().intersects(i->getProjBounds()))
      this->hp--;
  }
  if(this->hp == 0) {
    smg.playSound("lizard_death");
    state = DEAD;
  }
  if(getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;

  enemyShoot();
  enemy_move(delta);
}