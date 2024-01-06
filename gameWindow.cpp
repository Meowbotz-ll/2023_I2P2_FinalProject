#include "gameWindow.h"
#include "log.h"  // Include your log class
#include "algorithm"
#include "bullet.h"


#include <iterator>
using namespace std;

GameWindow::GameWindow() : currentState(MENU),doexit(false){
    Log::Info("GameWindow Created");
    init();
    enemies.push_back(Enemy(100, 100));
}

GameWindow::~GameWindow() {
    Log::Info("GameWindow Deleted");
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    if (ui_font) {
        al_destroy_font(ui_font);
    }
}

void GameWindow::init() {
    Log::Info("Init Started:");

    // Initialize Allegro subsystems
    if (!al_init()) {
        Log::Error("Failed to initialize Allegro");
        return;
    }
    if (!al_install_keyboard()) {
        Log::Error("Failed to initialize keyboard");
        return;
    }
    if (!al_init_image_addon()) {
        Log::Error("Failed to initialize image addon");
        return;
    }
    if (!al_init_primitives_addon()) {
        Log::Error("Failed to initialize primitives addon");
        return;
    }
    if (!al_install_mouse()) {
        Log::Error("Failed to initialize mouse");
        return;
    }
    // Initialize font add-ons
    al_init_font_addon(); // Initialize the font addon
    if (!al_init_ttf_addon()) { // Initialize the TrueType font addon
        Log::Error("Failed to initialize TTF font addon");
        return;
    }
// Initialize the font for the menu
    Log::Info("Loading font...");
    font = al_load_font("fonts/ARCADE.TTF", 36, 0);
    if (!font) {
        Log::Error("Failed to load font at fonts/ARCADE.TTF");
    }
    menu.init(font);
    //ui font
    ui_font = al_load_font("fonts/ARCADE.TTF", 24, 0); // Load a font
    if (!ui_font) {
        Log::Error("Failed to load UI font");
    }
    startTime = al_get_time(); // Record the start time
// Initialize audio subsystem
    if (!al_install_audio()) {
        Log::Error("Failed to initialize audio");
        return;
    }
    if (!al_init_acodec_addon()) {
        Log::Error("Failed to initialize audio codec addon");
        return;
    }
    if (!al_reserve_samples(16)) {  // Reserve enough sample instances for your game
        Log::Error("Failed to reserve audio samples");
        return;
    }
    // Init background Music
    menuMusic = al_load_sample("audio/Electroman-Adventures.mp3");
    gameMusic = al_load_sample("audio/Electroman-Adventures.mp3");

const char* playerGifFiles[2] = {
        "player_img/skeleton_left.gif",   // Replace with actual file path
        "player_img/skeleton_right.gif",  // Replace with actual file path
    };

// Log the loading of each GIF file
    for (int i = 0; i < 2; ++i) {
        Log::Info(std::string("Loading GIF: ") + playerGifFiles[i]);
    }

if (!menuMusic || !gameMusic) {
    Log::Error("Failed to load music files");
    return;
}

    // Create display
    display = al_create_display(800, 600);
    if (!display) {
        Log::Error("Display failed to be created");
        return;
    } else {
        Log::Info("Display created");
    }
    // Create timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        Log::Error("Timer failed to be created");
        return;
    } else {
        Log::Info("Timer created");
    }

    // Create event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        Log::Error("Event queue failed to be created");
        return;
    } else {
        Log::Info("Event queue created");
    }

    // Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    player.init(400, 500,playerGifFiles);

    Log::Info("GameWindow initialization complete");
}


void GameWindow::run() {
    Log::Info("Game Started!");

    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        switch (currentState) {
            case MENU:
            //Log::Info("In Menu State");
                if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        Log::Info("Enter Key Pressed - Switching to Game State"); // Debug message for pressing Enter
                        al_stop_samples(); // Stop any currently playing music
                        currentState = GAME;
                        menu.gameStart = true; // Update the flag in menu
                    }
                }
                menu.update();
                break;

            case GAME:
            //Log::Info("In Menu State");
        switch (ev.type) {
            case ALLEGRO_EVENT_TIMER:
            
                player.update();
                for (auto& enemy : enemies) {
                    enemy.update();
                }
                
                // Add any other updates here, e.g., for game world, enemies, etc.
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                doexit = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key_state[ev.keyboard.keycode] = true;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key_state[ev.keyboard.keycode] = false;
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    Log::Info("Escape Key Pressed - Switching to Menu State"); // Debug message for pressing Escape
                    al_stop_samples(); // Stop any currently playing music
                    currentState = MENU;
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (ev.mouse.button & 1) { // Left mouse button
                    Log::Info("Left Mouse Button Clicked");
                    player.shoot(ev.mouse.x, ev.mouse.y);
                }
                break;

            // Add additional cases here for other types of events
        }


        for (auto& bullet : player.getBullets()) {
            if (bullet.isHit()) continue; // 如果子弹已经击中了敌人，跳过这个子弹

            for (auto& enemy : enemies) {
                if (checkCollision(bullet, enemy) && enemy.isAlive()) {
                    enemy.hit(Bullet::DEFAULT_DAMAGE);
                    bullet.setHit(true); // 标记子弹已经击中敌人
                    bullet.setAlive(false); // 设置子弹为不活跃
                    break; // 退出内部循环
                }
            }
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
                    [](const Enemy& enemy) { return !enemy.isAlive(); }), enemies.end());

        break;

        }

        draw();
    }
}


void GameWindow::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    switch (currentState) {
        case MENU:
            al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            menu.draw();
            break;

        case GAME:
currentTime = static_cast<int>(al_get_time() - startTime);

            std::ostringstream timeStream;
            timeStream.precision(0); // Set precision to 0 to avoid any decimals
            timeStream << "Time: " << currentTime;
            std::string timeText = timeStream.str();

            std::string scoreText = "Score: " + std::to_string(score);

            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 10, 0, timeText.c_str());
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 40, 0, scoreText.c_str());
            
            al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            player.draw();
            for (auto& enemy : enemies) {
                enemy.draw();
            }
            // Draw other game elements
            break;
    }

    al_flip_display();
}
bool GameWindow::checkCollision(const Bullet &bullet, const Enemy &enemy) {
    // Simple circle-rectangle collision detection
    float distX = std::abs(bullet.x - enemy.x);
    float distY = std::abs(bullet.y - enemy.y);

    if (distX > (Bullet::SIZE / 2 + Enemy::RADIUS)) { return false; }
    if (distY > (Bullet::SIZE / 2 + Enemy::RADIUS)) { return false; }

    if (distX <= (Bullet::SIZE / 2)) { return true; } 
    if (distY <= (Bullet::SIZE / 2)) { return true; }

    // Corner collision
    float dx = distX - Bullet::SIZE / 2;
    float dy = distY - Bullet::SIZE / 2;
    return (dx * dx + dy * dy <= (Enemy::RADIUS * Enemy::RADIUS));
}

