#include "gameWindow.h"
#include "log.h"  // Include your log class
#include "algorithm"
#include "bullet.h"


#include <iterator>
using namespace std;

GameWindow::GameWindow() : currentState(MENU),previousState(MENU),backgroundImage(nullptr),doexit(false),gameSceneInitialized(false){
    Log::Info("GameWindow Created");
    init();
    //enemies.push_back(Enemy(100, 100, 0.5));
}

GameWindow::~GameWindow() {
    Log::Info("GameWindow Deleted");
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    if (ui_font) {
        al_destroy_font(ui_font);
    }
    if (backgroundImage) {
        al_destroy_bitmap(backgroundImage);
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
    // Load background image
    backgroundImage = al_load_bitmap("img/bg.jpeg");
    if (!backgroundImage) {
        Log::Error("Failed to load background image");
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

    // 初始化按鈕位置和大小
    back_button_x = 300;
    back_button_y = 400;
    exit_button_x = 500;
    exit_button_y = 400;
    button_width = 100;  // 根據需要調整大小
    button_height = 50;

    Log::Info("GameWindow initialization complete");
}

void GameWindow::initMenuScene()
{
    Log::Info("Menu Scene Initialized");
}
void GameWindow::initGameOverScene() {
    // Add your game over scene initialization code here
    Log::Info("Game Over Scene Initialized");
    // Example: Load game over assets, set up UI elements, etc.
}

void GameWindow::initGameScene()
{
    if(!gameSceneInitialized)
    {
        if(backgroundImage!=nullptr)
        {
        al_draw_bitmap(backgroundImage, 0, 0, 0);
        Log::Info("Background Init");
        }
        gameSceneInitialized=true;
    }
    al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}

void GameWindow::initScene() {
    // Check if the current state is different from the previous state
    if (currentState != previousState) {
        switch (currentState) {
            case MENU:
                initMenuScene();  // Initialize menu-specific resources
                break;
            case GAME:
                initGameScene();  // Initialize game-specific resources
                break;
            case GAME_OVER:

                break;
            // Add other cases as needed
        }
        previousState = currentState;  // Update the previous state
    }
}
void GameWindow::run() {
    Log::Info("Game Started!");

    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        initScene();
        
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
                game_enemy();
                game_player();
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
                if (ev.mouse.button & 2) {  // Right mouse button
                    Log::Info("Right Mouse Button Clicked");

                }
                
                break;
                case GAME_OVER:
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    Log::Info("Escape Clicked");
                    // Example: Return to menu or close the game
                    currentState=MENU;
                }
                
                break;

            // Add additional cases here for other types of events
        }

        
        case GAME_OVER:
            
        break;

        }

        
        draw();
    }
}

void GameWindow::game_enemy()
{
    static double last_spawn_time = 0;
    static double enemySpawnInterval = 2.0;     
    if (al_get_time() - last_spawn_time > enemySpawnInterval) {
        last_spawn_time = al_get_time();
        float spawnY;
        float velocityX;
        EnemyType type;

        // 随机决定生成哪种类型的敌人
        if (rand() % 2 == 0) {
            // 生成天上的敌人
            spawnY = static_cast<float>(rand() % 300); // 屏幕上半部
            velocityX = 1.0; // 初始速度
            type = EnemyType::AIR;
        } else {
            // 生成地上的敌人
            spawnY = 475; // 与玩家相同的高度
            velocityX = 2.0; // 天上敌人速度的两倍
            type = EnemyType::GROUND;
        }

        // 随机决定敌人从左侧或右侧出现
        float spawnX;
        if (rand() % 2 == 0) {
            spawnX = -1; // 从屏幕左侧出现
        } else {
            spawnX = 801; // 从屏幕右侧出现
            velocityX = -velocityX; // 改变移动方向
        }

        // 添加敌人到列表
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type));
        enemySpawnInterval = std::max(1.0, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    // 遍历所有敌人
    for (auto& enemy : enemies) {
        enemy.update(); // 更新敌人位置和状态
        // 如果敌人是天空中的敌人，则射击玩家
        if (enemy.getType() == EnemyType::AIR) {
            enemy.shootAtPlayer(player); // 让敌人射击玩家
        }
        // 遍历天空敌人的子弹并保持它们永远活跃
            for (auto& bullet : enemy.getBullets()) {
                bullet.setAlive(true);
            }
    }

    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.getBullets()) {
            bullet.setAlive(true);
            if (checkCollision(bullet, player)) {
                player.getHit(1);
                bullet.setAlive(false); // Remove the bullet upon collision
            }
        }
    }

    for (auto& enemy : enemies) {
        enemy.removeInactiveBullets();
    }
    //ground
    for (auto& enemy : enemies) {
        enemy.update();
        if (enemy.isAlive() && checkCollision(player, enemy)) {
            player.getHit(1);
            enemy.set_Alive(false);
        }
    }
}


