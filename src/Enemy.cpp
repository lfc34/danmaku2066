/// PORTED TO SFML3
#include "Enemy.hpp"
#include "AssetManager.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

/////////////////////////////////////////////////////////////////////////////
// ENEMY
Enemy::Enemy(EnemyData &ed, const std::string &name)
    : data(ed), sprite(data.getSprite(name)) {}

void Enemy::enemy_move(const float &delta) {
  if (sprite.getPosition().y >= path.mid_point.y)
    current_direction = path.end_point;
  sprite.move(current_direction * speed * delta);
}

void Enemy::send_to_valhalla() { sprite.setPosition({-800, -600}); }

sf::Sprite &Enemy::getSprite() { return sprite; }

int &Enemy::getHp() { return this->hp; }

sf::Rect<float> Enemy::getBounds() { return sprite.getGlobalBounds(); }

Enemy::~Enemy() {
  plr_prj_vec_ptr = nullptr;
  enm_prj_vec_ptr = nullptr;
}

///////////////////////////////////////////////////////////
// FAIRY
Fairy::Fairy(EnemyPtrs &dt, MovePattern pattern, const sf::Vector2f &spawn_pos,
             EnemyData &ed)
    : Enemy(ed, "fairy") {
  speed = 1.2f;
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
  sprite.setPosition(spawn_pos);
  sprite.setOrigin({30, 30});
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE;
}

void Fairy::increase_score(unsigned int &score) { score += 10; }

