#include "Enemy.h"

void load_texture(sf::Texture& texture, const char* path) {
  if (!texture.loadFromFile(path)) {
    std::cerr << "Failed to load " << path << ". Exiting...\n";
    exit(1);
  }
}

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

int& Enemy::getHp() {
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
  if(this->hp <= 0) {
    smg.playSound("fairy_death");
    state = DEAD;
  }
  if(getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;
  enemyShoot();
  enemy_move(delta);
}

void Fairy::enemyShoot() {
  if (ShootTimer.getElapsedTime().asMilliseconds() >= (rand() % 300 + 50)) {
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
  if (ShootTimer.getElapsedTime().asMilliseconds() >= (rand() % 600 + 350)) {
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
  if(this->hp <= 0) {
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
 if (ShootTimer.getElapsedTime().asMilliseconds() >= (rand() % 200 + 100)) {
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
  if(this->hp <= 0) {
    smg.playSound("lizard_death");
    state = DEAD;
  }
  if(getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;

  EnemySprite.rotate(10);
  enemyShoot();
  enemy_move(delta);
}

////////////////////////////////////////////////////////
// BOSS

Boss::Boss(EnemyData& dt) {
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
  load_texture(boss_idle, "../assets/gfx/b_idle.png");
  load_texture(boss_flying, "../assets/gfx/b_fly.png");
  load_texture(boss_shooting, "../assets/gfx/b_atk.png");
  boss_sprite.setTexture(boss_idle);
  
  // let him sit out of screen until trigger() is called
  boss_sprite.setPosition(350, -250);
}

sf::Sprite& Boss::get_sprite() {
  return boss_sprite;
}

void Boss::trigger() {
  shoot_timer.restart();
  boss_sprite.setPosition(350, 45);
  triggered = true;
}

void Boss::move(const float& delta, const float x_offset) {
  if (state == Flying) {
    boss_sprite.setTexture(boss_flying);
    boss_sprite.move(x_offset * delta, 0);
  }
}

void Boss::boss_shoot() {
  boss_sprite.setTexture(boss_shooting);
  switch (state) {
    case Shooting_PH1:
      if (shoot_timer.getElapsedTime().asMilliseconds() > 100) {
        for (int i = 800; i > 0; i-=100) {
          enm_prj_vec_ptr->emplace_back(std::make_unique<Flameshard>
                                       (sf::Vector2f(float(rand()%i+50),0)));
          
          shoot_timer.restart();
        }
      }
      break;

    case Shooting_PH2:
      if (shoot_timer.getElapsedTime().asMilliseconds() > 450) {
        for (int i = 800; i > 0; i-=150) {
          enm_prj_vec_ptr->emplace_back(std::make_unique<Fireball>
                                       (sf::Vector2f(float(i),0), float(rand()%100)));
          shoot_timer.restart();
        }
      }
      break;

    case Shooting_PH3:
      if (shoot_timer.getElapsedTime().asMilliseconds() > 200) {
        for (int i = 800; i > 0; i-=70) {
          enm_prj_vec_ptr->emplace_back(std::make_unique<Pebble>
                                       (sf::Vector2f(float(rand()%i+300), 0)));
          shoot_timer.restart();
        }
      }
  }
}

int Boss::update_boss(const float& delta, float& phase_timer) {
  boss_sprite.setTexture(boss_idle);
  for(const auto& i : *plr_prj_vec_ptr) {
    if(boss_sprite.getGlobalBounds().intersects(i->getProjBounds()))
      this->hp--;
  }

  if (!(state == Flying))
    boss_shoot();

  if (hp <= 0) {
    state = Dying;
    return 1;
  }
  
  if (phase_timer > 52) {
    state = Shooting_PH1;
  }
  if (phase_timer > 54 && move_stage == 0) {
    state = Flying;
    if (boss_sprite.getPosition().x > 100)
      move(delta, -200);
    else if (boss_sprite.getPosition().x <= 100)
      ++move_stage;
  }
  if (phase_timer > 56) {
    state = Shooting_PH2;
  }
  if (phase_timer > 60 && move_stage == 1) {
    state = Flying;
    if (boss_sprite.getPosition().x < 600)
      move(delta, 200);
    else if (boss_sprite.getPosition().x >= 600)
      ++move_stage;
  }
  if (phase_timer > 63) {
    state = Shooting_PH3;
  }
  if (phase_timer > 67 && move_stage == 2) {
    state = Flying;
    if (boss_sprite.getPosition().x > 350)
      move(delta, -200);
    else if (boss_sprite.getPosition().x <= 300)
      ++move_stage;
  }
  if (phase_timer > 69) {
    state = Shooting_PH1;
  }
  if (phase_timer > 73) {
    phase_timer = 51;
  }
  return 0;
}