void GameWindow::game_player()
{
    player.update();
    // Check if the player's health is 0 or less
    if (player.getHp() <= 0) {
        //Log::Info("Game Over!");
        currentState = GAME_OVER;
    }
    // 检测子弹与敌人的碰撞
    for (auto& bullet : player.getBullets()) {
        if (bullet.isHit()) continue; // 如果子弹已经击中了敌人，跳过这个子弹
        for (auto& enemy : enemies) {
            if (checkCollision(bullet, enemy) && enemy.isAlive()) {
                enemy.hit(bullet.getDamage());
                bullet.setHit(true); // 标记子弹已经击中敌人
                bullet.setAlive(false); // 设置子弹为不活跃
                if(enemy.hp<=0){
                    score++;
                    enemy.set_Alive(false);
                }
                    break; // 退出内部循环
            }
        }
    }
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) {
            for (auto& bullet : enemy.getBullets()) {
                bullet.update();
                // 也许可以在这里检查子弹是否超出屏幕范围，并将其标记为不活跃
                if (checkCollision(bullet, player)) {
                    player.getHit(bullet.getDamage()); // 玩家受到伤害
                    bullet.setAlive(false); // 子弹消失
                }
            }
        }
    }

}

bool GameWindow::checkCollision(const Bullet &bullet, const Enemy &enemy) {
    // Simple circle-rectangle collision detection
    float distX = std::abs(bullet.x - enemy.x);
    float distY = std::abs(bullet.y - enemy.y);

    if (distX > (bullet.getSize()/ 2 + Enemy::RADIUS)) { return false; }
    if (distY > (bullet.getSize()/ 2 + Enemy::RADIUS)) { return false; }

    if (distX <= (bullet.getSize()/ 2)) { return true; } 
    if (distY <= (bullet.getSize()/ 2)) { return true; }

    // Corner collision
    float dx = distX - bullet.getSize()/ 2;
    float dy = distY - bullet.getSize()/ 2;
    return (dx * dx + dy * dy <= (Enemy::RADIUS * Enemy::RADIUS));
}

bool GameWindow::checkCollision(const Bullet& bullet, const Player& player) {
    // Simple AABB (Axis-Aligned Bounding Box) collision check
    float playerLeft = player.getX();
    float playerRight = player.getX() + player.PLAYER_SIZE;
    float playerTop = player.getY();
    float playerBottom = player.getY() + player.PLAYER_SIZE;

    float bulletLeft = bullet.x;
    float bulletRight = bullet.x + bullet.getSize();
    float bulletTop = bullet.y;
    float bulletBottom = bullet.y + bullet.getSize();

    return (playerLeft < bulletRight && playerRight > bulletLeft &&
            playerTop < bulletBottom && playerBottom > bulletTop);
}

bool GameWindow::checkCollision(const Player& player, const Enemy& enemy) {
    // 獲取玩家和敵人的位置和大小
    float playerLeft = player.getX();
    float playerRight = player.getX() + player.PLAYER_SIZE;
    float playerTop = player.getY();
    float playerBottom = player.getY() + player.PLAYER_SIZE;

    float enemyLeft = enemy.get_X();
    float enemyRight = enemy.get_X() + enemy.ENEMY_SIZE;  // 假設敵人有一個SIZE常量
    float enemyTop = enemy.get_Y();
    float enemyBottom = enemy.get_Y() + enemy.ENEMY_SIZE;

    // 檢查矩形是否重疊
    return !(playerLeft > enemyRight || playerRight < enemyLeft ||
             playerTop > enemyBottom || playerBottom < enemyTop);
}


void GameWindow::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Move the declarations outside the switch statement
    std::ostringstream timeStream;
    std::string timeText;
    std::string scoreText;
    const int maxHealthWidth = 750; // Maximum width of the health bar
    int currentHealthWidth;

    switch (currentState) {
        case MENU:
            menu.draw();
            break;

        case GAME:
            currentTime = static_cast<int>(al_get_time() - startTime);
            timeStream << "Time: " << currentTime;
            timeText = timeStream.str();
            scoreText = "Score: " + std::to_string(score);

            // Draw Health Bar
            currentHealthWidth = (player.getHp() * maxHealthWidth) / 100;
            al_draw_filled_rectangle(10, 600 - 30, 10 + currentHealthWidth, 600 - 10, al_map_rgb(255, 0, 0));

            // Draw text
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 10, 0, timeText.c_str());
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 40, 0, scoreText.c_str());
            
            al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            player.draw();
            for (auto& enemy : enemies) {
                // 这里只绘制活着的敌人
                if (enemy.isAlive()) {
                    enemy.draw();
                }
                // 但是无论敌人是否活着，都绘制其子弹
                for (auto& bullet : enemy.getBullets()) {
                    if (checkCollision(bullet, player)) continue;
                    else bullet.draw();
                    
                }
            }
            break;

        case GAME_OVER:
            // Drawing code for game over screen
            al_draw_text(ui_font, al_map_rgb(255, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTER, "Game Over");
             // 繪製返回按鈕
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), back_button_x, back_button_y, 0, "Back");

            // 繪製退出按鈕
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), exit_button_x, exit_button_y, 0, "Exit");
            break;
    }

    al_flip_display();
}

