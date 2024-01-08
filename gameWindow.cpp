#include "gameWindow.h"
#include "log.h"  // Include your log class
#include "algorithm"
#include "bullet.h"


#include <iterator>
using namespace std;

// 在GameWindow類中添加以下成員變量
const int NUM_MODES = 3;  // 定義模式的數量
ALLEGRO_BITMAP* modeSelectionImages[NUM_MODES];  // 保存模式選擇界面的圖片
int selectedMode = -1;  // 被選擇的模式，初始值為-1表示未選擇

GameWindow::GameWindow() : currentState(MENU),previousState(MENU),backgroundImage(nullptr),doexit(false),gameSceneInitialized(false){
    Log::Info("GameWindow Created");
    init();
    lastBombTime=0;
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
void GameWindow::resetGame() {
    // Reset player stats, position, etc.
    player.reset(); // Assuming there is a reset method in Player class
    bombAvailable=true;
    lastBombTime = 0;
    // Clear and reset enemy list
    enemies.clear();
    // Optionally, initialize first set of enemies

    // Reset game score and other relevant stats
    score = 0;
    // reset timer
    startTime = al_get_time(); // Reset the start time to the current time
    currentTime = 0; // Reset the current time or elapsed time to 0
    // Reset any other game elements that change during gameplay
}
void GameWindow::checkBombCooldown() {
        if (!bombAvailable && al_get_time() - lastBombTime >= bombCooldown) {
            bombAvailable = true;
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
    backgroundImage = al_load_bitmap("img/background.jpg");
    if (!backgroundImage) {
        Log::Error("Failed to load background image");
        return;
    }


    // Init background Music
    menuMusic = al_load_sample("audio/Run-Amok(chosic.com).mp3");
    gameMusic = al_load_sample("audio/Sakura-Girl-Daisy-chosic.com_.mp3");
    bombSound = al_load_sample("audio/bomb_explosion.wav");
    loseMusic = al_load_sample("audio/violin-lose.mp3");

const char* playerGifFiles[2] = {
        "player_img/player_r.gif",   // Replace with actual file path
        "player_img/player_l.gif",  // Replace with actual file path
    };
    enemyGif[0] = "enemy_gif/flying.gif";
    enemyGif[1] = "enemy_gif/enemy_ground_l.gif";
    enemyGif[2] = "enemy_gif/enemy_ground_r.gif";
    enemyGif[3] = "enemy_gif/enemy_ground_l.gif";

// Log the loading of each GIF file
    for (int i = 0; i < 2; ++i) {
        Log::Info(std::string("Loading GIF: ") + playerGifFiles[i]);
    }

if (!menuMusic || !gameMusic||!bombSound) {
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
    std::ifstream inFile("leaderboard.txt");
    leaderboardScores.clear();
    int score;
    while (inFile >> score) {
        leaderboardScores.push_back(score);
    }
    inFile.close();
    Log::Info("Score Loaded");
}
void GameWindow::initLeaderboardScene()
{
 // Load a larger font for the title
    ALLEGRO_FONT* title_font = al_load_font("fonts/ARCADE.TTF", 48, 0);
    if (!title_font) {
        Log::Error("Failed to load title font");
        return; // Early return if font loading fails
    }

    // Draw the title "Leaderboard"
    al_draw_text(title_font, al_map_rgb(255, 215, 0), 400, 50, ALLEGRO_ALIGN_CENTER, "Leaderboard");

    // Logic to display scores
    int yPosition = 150; // Adjusted starting position for displaying scores (below the title)
    int index = 1;
    for (const auto& score : leaderboardScores) {
        std::string scoreLine = std::to_string(index) + ". " + std::to_string(score);
        al_draw_text(ui_font, al_map_rgb(255, 255, 255), 400, yPosition, ALLEGRO_ALIGN_CENTER, scoreLine.c_str());
        yPosition += 30; // Increment position for the next score
        index++;
    }

    // Clean up the title font
    al_destroy_font(title_font);
}
void GameWindow::initModeSelectionScene() {
    for (int i = 0; i < NUM_MODES; ++i) {
        // 根據實際需求替換下面的路徑
        modeSelectionImages[i] = al_load_bitmap(("img/mode" + std::to_string(i + 1) + ".jpg").c_str());
        // 設置模式選擇界面的位置，這裡假設每個模式圖片的寬度為100
        al_draw_bitmap(modeSelectionImages[i], i * 100, 0, 0);
    }
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
        resetGame();
        Log::Info("Game Reset");
        if(backgroundImage!=nullptr)
        {
        al_draw_bitmap(backgroundImage, 0, 0, 0);
        Log::Info("Background Init");
        }
        gameSceneInitialized=true;
    }
    
}
void GameWindow::updateLeaderBoard() {
    std::vector<int> scores;
    std::ifstream inFile("leaderboard.txt");

    // Read existing scores from the file
    int existingScore;
    while (inFile >> existingScore) {
        scores.push_back(existingScore);
    }
    inFile.close();

    // Add the current game's score
    scores.push_back(score);

    // Sort the scores
    std::sort(scores.begin(), scores.end(), std::greater<int>());

    // Write the scores back to the file
    std::ofstream outFile("leaderboard.txt");
    for (const auto& s : scores) {
        outFile << s << std::endl;
    }
    outFile.close();
}
void GameWindow::useBomb() {
if (!bombAvailable) return;

    al_play_sample(bombSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            enemy.hit(bombDamage);  // Apply damage
            if (enemy.hp <= 0) {
                enemy.set_Alive(false);
            }
        }
    }

    bombAvailable = false;
    lastBombTime = al_get_time(); // Record the time when bomb was used
}

void GameWindow::initScene() {
    // Check if the current state is different from the previous state
    if (currentState != previousState) {
        switch (currentState) {
            case MENU:
                initMenuScene();  // Initialize menu-specific resources
                gameSceneInitialized=false;
                break;
            case MODE_SELECTION:
                initModeSelectionScene();
                resetGame();
                gameSceneInitialized=false;
                break;
            case GAME:
                initGameScene();  // Initialize game-specific resources
                gameSceneInitialized=false;
                break;
            case GAME_OVER:
                updateLeaderBoard();
                Log::Info("Updated High score! "+std::to_string(score)+".");
                gameSceneInitialized=false;
                break;
            case LEADERBOARD:
                break;
            // Add other cases as needed
        }
        previousState = currentState;  // Update the previous state
    }
}
void GameWindow::createExplosion(float x, float y) {
    float angleStep = 60.0; // Degrees
    for (int i = 0; i < 6; ++i) {
        float angle = i * angleStep;
        float radian = angle * (M_PI / 180.0); // Convert to radians
        float dx = cos(radian);
        float dy = sin(radian);
        Bullet newBullet(x, y, dx, dy);
        newBullet.setSize(10);
        newBullet.setDamage(3); // Damage of each exploded bullet
        player.getBullets().push_back(newBullet);
    }
}
void GameWindow::run() {
    Log::Info("Game Started!");
    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        initScene();
        checkBombCooldown();
        switch (ev.type) {
            case ALLEGRO_EVENT_TIMER:{
                switch(selectedMode){
                    case 0:
                        mode1();
                        game_player();
                        break;
                    case 1:
                        mode2();
                        game_player();
                        break;
                    case 2:
                        mode3();
                        game_player();
                        break;
                }
                    
                // Add any other updates here, e.g., for game world, enemies, etc.
                break;
            }
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
        }
        switch (currentState) {
            case MENU:
            //Log::Info("In Menu State");
                if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        Log::Info("Enter Key Pressed - Switching to Game State"); // Debug message for pressing Enter
                        al_stop_samples(); // Stop any currently playing music
                        //currentState = GAME;
                        currentState = MODE_SELECTION;
                        //menu.gameStart = true; // Update the flag in menu
                    }
                    if (ev.keyboard.keycode == ALLEGRO_KEY_L) {
                        currentState = LEADERBOARD;
                    }
                }
                //menu.update();
                break;
            case LEADERBOARD:
                initLeaderboardScene();
                // Add logic to return to the menu, for example, by pressing ESCAPE
                if (ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    currentState = MENU;
                }
                break;
            break;        
            case MODE_SELECTION:
                if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                    switch (ev.keyboard.keycode) {
                        case ALLEGRO_KEY_1:
                            selectedMode = 0;
                            Log::Info("Mode 1 Selected");
                            currentState = GAME;
                            break;
                        case ALLEGRO_KEY_2:
                            selectedMode = 1;
                            Log::Info("Mode 2 Selected");
                            currentState = GAME;
                            break;
                        case ALLEGRO_KEY_3:
                            selectedMode = 2;
                            Log::Info("Mode 3 Selected");
                            currentState = GAME;
                            break;
                    }
                }
                break;
            case GAME:
            //Log::Info("In Menu State");

            switch (ev.type) {
            case ALLEGRO_EVENT_TIMER:{
                switch(selectedMode){
                    case 0:
                        mode1();
                        game_player();
                        break;
                    case 1:
                        mode2();
                        game_player();
                        break;
                    case 2:
                        mode3();
                        game_player();
                        break;
                }
                    
                // Add any other updates here, e.g., for game world, enemies, etc.
                break;
            }

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (ev.mouse.button & 1) { // Left mouse button
                    Log::Info("Left Mouse Button Clicked - Shoot");
                    player.shoot(ev.mouse.x, ev.mouse.y);
                }
                if (ev.mouse.button & 2) {  // Right mouse button
                    Log::Info("Right Mouse Button Clicked - Bomb Used");
                    useBomb();
                }
                
                break;
                case GAME_OVER:

                if (ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    Log::Info("Escape Clicked");
                    // Example: Return to menu or close the game
                    currentState=MENU;
                }
                
                break;      
        }


        }
        
        draw();
    }
}

