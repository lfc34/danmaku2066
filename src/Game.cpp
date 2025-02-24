#include "Game.h"

Game::Game() : 
window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Muzhik 2066") {
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
  window.setMouseCursorVisible(false);
  std::clog << "Window initialised\n";
  std::clog << "Sound manager initialized\n";
  std::clog << "Game instance created\n";
}

int Game::menuLoop() {   
  Menu menu;
  while(window.isOpen() && !(menu.GameStarted)) {
    menu.drawMenu(window);
    switch (menu.menu_loop(window)) {
      case menu.MUTE_AUDIO:
        IS_GAME_MUTED = true;
        SndMgr.is_muted = true;
        break;

      case menu.UNMUTE_AUDIO:
        IS_GAME_MUTED = false;
        SndMgr.is_muted = false;
        break;

      case menu.QUIT:
        return -1;
        break;

      case menu.START_GAME:
        return 0;
    }
  }
  window.clear();
  return -11;
}

void Game::game_pause() {
  constexpr bool Continue {1};
  PauseMenu pm;
  while (window.isOpen()) {
    pm.draw_menu(window);
    if(pm.menu_loop(window) == Continue) {
      break;
    }
  }
}

// better be refactored into another class
void Game::game_over(sf::Font& font, unsigned int& score) {
  using namespace Controls;
  
  sf::Text game_over("Game over!", font, 50);  
  game_over.setPosition(260, 120);

  sf::Text restart_btn("Restart", font, 35);
  restart_btn.setPosition(330, 300);
  restart_btn.setFillColor(sf::Color::Green);

  sf::Text quit_game_btn("Quit game", font, 35);
  quit_game_btn.setPosition(310, 340);

  sf::Text final_score("Score: " + std::to_string(score), font, 40);
  final_score.setPosition(310, 180);
  final_score.setFillColor(sf::Color::Red);

  std::vector<sf::Text*> options_list {&restart_btn, &quit_game_btn};

  enum Options {
    Restart,
    Quit
  };

  size_t option = Restart;
  sf::Event event;
  while(window.isOpen()) {
    window.clear();
    while(window.pollEvent(event)) {
      if (event.type == sf::Event::KeyReleased && event.key.code == UP) {
        options_list.at(option)->setFillColor(sf::Color::White);
        if (option == Restart)
          option = Quit;
        else
          --option;
        options_list.at(option)->setFillColor(sf::Color::Green);
      } else if (event.type == sf::Event::KeyReleased && event.key.code == DOWN) {
        options_list.at(option)->setFillColor(sf::Color::White);
        ++option;
        if (option > Quit)
          option = Restart;
        options_list.at(option)->setFillColor(sf::Color::Green);
      }
      switch (option) {
        case Restart:
          if (event.type == sf::Event::KeyReleased && event.key.code == Z_K)
            lvl1Loop();
          break;

        case Quit:
          if (event.type == sf::Event::KeyReleased && event.key.code == Z_K) 
            exit(1);
          break;
      }
    }
    window.draw(game_over);
    window.draw(final_score);
    window.draw(restart_btn);
    window.draw(quit_game_btn);
    window.display();
  }
}


