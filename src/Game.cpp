#include "Game.h"

// MOVE PATTERNS
const MovePattern curve {}; 
const MovePattern op_curve {
  sf::Vector2f (800, 0),
  sf::Vector2f (-200, 150),
  sf::Vector2f (-400, -85)
};
const MovePattern march {
  sf::Vector2f (0, 0),
  sf::Vector2f (0, 100),
  sf::Vector2f (0, -100)
};
const MovePattern lr_line {
  sf::Vector2f (0, 50),
  sf::Vector2f (0,50),
  sf::Vector2f (200, 50)
};
const MovePattern rl_line {
  sf::Vector2f (800, 50),
  sf::Vector2f (800, 40),
  sf::Vector2f (-200, 50)
};

Game::Game() : 
window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Muzhik 2066") {
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
  window.setMouseCursorVisible(false);
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

      case menu.SURVIVAL:
        return 1;
    }
  }
  window.clear();
  return -11;
}

void Game::game_pause(sf::Clock& wave_clock, sf::Clock& spawn_timer) {
  enum PauseReturn {
    Continue = 1,
    Restart = 2,
    Unmute,
    Mute
  };
  PauseMenu pm(IS_GAME_MUTED);
  bool paused = true;
  while (window.isOpen() && paused) {
    // constantly restart timers to keep them at 0
    wave_clock.restart();
    spawn_timer.restart();
    pm.draw_menu(window);
    switch (pm.menu_loop(window)) {
      case Continue:
        paused = false;
        break;

      case Unmute:
        IS_GAME_MUTED = false;
        break;

      case Mute: 
        IS_GAME_MUTED = true;
        break;

      case Restart:
        lvl1Loop();
        break;
    }
  }
}