void GameWindow::mode1()
{
    static double last_spawn_time = 0;
    static double enemySpawnInterval = 2.0;     
    if (al_get_time() - last_spawn_time > enemySpawnInterval) {
        last_spawn_time = al_get_time();
        float spawnY;
        float velocityX;
        EnemyType type;

        // 生成地上的敌人
        spawnY = 460; // 与玩家相同的高度
        velocityX = 1.0; // 天上敌人速度的两倍
        type = EnemyType::GROUND;
    

        // 随机决定敌人从左侧或右侧出现
        float spawnX;
        if (rand() % 2 == 0) {
            spawnX = -1; // 从屏幕左侧出现
        } else {
            spawnX = 801; // 从屏幕右侧出现
            velocityX = -velocityX; // 改变移动方向
        }

        // 添加敌人到列表
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type,enemyGif));//air
        enemySpawnInterval = std::max(1.0, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    ground_enemy();
}

void GameWindow::mode2()
{
    static double last_spawn_time = 0;
    static double enemySpawnInterval = 5.0;     
    if (al_get_time() - last_spawn_time > enemySpawnInterval) {
        last_spawn_time = al_get_time();
        float spawnY;
        float velocityX;
        EnemyType type;

        // 生成天上的敌人
        spawnY = static_cast<float>(rand() % 300); // 屏幕上半部
        velocityX = 1.0; // 初始速度
        type = EnemyType::AIR;

        // 随机决定敌人从左侧或右侧出现
        float spawnX;
        if (rand() % 2 == 0) {
            spawnX = -1; // 从屏幕左侧出现
        } else {
            spawnX = 801; // 从屏幕右侧出现
            velocityX = -velocityX; // 改变移动方向
        }

        // 添加敌人到列表
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type,enemyGif));//air
        enemySpawnInterval = std::max(1.0, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    sky_enemy();
}