void Game::lvl1Loop() {
  // TODO: refactor into own class or func
  // UI for level
  sf::Font UI_font;
  // load UI font
  if(!UI_font.loadFromFile("../assets/gfx/dynapuff.ttf")) {
    std::cerr << "Failed to load assets/gfx/dynapuff.tff. Exiting...\n";
    exit(1);
  }
  unsigned int lvl_score {};
  sf::Text player_score(std::to_string(lvl_score), UI_font, 19);
  player_score.setPosition(640, 570);
  sf::Text player_lives;
  player_lives.setFont(UI_font);
  player_lives.setCharacterSize(19);
  player_lives.setPosition(640, 550);

  sf::Event event;

  // vectors to store projectiles and entities
  std::vector<std::unique_ptr<Projectile>> ProjVec {};
  std::vector<std::unique_ptr<Projectile>> enemy_proj_vec;
  std::vector<std::unique_ptr<Enemy>> enemy_vec; 

  // player
  std::unique_ptr player = std::make_unique<Player>(ProjVec,
                           enemy_vec, enemy_proj_vec);

  // timers
  float delta {};
  /** Clock that counts time duration of one frame (delta time) */
  sf::Clock frame_clock;
  /** Clock that controls when wave appears (can be used for boss fight???)*/
  sf::Clock waveTimer;
  /** Clock that controls how often enemies in wave spawn */
  sf::Clock enemy_spawn_timer;
  sf::Clock BossTimer;

  // RNG part. Let it be here, maybe I'll need it later.
  // for now enemies will spawn in waves in predefined pattern
  // std::random_device rd;
  // std::mt19937 rng(rd()); 
  // std::uniform_int_distribution<std::mt19937::result_type> rand_spwn_pos(25, 775);

  Level lvl1("../assets/sfx/lvl1.ogg",
          "../assets/gfx/bg.jpg");

  lvl1.playMusic(IS_GAME_MUTED);
  
  //=========================WAVE SPECIFIC DATA==============================//
  // Wave index
  [[maybe_unused]]int currentWave {0};

  // Number of enemies for every wave
  int enemies_in_wave[2] = {10, 10};
  std::vector<MovePattern> directions;
  MovePattern wave1 {}; 
  directions.push_back(wave1);
  MovePattern wave2 {
    sf::Vector2f (800, 0),
    sf::Vector2f (-200, 200),
    sf::Vector2f (-400, -100)
  };
  //=========================WAVE SPECIFIC DATA==============================//

  // ====== GAME LOOP ====== //
  while(window.isOpen()) {
    //clear window at the start of each frame
    window.clear();
    
    lvl1.drawLevel(window); 

    // player update + input handler
    player->input_handler(kb, delta, SndMgr);
    player->updatePlayer(window);

    // UI draw part
    player_lives.setString("Lives:" + std::to_string(player->lives));
    player_score.setString("Score:" + std::to_string(lvl_score));
    window.draw(player_score);
    window.draw(player_lives);
    
    // event processor
    while(window.pollEvent(event)) {
      // closing window via cross on the panel
      if(event.type == sf::Event::Closed) {
        window.close();
        break;
      }
      
      // game pause
      if(event.type == sf::Event::KeyReleased 
        && event.key.code == Controls::ESC) { 
        lvl1.stop_music();
        frame_clock.restart();
        game_pause();
        lvl1.playMusic(IS_GAME_MUTED);
        
        // restart all timers after game continued
        // TODO: fix bug when you can delay enemy waves
        waveTimer.restart();
        frame_clock.restart();
        enemy_spawn_timer.restart();
      }
    }

    if (player->lives <= 0) {
      std::clog << "Player died\n";
      lvl1.stop_music();
      game_over(UI_font, lvl_score);
    }
    
    
    // ====== ENEMY WAVES PART ====== //
    // wave 1
    if (waveTimer.getElapsedTime().asSeconds() > 3) {
      for (int i = enemies_in_wave[0]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 200) {
          enemy_vec.emplace_back(std::make_unique<MoonStone>
                                (ProjVec, enemy_proj_vec, wave1));    
          --enemies_in_wave[0];
          enemy_spawn_timer.restart();
        }
      }
    }

    // wave 2
    if (waveTimer.getElapsedTime().asSeconds() > 8) {
      for (int i = enemies_in_wave[1]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 200) {
          enemy_vec.emplace_back(std::make_unique<MoonStone>
                                (ProjVec, enemy_proj_vec, wave2));
          --enemies_in_wave[1];
          enemy_spawn_timer.restart();
        }
      }
    }
    
    // ENEMY UPDATE LOOP
    for (auto& enemy : enemy_vec) {
      if(enemy->state == Enemy::DEAD && !(enemy_vec.empty())) {
        enemy->send_to_valhalla(enemy->getSprite());
        SndMgr.playSound("fairy_death");
        lvl_score += 10;
        // very expensive operation. Also all enemies blink
        // when one of them dies
        enemy_vec.erase(std::find(enemy_vec.begin(), enemy_vec.end(), enemy));
        enemy_vec.shrink_to_fit();
        break;
      } else if (enemy->state == Enemy::FLEW_AWAY && !(enemy_vec.empty())) {
        enemy->send_to_valhalla(enemy->getSprite());
        enemy_vec.erase(std::find(enemy_vec.begin(), enemy_vec.end(), enemy));
        enemy_vec.shrink_to_fit();
        break;
      } else {
        enemy->updateEnemy(delta);
        window.draw(enemy->getSprite());
      }
    }
    // ====== ENEMY WAVES PART ====== //

    // ====== PROJECTILES PART ====== //
    // update player projectiles
    for (auto& plr_proj : ProjVec) {
      if (plr_proj->isFlewAway() && !(ProjVec.empty())) {
        ProjVec.erase(ProjVec.begin());
        ProjVec.shrink_to_fit();
        break; 
      }
      else {
        plr_proj->update(delta);
        window.draw(plr_proj->getShape());
      }
    }
    
    // update enemy projectiles
    for (auto& enm_proj : enemy_proj_vec) {
      if(!enm_proj->isFlewAway()) {
        enm_proj->update(delta);
        window.draw(enm_proj->getShape());
      }
    }
    // ====== PROJECTILES PART ====== //

    // draw what have been rendered so far
    window.display();                              
    
    // calculate delta time at the end of each frame
    delta = (float)frame_clock.restart().asSeconds();    
  }
  // ====== GAME LOOP ====== //
}

Game::~Game() {
  std::clog << "Game finished succesfully (i hope so)\n";
}