#include "Game.h"

Game::Game() : 
window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Muzhik 2066") {
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
  window.setMouseCursorVisible(false);
  std::clog << "Window initialised\n";
  std::clog << "Game instance created\n";
}

void Game::game_pause() {
  constexpr bool Continue {1};
  PauseMenu pm;
  while (window.isOpen()) {
    pm.draw_menu(window);
    if(pm.menu_loop(kb) == Continue) 
      break;
  }
}

Game::~Game() {
  std::clog << "Game finished succesfully (i hope so)\n";
}

int Game::menuLoop() {   
  Menu menu;
  while(window.isOpen() && !(menu.GameStarted)) {
    window.clear();
    menu.drawMenu(window);
    menu.buttonSelector(window, kb);
    window.display();
  }
  window.clear();
  if (menu.GameStarted)
    return 0;
  else  
    return 1;
}

void Game::lvl1Loop() {
  sf::Event event;

  // Sound manager to share across classes that need it
  std::shared_ptr SndMgr = std::make_shared<SoundManager>();

  // Projectile vector made shared for other classes that need it
  std::shared_ptr ProjVec = 
  std::make_shared<std::vector
  <std::unique_ptr<Projectile>>>();

  std::shared_ptr enemy_proj_vec =
  std::make_shared<std::vector<std::unique_ptr<Projectile>>>();

  std::shared_ptr enemy_vec = std::make_shared<std::vector<std::unique_ptr<Enemy>>>();

  // player
  std::unique_ptr player = std::make_unique<Player>(SndMgr, ProjVec,
                           enemy_vec, enemy_proj_vec);

  // timers
  float delta;
  sf::Clock frame_clock;
  sf::Clock waveTimer;
  sf::Clock enemy_spawn_timer;
  sf::Clock BossTimer;
  sf::Clock button_timer;

  // RNG part. Let it be here, maybe I'll need it later.
  // for now enemies will spawn in waves in predefined pattern
  // std::random_device rd;
  // std::mt19937 rng(rd()); 
  // std::uniform_int_distribution<std::mt19937::result_type> rand_spwn_pos(25, 775);

  Level lvl1("../assets/sfx/lvl1.ogg",
          "../assets/gfx/bg.jpg");

  lvl1.playMusic();
  
  //=========================WAVE SPECIFIC DATA==============================//
  // Wave index
  unsigned int currentWave {0};

  // Number of enemies for every wave
  std::vector<unsigned int> enemyCntWave = {10, 10};

  // Wrong. TODO: replace as a struct
  // Try to implement 3-point system
  std::vector<EnemyPathWay> directions;
  // there is more points to add, !!!REMEMBER!!!
  EnemyPathWay wave1 = {
    sf::Vector2f (0, 0),
    sf::Vector2f (200, 200),
    sf::Vector2f (400, -200)
  };
  directions.push_back(wave1);

  //=========================WAVE SPECIFIC DATA==============================//

  while(window.isOpen()) {
    // allows us to close window via cross on the panel
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
        break;
      }
    }

    // zaebalo
    if (kb.isKeyPressed(sf::Keyboard::Key::X))
      window.close();

    window.clear();

    lvl1.drawLevel(window); // must be first in draw order

    // ENEMY WAVES PART //
    if (enemyCntWave.at(currentWave) > 0 
       && waveTimer.getElapsedTime().asSeconds() > 3) {

      if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 200) {     
        enemy_vec->emplace_back(std::make_unique<MoonStone>
                (wave1.spawn_pos, ProjVec, enemy_proj_vec,
                directions.at(currentWave)));

        --enemyCntWave.at(currentWave);
        enemy_spawn_timer.restart();
      }
      // !!!Don't forget out of bounds check
      // if (enemyCntWave.at(currentWave) <= 0)
        // ++currentWave;
    }

    for (auto& enemy : *enemy_vec) {
      if(!(enemy->is_dead)) {
        enemy->updateEnemy();
        window.draw(enemy->getSprite());
      } 
    }
    // ENEMY WAVES PART //

    for (auto& plr_proj : *ProjVec) {
      if (plr_proj->isFlewAway() && !(ProjVec->empty())) {
        ProjVec->erase(ProjVec->begin());
        ProjVec->shrink_to_fit();
        break; // it's just works
      }
      else {
        plr_proj->update(delta);
        window.draw(plr_proj->getShape());
      }
    }
    
    for (auto& enm_proj : *enemy_proj_vec) {
      if(!enm_proj->isFlewAway()) {
        enm_proj->update(delta);
        window.draw(enm_proj->getShape());
      }
    }
     
    // func returns 1 only if ESC was pressed
    if(player->kbInputHandler(kb, delta) == 1) { 
      lvl1.stop_music();
      game_pause();
      lvl1.playMusic();
    }

    player->updatePlayer(window);

    // draw what have been rendered so far
    window.display();                              
    delta = (float)frame_clock.restart().asSeconds();
  }
}