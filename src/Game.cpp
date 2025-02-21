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

void Game::game_over(sf::Font& font, unsigned int& score) {
  using namespace Controls;
  sf::Text game_over("Game over!", font, 50);  
  game_over.setPosition(260, 120);

  sf::Text restart_btn("Restart", font, 25);
  restart_btn.setPosition(260, 300);
  restart_btn.setFillColor(sf::Color::Green);
  sf::Text quit_game_btn("Quit game", font, 25);
  quit_game_btn.setPosition(260, 340);

  sf::Text final_score("Score: " + std::to_string(score), font, 40);
  final_score.setPosition(260, 180);
  final_score.setFillColor(sf::Color::Red);

  enum Options {
    Restart,
    Quit
  };

  size_t option = Restart;
  while(window.isOpen()) {
    if (kb.isKeyPressed(UP)) {
      restart_btn.setFillColor(sf::Color::Green);
      quit_game_btn.setFillColor(sf::Color::White);
      option = Restart;
    } else if (kb.isKeyPressed(Controls::DOWN)) {
      quit_game_btn.setFillColor(sf::Color::Green);
      restart_btn.setFillColor(sf::Color::White);
      option = Quit;
    }
    switch (option) {
      case Restart:
        if (kb.isKeyPressed(Z_K))
          lvl1Loop();
        break;

      case Quit:
        if (kb.isKeyPressed(Z_K)) 
          exit(1);
        break;
    }
    window.clear();
    window.draw(game_over);
    window.draw(final_score);
    window.draw(restart_btn);
    window.draw(quit_game_btn);
    window.display();
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

  // Sound manager to load sounds to buffer and play them
  std::shared_ptr SndMgr = std::make_shared<SoundManager>();

  // vectors to store projectiles and entities
  std::vector<std::unique_ptr<Projectile>> ProjVec {};
  std::vector<std::unique_ptr<Projectile>> enemy_proj_vec;
  std::vector<std::unique_ptr<Enemy>> enemy_vec; 

  // player
  std::unique_ptr player = std::make_unique<Player>(SndMgr, ProjVec,
                           enemy_vec, enemy_proj_vec);

  // timers
  float delta {};
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
  std::vector<MovePattern> directions;
  MovePattern wave1 {}; 
  directions.push_back(wave1);
  //=========================WAVE SPECIFIC DATA==============================//

  // ====== GAME LOOP ====== //
  while(window.isOpen()) {
    // allows us to close window via cross on the panel
    while(window.pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
        window.close();
        break;
      }
    }
    window.clear();

    lvl1.drawLevel(window); 
    if (player->lives <= 0) {
      std::clog << "Player died\n";
      lvl1.stop_music();
      game_over(UI_font, lvl_score);
    }

    // ENEMY WAVES PART //
    // this loop can be refactored as a function to fill enemy vector
    if (enemyCntWave.at(currentWave) > 0 
       && waveTimer.getElapsedTime().asSeconds() > 3) {

      if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 200) {     
        enemy_vec.emplace_back(std::make_unique<MoonStone>
                (ProjVec, enemy_proj_vec, wave1));

        --enemyCntWave.at(currentWave);
        enemy_spawn_timer.restart();
      }
      // !!!Don't forget out of bounds check
      // if (enemyCntWave.at(currentWave) <= 0)
        // ++currentWave;
    }
    for (auto& enemy : enemy_vec) {
      if(enemy->state == Enemy::DEAD && !(enemy_vec.empty())) {
        enemy->send_to_valhalla(enemy->getSprite());
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
    // ENEMY WAVES PART //

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
    
    for (auto& enm_proj : enemy_proj_vec) {
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

    // UI draw part
    player_score.setString("Score:" + std::to_string(lvl_score));
    player_lives.setString("Lives:" + std::to_string(player->lives));
    window.draw(player_score);
    window.draw(player_lives);

    // draw what have been rendered so far
    window.display();                              
    delta = (float)frame_clock.restart().asSeconds();
  }
  // ====== GAME LOOP ====== //
}