void GameWindow::sky_enemy()
{
    for (auto& enemy : enemies) {
        enemy.update(); // 更新敌人位置和状态

        // 天空中的敌人继续射击
        if (enemy.getType() == EnemyType::AIR && enemy.isAlive()) {
            enemy.shootAtPlayer(player);
        }
    }

    // 遍历每个敌人的子弹
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.getBullets()) {
            bullet.update(); // 更新子弹位置
            // 检查子弹是否击中玩家
            if (bullet.is_Alive() && checkCollision(bullet, player)) {
                player.getHit(1);
                bullet.setAlive(false); // 子弹击中玩家后消失
            }
        }
    }

    // 删除所有非活动的子弹
    for (auto& enemy : enemies) {
        enemy.removeInactiveBullets();
    }
}

void GameWindow::ground_enemy()
{
    for (auto& enemy : enemies) {
        enemy.update();
        if (enemy.isAlive() && checkCollision(player, enemy)) {
            player.getHit(1);
            enemy.set_Alive(false);
        }
    }
}

void GameWindow::mode3()
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
            spawnY = 460; // 与玩家相同的高度
            velocityX = 1.0; // 天上敌人速度的两倍
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
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type,enemyGif));//air
        enemySpawnInterval = std::max(0.7, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    ground_enemy();
    sky_enemy();
}


