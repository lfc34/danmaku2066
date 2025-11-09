/// PORTED TO SFML3 v
#include "Game.hpp"

#include <iostream>
#include <map>
#include <algorithm>
#include <memory>

#include "Defaults.hpp"
#include "GameState.hpp"
#include "Interface.hpp"
#include "Logger.hpp"
#include "Projectile.hpp"
#include "Enemy.hpp"
#include "Level.hpp"
#include "Player.hpp"

Game::Game() : 
  window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Muzhik 2066"),
  GAME_STATE(GameState::getInstance())
{
  window.setFramerateLimit(60);
  window.setVerticalSyncEnabled(true);
  window.setMouseCursorVisible(false);
}

void Game::MainLoop() {   
  MainMenu* menu = new MainMenu(m_uidata);
  menu->displayMenu(window);
  if (GAME_STATE.scene == GameState::LEVEL) {
    delete menu;
    lvl1Loop();
  }
  if (GAME_STATE.scene == GameState::SURVIVAL) {
    delete menu;
    survival_loop();
  }
  delete menu;
  Logger::log_clr("Exiting main loop...");
}

void Game::game_pause(sf::Clock& wave_clock, sf::Clock& spawn_timer) {
  enum PauseReturn {
    Continue = 1,
    Restart = 2,
    Audio
  };
  PauseMenu pm;
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

      case Audio: 
        GAME_STATE.toggleAudio();
        break;

      case Restart:
        lvl1Loop();
        break;
    }
  }
}

void Game::game_over(const sf::Font& font, int& score) {
  using namespace sf::Keyboard;
  
  sf::Text game_over(font, "Game over!", 50);  
  game_over.setPosition({260, 120});

  sf::Text restart_btn(font, "Restart", 35);
  restart_btn.setPosition({330, 300});
  restart_btn.setFillColor(sf::Color::Green);

  sf::Text quit_game_btn(font, "Quit game", 35);
  quit_game_btn.setPosition({310, 340});

  sf::Text final_score(font, "Score: " + std::to_string(score), 40);
  final_score.setPosition({310, 180});
  final_score.setFillColor(sf::Color::Red);

  std::vector<sf::Text*> options_list {&restart_btn, &quit_game_btn};

  enum Options {
    Restart,
    Quit
  };

  size_t option = Restart;
  while(window.isOpen()) {
    window.clear();
    while(const std::optional event = window.pollEvent()) {
      auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
      if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Up) {
        options_list.at(option)->setFillColor(sf::Color::White);
        if (option == Restart)
          option = Quit;
        else
          --option;
        options_list.at(option)->setFillColor(sf::Color::Green);
      } else if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Down) {
        options_list.at(option)->setFillColor(sf::Color::White);
        ++option;
        if (option > Quit)
          option = Restart;
        options_list.at(option)->setFillColor(sf::Color::Green);
      }
      switch (option) {
        case Restart:
          if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Enter) {
            if (game_mode == Game_Level)
              lvl1Loop();
            else  
              survival_loop();
          }
          break;

        case Quit:
          if (event->is<sf::Event::KeyReleased>() && keyReleased->code == Key::Enter) 
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

void Game::win_screen(int& score, int& plr_lives, const sf::Font& font) {
  sf::Texture winbg_t;
  if(!(winbg_t.loadFromFile("../assets/gfx/winscreen.png"))) {
    std::cerr << "Failed to load win screen (lmao). Exiting...\n";
    exit(1);
  }
  sf::Sprite winbg(winbg_t);
  std::string f_score = std::to_string(plr_lives * score);
  sf::Text final_score(font, f_score, 50);
  final_score.setPosition({60, 300});
  while (window.isOpen()) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M))
      MainLoop();
    window.clear();
    window.draw(winbg);
    window.draw(final_score);
    window.display();
  }
}

// the "survival loop" MUST NOT load anything except its own DATA!!!
void Game::survival_loop() {
  using namespace sf::Keyboard;
  game_mode = Game_Survival;
  // UI for level
  const sf::Font UI_font("../assets/gfx/dynapuff.ttf");

  int lvl_score {};
  sf::Text player_score(UI_font, std::to_string(lvl_score), 19);
  player_score.setPosition({640, 570});
  sf::Text player_lives(UI_font, "0", 19);
  player_lives.setPosition({640, 550});

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
    {0, {}}, 
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

  survival.playMusic();
   // ====== GAME LOOP ====== //
  while(window.isOpen()) {

    //clear window at the start of each frame
    window.clear();
    
    survival.drawLevel(window); 

    // player update + input handler
    player->input_handler(delta, SndMgr);
    player->updatePlayer(window, SndMgr);

    // UI draw part
    player_lives.setString("Lives:" + std::to_string(player->lives));
    player_score.setString("Score:" + std::to_string(lvl_score));
    window.draw(player_score);
    window.draw(player_lives);
    
    // event processor
    while(const std::optional event = window.pollEvent()) {
      const auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
      // closing window via cross on the panel
      if(event->is<sf::Event::Closed>()) {
        window.close();
        break;
      }
      
      // game pause
      if(event->is<sf::Event::KeyReleased>() 
        && keyReleased->code == Key::Escape) { 
        survival.stop_music();

        frame_clock.restart();
        game_pause(frame_clock, frame_clock);
        // after continuing the gaem
        survival.playMusic();
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
  using namespace sf::Keyboard;
  game_mode = Game_Level;
  // UI for level
  const sf::Font UI_font("../assets/gfx/dynapuff.ttf");
  // load UI font
  int lvl_score {};
  sf::Text player_score(UI_font, std::to_string(lvl_score), 19);
  player_score.setPosition({640, 570});
  sf::Text player_lives(UI_font, "0", 19);
  player_lives.setPosition({640, 550});

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
  
  lvl1.playMusic();
  
  // Number of enemies for every wave
  std::array<int, 8> enemies_in_wave = {10, 10, 5, 5, 14, 14, 15, 15};
    
  // load boss before game starts, spawn him later
  Boss wizard(ptrs);
  std::array<Boss, 1> boss_arr = {wizard};

    // ====== GAME LOOP ====== //
  while(window.isOpen()) {

    //clear window at the start of each frame
    window.clear();
    
    lvl1.drawLevel(window); 

    // player update + input handler
    player->input_handler(delta, SndMgr);
    player->updatePlayer(window, SndMgr);

    // UI draw part
    player_lives.setString("Lives:" + std::to_string(player->lives));
    player_score.setString("Score:" + std::to_string(lvl_score));
    window.draw(player_score);
    window.draw(player_lives);
    
    // event processor
    while(const std::optional event = window.pollEvent()) {
      const auto* keyReleased = event->getIf<sf::Event::KeyReleased>();
      // closing window via cross on the panel
      if(event->is<sf::Event::Closed>()) {
        window.close();
        break;
      }
      
      // game pause
      if(event->is<sf::Event::KeyReleased>() 
        && keyReleased->code == Key::Escape) { 
        lvl1.stop_music();

        frame_clock.restart();
        game_pause(frame_clock, frame_clock);
        // after continuing the gaem
        lvl1.playMusic();
        frame_clock.restart();
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
          lvl1.play_boss_mus();
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