void Game::game_over(sf::Font& font, int& score) {
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
          if (event.type == sf::Event::KeyReleased && event.key.code == RET) {
            if (game_mode == Game_Level)
              lvl1Loop();
            else  
              survival_loop();
          }
          break;

        case Quit:
          if (event.type == sf::Event::KeyReleased && event.key.code == RET) 
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

void Game::win_screen(int& score, int& plr_lives, sf::Font& font) {
  sf::Texture winbg_t;
  if(!(winbg_t.loadFromFile("../assets/gfx/winscreen.png"))) {
    std::cerr << "Failed to load win screen (lmao). Exiting...\n";
    exit(1);
  }
  sf::Sprite winbg(winbg_t);
  std::string f_score = std::to_string(plr_lives * score);
  sf::Text final_score;
  final_score.setFont(font);
  final_score.setString(f_score);
  final_score.setCharacterSize(50);
  final_score.setPosition(60, 300);
  while (window.isOpen()) {
    if (kb.isKeyPressed(sf::Keyboard::Key::M))
      menuLoop();
    window.clear();
    window.draw(winbg);
    window.draw(final_score);
    window.display();
  }
}

void Game::survival_loop() {
  game_mode = Game_Survival;
  // UI for level
  sf::Font UI_font;
  // load UI font
  if(!UI_font.loadFromFile("../assets/gfx/dynapuff.ttf")) {
    std::cerr << "Failed to load assets/gfx/dynapuff.tff. Exiting...\n";
    exit(1);
  }
  int lvl_score {};
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
  EnemyData ptrs {
    &ProjVec,
    &enemy_proj_vec
  };

  // player
  std::unique_ptr player = std::make_unique<Player>(ProjVec,
    enemy_vec, enemy_proj_vec);
    
  // timers
  float delta {};
  /** Clock that counts time duration of one frame (delta time) */
  sf::Clock frame_clock;

  sf::Clock enemy_spawn_timer;
  
  float spawn_rand {};
  
  std::map<int, MovePattern> mp_map{
    {0, curve}, 
    {1, op_curve}, 
    {2, march}, 
    {3, lr_line},
    {4, rl_line}
  };
  enum EnemyType {
    FAIRY,
    LIZARD,
    SKULL
  };
  MovePattern mp {};

  Level survival("../assets/sfx/surv.ogg",
  "../assets/gfx/bg_surv.jpg");

  survival.playMusic(IS_GAME_MUTED);
   // ====== GAME LOOP ====== //
  while(window.isOpen()) {

    // to make mute audio button work
    if (IS_GAME_MUTED)
      SndMgr.is_muted = true;
    else  
      SndMgr.is_muted = false;

    //clear window at the start of each frame
    window.clear();
    
    survival.drawLevel(window); 

    // player update + input handler
    player->input_handler(kb, delta, SndMgr);
    player->updatePlayer(window, SndMgr);

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
        survival.stop_music();

        frame_clock.restart();
        game_pause(frame_clock, frame_clock);
        // after continuing the gaem
        survival.playMusic(IS_GAME_MUTED);
        frame_clock.restart();
      }
    }

    if (player->lives <= 0) {
      std::clog << "Player died\n";
      SndMgr.playSound("player_death");
      survival.stop_music();
      game_over(UI_font, lvl_score);
    }
    
    // ====== ENEMY WAVES PART ====== //
    spawn_rand = float(rand()%6 + 2);
    switch (rand()%4) {
      case FAIRY:
        mp = mp_map.at(rand()%4);
        if (enemy_spawn_timer.getElapsedTime().asSeconds() > spawn_rand) {
          enemy_vec.emplace_back(std::make_unique<Fairy>
                                (ptrs, mp, mp.spawn_pos));
          enemy_spawn_timer.restart();
        }
        break;
      
      case LIZARD:
        mp = mp_map.at(rand()%4);
        if (enemy_spawn_timer.getElapsedTime().asSeconds() > spawn_rand) {
          enemy_vec.emplace_back(std::make_unique<LizardKiller>
                                (ptrs, mp, mp.spawn_pos));
          enemy_spawn_timer.restart();
        }
        break;

      case SKULL:
        mp = mp_map.at(rand()%4);
        if (enemy_spawn_timer.getElapsedTime().asSeconds() > spawn_rand) {
          enemy_vec.emplace_back(std::make_unique<Skull>
                                (ptrs, mp, mp.spawn_pos));
          enemy_spawn_timer.restart();
        }
        break;
    }
    
    // ENEMY UPDATE LOOP
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
        enemy->updateEnemy(delta, SndMgr);
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


void Game::lvl1Loop() {
  game_mode = Game_Level;
  // UI for level
  sf::Font UI_font;
  // load UI font
  if(!UI_font.loadFromFile("../assets/gfx/dynapuff.ttf")) {
    std::cerr << "Failed to load assets/gfx/dynapuff.tff. Exiting...\n";
    exit(1);
  }
  int lvl_score {};
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
  EnemyData ptrs {
    &ProjVec,
    &enemy_proj_vec
  };

  // player
  std::unique_ptr player = std::make_unique<Player>(ProjVec,
    enemy_vec, enemy_proj_vec);
    
  // timers
  float delta {};
  /** Clock that counts time duration of one frame (delta time) */
  sf::Clock frame_clock;
  /** Clock that counts time for wave to appear */
  sf::Clock waveTimer;
  float time_wave {};
  /** Clock that controls how often enemies in wave spawn */
  sf::Clock enemy_spawn_timer;
  
  // RNG part. Let it be here, maybe I'll need it later.
  // for now enemies will spawn in waves in predefined pattern
  // std::random_device rd;
  // std::mt19937 rng(rd()); 
  // std::uniform_int_distribution<std::mt19937::result_type> rand_spwn_pos(25, 775);
  
  Level lvl1("../assets/sfx/lvl1.ogg",
  "../assets/gfx/bg.jpg");

  lvl1.load_boss_mus("../assets/sfx/boss1.ogg");
  
  lvl1.playMusic(IS_GAME_MUTED);
  
  // Number of enemies for every wave
  std::array<int, 8> enemies_in_wave = {10, 10, 5, 5, 14, 14, 15, 15};
    
  // load boss before game starts, spawn him later
  Boss wizard(ptrs);
  std::array<Boss, 1> boss_arr = {wizard};

    // ====== GAME LOOP ====== //
  while(window.isOpen()) {

    // to make mute audio button work
    if (IS_GAME_MUTED)
      SndMgr.is_muted = true;
    else  
      SndMgr.is_muted = false;

    //clear window at the start of each frame
    window.clear();
    
    lvl1.drawLevel(window); 

    // player update + input handler
    player->input_handler(kb, delta, SndMgr);
    player->updatePlayer(window, SndMgr);

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
        game_pause(waveTimer, enemy_spawn_timer);
        // after continuing the gaem
        lvl1.playMusic(IS_GAME_MUTED);
        frame_clock.restart();
        enemy_spawn_timer.restart();
      }
    }

    if (player->lives <= 0) {
      std::clog << "Player died\n";
      SndMgr.playSound("player_death");
      lvl1.stop_music();
      game_over(UI_font, lvl_score);
    }
    
    // ====== ENEMY WAVES PART ====== //
    time_wave += waveTimer.restart().asSeconds();
    // wave 1
    if (time_wave > 3) {
      for (int i = enemies_in_wave[0]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 200) {
          enemy_vec.emplace_back(std::make_unique<Fairy>
                                (ptrs, curve, curve.spawn_pos));    
          --enemies_in_wave[0];
          enemy_spawn_timer.restart();
        }
      }
    }

    // wave 2
    if (time_wave > 8) {
      for (int i = enemies_in_wave[1]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 200) {
          enemy_vec.emplace_back(std::make_unique<Fairy>
                                (ptrs, op_curve, op_curve.spawn_pos));
          --enemies_in_wave[1];
          enemy_spawn_timer.restart();
        }
      }
    }

    // wave 3
    if (time_wave > 13) {
      for (int i = enemies_in_wave[2]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 150) {
          enemy_vec.emplace_back(std::make_unique<Fairy>(ptrs, rl_line, rl_line.spawn_pos));
          enemy_spawn_timer.restart();
        }
        enemy_vec.emplace_back(std::make_unique<LizardKiller>
                              (ptrs, march, sf::Vector2f ((100 * (float)i + 1), 0)));
        --enemies_in_wave[2];
      }
    }

    // wave 4
    if (time_wave > 19) {
      for (int i = enemies_in_wave[3]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 150) {
          enemy_vec.emplace_back(std::make_unique<Fairy>(ptrs, lr_line, lr_line.spawn_pos));
          enemy_spawn_timer.restart();
        }
        enemy_vec.emplace_back(std::make_unique<LizardKiller>
                              (ptrs, march, sf::Vector2f ((100 * (float(i) + 2)), 0)));
        --enemies_in_wave[3];
      }
    }

    // wave 5 
    if (time_wave > 25) {
      for (int i = enemies_in_wave[4]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 350) {
          enemy_vec.emplace_back(std::make_unique<Skull>
                                (ptrs, curve, curve.spawn_pos));
          --enemies_in_wave[4];
          enemy_spawn_timer.restart();
        }
      }
    }

    // wave 6
    if (time_wave > 31) {
      for (int i = enemies_in_wave[5]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 350) {
          enemy_vec.emplace_back(std::make_unique<Skull>
                                (ptrs, op_curve, op_curve.spawn_pos));
          --enemies_in_wave[5];
          enemy_spawn_timer.restart();
        }
      }
    }
    
    // wave 7
    if (time_wave > 37) {
      for (int i = enemies_in_wave[6]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 150) {
          enemy_vec.emplace_back(std::make_unique<Fairy>
                                (ptrs, lr_line, lr_line.spawn_pos));
          --enemies_in_wave[6];
          enemy_spawn_timer.restart();
        }
      }
    }

    // wave 8
    if (time_wave > 43) {
      for (int i = enemies_in_wave[7]; i > 0; --i) {
        if (enemy_spawn_timer.getElapsedTime().asMilliseconds() > 150) {
          enemy_vec.emplace_back(std::make_unique<Fairy>
                                (ptrs, rl_line, rl_line.spawn_pos));
          --enemies_in_wave[7];
          enemy_spawn_timer.restart();
        }
      }
    }

    // FINAL BOSS LVL1
    if (time_wave > 50 && enemies_in_wave[7] <= 0) {
      for (auto& boss : boss_arr) {
        if (!(boss.triggered)) {
          boss.trigger();
          lvl1.play_boss_mus(IS_GAME_MUTED);
        } else {
          window.draw(boss.get_sprite());
          if (boss.update_boss(delta, time_wave)) {
            lvl_score+=1000;
            lvl1.stop_music();
            SndMgr.playSound("game_win");
            win_screen(lvl_score, player->lives, UI_font);
          }
        }
      }
    }


    // ENEMY UPDATE LOOP
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
        enemy->updateEnemy(delta, SndMgr);
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