void GameWindow::game_player()
{
    player.update(score);
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
                if(bullet.isExploding())
                {
                    createExplosion(bullet.x,bullet.y);
                }
                bullet.setHit(true); // 标记子弹已经击中敌人
                if(bullet.isBouncing)
                {
                    bullet.dx = -bullet.dx;
                    bullet.dy = -bullet.dy;
                    bullet.bounceCount--;
                    if(bullet.bounceCount<=0)
                    {
                        bullet.setAlive(false);
                    }
                }
                else
                {

                bullet.setAlive(false); // 设置子弹为不活跃
                }
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

// 修改遊戲循環的一部分以更新 GIF
/*void GameWindow::background_draw() {
    //ALGIF_ANIMATION * backgroundGIF = algif_load_animation("img/background.gif");
    ALLEGRO_BITMAP* frameBitmap = algif_get_bitmap(backgroundGIF, al_get_time());
    if (frameBitmap) {
        // Get the dimensions of the display
        int screenWidth = al_get_display_width(display);
        int screenHeight = al_get_display_height(display);

        // Draw the bitmap to cover the entire screen
        al_draw_bitmap(frameBitmap, 0, 0, 0);
    }
}*/


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
            al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            break;
        
        case MODE_SELECTION:
                    // Display instructions for choosing different levels
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTER, "Press 1 to choose Ground Level");
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTER, "Press 2 to choose Sky Level");
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Press 3 to choose Hard Level");
            break;

        case GAME:
            // Draw background first
            /*if(backgroundImage != nullptr) {
                al_draw_bitmap(backgroundImage, 0, 0, 0);
            }*/


            //al_draw_bitmap(backgroundImage, 0, 0, 0);
            currentTime = static_cast<int>(al_get_time() - startTime);
            timeStream << "Time: " << currentTime;
            timeText = timeStream.str();
            scoreText = "Score: " + std::to_string(score);

            float healthPercentage;
            int red, green;

            // ... other parts of the function ...

            // Inside your control structure where you are drawing the health bar:
            healthPercentage = static_cast<float>(player.getHp()) / static_cast<float>(player.INITIAL_HP);
            red = static_cast<int>(255 * (1 - healthPercentage)); // Increases as health decreases
            green = static_cast<int>(255 * healthPercentage); // Decreases as health decreases

            // Now draw the health bar with the calculated colors
            currentHealthWidth = (player.getHp() * maxHealthWidth) / player.INITIAL_HP;
            al_draw_filled_rectangle(10, 600 - 30, 10 + currentHealthWidth, 600 - 10, al_map_rgb(red, green, 0));

            // Draw text
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 10, 0, timeText.c_str());
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 40, 0, scoreText.c_str());
            
            al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            

            
            player.draw();
        if (!bombAvailable) {
            double remainingCooldown = bombCooldown - (al_get_time() - lastBombTime);
            remainingCooldown = std::max(0.0, remainingCooldown); // Avoid negative values
            std::ostringstream cooldownStream;
            cooldownStream.precision(1); // Set precision for decimal seconds
            cooldownStream << std::fixed << "Bomb Cooldown: " << remainingCooldown << "s";
            std::string cooldownText = cooldownStream.str();
            al_draw_text(ui_font, al_map_rgb(255, 0, 0), 10, 70, 0, cooldownText.c_str());
        }
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

        case LEADERBOARD:
            initLeaderboardScene();
            break;
        case GAME_OVER:
            al_play_sample(loseMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            // Drawing code for game over screen
            al_draw_text(ui_font, al_map_rgb(255, 0, 0), 400, 300, ALLEGRO_ALIGN_CENTER, "Game Over");
             // 繪製返回按鈕
            std::string highScoreText = "High Score: " + std::to_string(score);
            al_draw_text(ui_font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTER, highScoreText.c_str());
            break;

    }

    al_flip_display();
}


