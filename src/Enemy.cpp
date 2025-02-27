#include "Enemy.h"
#include "Projectile.h"
#include "Sound.h"

/////////////////////////////////////////////////////////////////////////////
// ENEMY
void Enemy::enemy_move(const float& delta) {
  if(EnemySprite.getPosition().y >= path.mid_point.y)
    current_direction = path.end_point;
  EnemySprite.move(current_direction * speed * delta);
}

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

///////////////////////////////////////////////////////////
// FAIRY
Fairy::Fairy(EnemyData& dt, MovePattern pattern, 
                     const sf::Vector2f& spawn_pos) 
{
  speed = 1.2f;
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

void Fairy::increase_score(unsigned int& score) {
  score += 10;
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

//////////////////////////////////////////////////////////////////
// LIZARD KILLER
LizardKiller::LizardKiller(EnemyData& dt, MovePattern pattern, 
                           const sf::Vector2f& spawn_pos)
{
  speed = 0.5f;
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

void LizardKiller::increase_score(unsigned int& score) {
  score += 50;
}

void LizardKiller::enemyShoot() {
  sf::Vector2f left_arm ((EnemySprite.getPosition().x + 10.0f),
                         (EnemySprite.getPosition().y + 60.0f));
  sf::Vector2f right_arm ((EnemySprite.getPosition().x + 75.0f),
                          (EnemySprite.getPosition().y + 60.0f));
  if (ShootTimer.getElapsedTime().asMilliseconds() >= 600) {
    enm_prj_vec_ptr->push_back(std::make_unique<Fireball>
                      (right_arm, 30));
    enm_prj_vec_ptr->push_back(std::make_unique<Fireball>
                      (left_arm, -30));
    ShootTimer.restart();
  }
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

//////////////////////////////////////////////////////
// SKULL
Skull::Skull(EnemyData& dt, MovePattern pattern,
             const sf::Vector2f spawn_pos) 
{
  speed = 1.0f;
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
	if(!(EnemyTexture.loadFromFile("../assets/gfx/skull.png"))) {
    std::cerr << "Failed to load enemy texture. Exiting...\n";
    exit(1);
  }
  EnemySprite.setTexture(EnemyTexture);
	EnemySprite.setPosition(spawn_pos);
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE; 
}

void Skull::increase_score(unsigned int& score) {
  score += 15;
}

void Skull::enemyShoot() {
 if (ShootTimer.getElapsedTime().asMilliseconds() >= 200) {
    enm_prj_vec_ptr->push_back(std::make_unique<Flameshard>
                      (EnemySprite.getPosition()));
    ShootTimer.restart();
  } 
}

void Skull::updateEnemy(const float& delta, SoundManager& smg) {
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

  EnemySprite.rotate(10);
  enemyShoot();
  enemy_move(delta);
}