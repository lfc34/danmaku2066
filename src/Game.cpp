#include "Game.h"
#include "Enemy.h"

Game::Game() : 
window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Muzhik 2066") {
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
  window.setMouseCursorVisible(false);
  std::clog << "Window initialised\n";
  std::clog << "Game instance created\n";
}

// TODO: check .h file
Game::~Game() {
  std::clog << "Game finished succesfully (i hope so)\n";
}

void Game::menuLoop() {   
  Menu menu;
  while(window.isOpen() && !(menu.GameStarted)) {
    window.clear();
    menu.drawMenu(window);
    menu.buttonSelector(window, kb);
    window.display();
  }
  window.clear();
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
  sf::Clock waveTimer;
  sf::Clock BossTimer;

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
  EnemyPathWay wave1 = {
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
    if (kb.isKeyPressed(sf::Keyboard::Key::Escape))
      window.close();

    window.clear();

    lvl1.drawLevel(window); // must be first in draw order

    /* collision logic
      if enemy rectangle intersects any kind
      of bullet object rectangle, they hp goes down */
    // ENEMY WAVES PART //
    if (enemyCntWave.at(currentWave) > 0 
       && waveTimer.getElapsedTime().asSeconds() > 0.2) {

      enemy_vec->emplace_back(std::make_unique<MoonStone>
              (sf::Vector2f(0,0), ProjVec, enemy_proj_vec,
              directions.at(currentWave)));

      --enemyCntWave.at(currentWave);
      waveTimer.restart();
    }

    for (auto& i : *enemy_vec) {
      i->updateEnemy();
      window.draw(i->getSprite());
    }
    // ENEMY WAVES PART //

    // TODO: write a prjUpdate() in game method for scaleability
    for (auto& i : *ProjVec) {
      if (i->isFlewAway() && !(ProjVec->empty())) {
        ProjVec->erase(ProjVec->begin());
        ProjVec->shrink_to_fit();
        break; // it's just works
      }
      else {
        i->update();
        window.draw(i->getShape());
      }
    }
    
    // TODO: VERY buggy, because enemy projectiles that are flew away
    // delete ones who are not. 
    // Possible solution: after projectile flew away make them transparent (xd)
    // this will not unload them from memory completely (like player bullets do)
    // but at least will save some video mem
    for (auto& i : *enemy_proj_vec) {
      // if (i->isFlewAway() && !(enemy_proj_vec->empty())) {
      //   enemy_proj_vec->erase(enemy_proj_vec->begin());
      //   enemy_proj_vec->shrink_to_fit();
      //   break; // it's just works
      // }
      // else {
      i->update();
      window.draw(i->getShape());
      // }
    }
     
    // TODO: change order for readability
    player->kbInputHandler(kb);
    player->updatePlayer(window);

    // draw what have been rendered so far
    window.display();                              
  }
}