void Fairy::updateEnemy(const float &delta) {
  // checks for enemy<->bullet collision
  for (const auto &i : *plr_prj_vec_ptr) {
    if (sprite.getGlobalBounds().findIntersection(i->getProjBounds()))
      this->hp--;
  }
  if (this->hp <= 0) {
    // smg.playSound("fairy_death");
    data.snd_fairy_death.play();
    state = DEAD;
  }
  if (getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;
  enemyShoot();
  enemy_move(delta);
}

void Fairy::enemyShoot() {
  if (ShootTimer.getElapsedTime().asMilliseconds() >= (rand() % 300 + 50)) {
    enm_prj_vec_ptr->push_back(std::make_unique<Pebble>(sprite.getPosition()));
    ShootTimer.restart();
  }
}

//////////////////////////////////////////////////////////////////
// LIZARD KILLER
LizardKiller::LizardKiller(EnemyPtrs &dt, MovePattern pattern,
                           const sf::Vector2f &spawn_pos, EnemyData &ed)
    : Enemy(ed, "lizard") {
  speed = 0.5f;
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
  sprite.setPosition(spawn_pos);
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE;
}

void LizardKiller::increase_score(unsigned int &score) { score += 50; }

void LizardKiller::enemyShoot() {
  sf::Vector2f left_arm((sprite.getPosition().x + 10.0f),
                        (sprite.getPosition().y + 60.0f));
  sf::Vector2f right_arm((sprite.getPosition().x + 75.0f),
                         (sprite.getPosition().y + 60.0f));
  if (ShootTimer.getElapsedTime().asMilliseconds() >= (rand() % 600 + 350)) {
    enm_prj_vec_ptr->push_back(std::make_unique<Fireball>(right_arm, 30));
    enm_prj_vec_ptr->push_back(std::make_unique<Fireball>(left_arm, -30));
    ShootTimer.restart();
  }
}

void LizardKiller::updateEnemy(const float &delta) {
  for (const auto &i : *plr_prj_vec_ptr) {
    if (sprite.getGlobalBounds().findIntersection(i->getProjBounds()))
      this->hp--;
  }
  if (this->hp <= 0) {
    // smg.playSound("lizard_death");
    data.snd_lizard_death.play();
    state = DEAD;
  }
  if (getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;

  enemyShoot();
  enemy_move(delta);
}

//////////////////////////////////////////////////////
// SKULL
Skull::Skull(EnemyPtrs &dt, MovePattern pattern, const sf::Vector2f spawn_pos,
             EnemyData &ed)
    : Enemy(ed, "skull") {
  speed = 1.0f;
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
  sprite.setPosition(spawn_pos);
  path = pattern;
  current_direction = path.mid_point;
  state = ALIVE;
}

void Skull::increase_score(unsigned int &score) { score += 15; }

void Skull::enemyShoot() {
  if (ShootTimer.getElapsedTime().asMilliseconds() >= (rand() % 200 + 100)) {
    enm_prj_vec_ptr->push_back(
        std::make_unique<Flameshard>(sprite.getPosition()));
    ShootTimer.restart();
  }
}

void Skull::updateEnemy(const float &delta) {
  for (const auto &i : *plr_prj_vec_ptr) {
    if (sprite.getGlobalBounds().findIntersection(i->getProjBounds()))
      this->hp--;
  }
  if (this->hp <= 0) {
    // smg.playSound("lizard_death");
    data.snd_lizard_death.play();
    state = DEAD;
  }
  if (getSprite().getPosition().y > 650 || getSprite().getPosition().y < -50)
    state = FLEW_AWAY;

  sprite.rotate(sf::degrees(10));
  enemyShoot();
  enemy_move(delta);
}

////////////////////////////////////////////////////////
// BOSS

Boss::Boss(EnemyPtrs &dt, TextureStorage &ts)
    : m_ts(ts), boss_sprite(m_ts.getData("boss_idle_tex")) {
  plr_prj_vec_ptr = dt.plr_prj_vec_ptr;
  enm_prj_vec_ptr = dt.enm_prj_vec_ptr;
  boss_idle = m_ts.getData("boss_idle_tex");
  boss_flying = m_ts.getData("boss_fly_tex");
  boss_shooting = m_ts.getData("boss_atk_tex");
  boss_sprite.setTexture(boss_idle, true);
  // let him sit out of screen until trigger() is called
  boss_sprite.setPosition({350, -250});
}

sf::Sprite &Boss::get_sprite() { return boss_sprite; }

void Boss::trigger() {
  shoot_timer.restart();
  boss_sprite.setPosition({350, 45});
  triggered = true;
}

void Boss::move(const float &delta, const float x_offset) {
  if (state == Flying) {
    boss_sprite.setTexture(boss_flying, true);
    boss_sprite.move({x_offset * delta, 0});
  }
}

void Boss::boss_shoot() {
  boss_sprite.setTexture(boss_shooting, true);
  switch (state) {
  case Shooting_PH1:
    if (shoot_timer.getElapsedTime().asMilliseconds() > 100) {
      for (int i = 800; i > 0; i -= 100) {
        enm_prj_vec_ptr->emplace_back(std::make_unique<Flameshard>(
            sf::Vector2f(float(rand() % i + 50), 0)));

        shoot_timer.restart();
      }
    }
    break;

  case Shooting_PH2:
    if (shoot_timer.getElapsedTime().asMilliseconds() > 450) {
      for (int i = 800; i > 0; i -= 150) {
        enm_prj_vec_ptr->emplace_back(std::make_unique<Fireball>(
            sf::Vector2f(float(i), 0), float(rand() % 100)));
        shoot_timer.restart();
      }
    }
    break;

  case Shooting_PH3:
    if (shoot_timer.getElapsedTime().asMilliseconds() > 200) {
      for (int i = 800; i > 0; i -= 70) {
        enm_prj_vec_ptr->emplace_back(
            std::make_unique<Pebble>(sf::Vector2f(float(rand() % i + 300), 0)));
        shoot_timer.restart();
      }
    }
  }
}

int Boss::update_boss(const float &delta, float &phase_timer) {
  boss_sprite.setTexture(boss_idle, true);
  for (const auto &i : *plr_prj_vec_ptr) {
    if (boss_sprite.getGlobalBounds().findIntersection(i->getProjBounds()))
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