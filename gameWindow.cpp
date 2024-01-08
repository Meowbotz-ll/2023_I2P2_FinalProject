#include "gameWindow.h"
#include "log.h"  // Include your log class
#include "algorithm"
#include "bullet.h"
#include "game.h"


#include <iterator>
using namespace std;

// 在GameWindow類中添加以下成員變量
const int NUM_MODES = 3;  // 定義模式的數量
ALLEGRO_BITMAP* modeSelectionImages[NUM_MODES];  // 保存模式選擇界面的圖片
int selectedMode = -1;  // 被選擇的模式，初始值為-1表示未選擇
//const char* enemyGif[4];

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
    //gameMusic = al_load_sample("audio/Electroman-Adventures.mp3");

const char* playerGifFiles[2] = {
        "player_img/skeleton_left.gif",   // Replace with actual file path
        "player_img/skeleton_right.gif",  // Replace with actual file path
    };
    /*enemyGif[0] = "enemy_gif/flying.gif";
    enemyGif[1] = "enemy_gif/walking_r.gif";
    enemyGif[2] = "enemy_gif/walking_l.gif";
    enemyGif[3] = "enemy_gif/walking_r.gif";*/

// Log the loading of each GIF file
    for (int i = 0; i < 2; ++i) {
        Log::Info(std::string("Loading GIF: ") + playerGifFiles[i]);
    }

if (!menuMusic /*|| !gameMusic*/) {
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

void GameWindow::initModeSelectionScene() {
    // 显示 "Mode" 文字
    al_draw_text(ui_font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Mode");

    for (int i = 0; i < NUM_MODES; ++i) {
        // 根据实际需要替换下面的路径
        modeSelectionImages[i] = al_load_bitmap(("img/mode" + std::to_string(i + 1) + ".jpg").c_str());
        if (!modeSelectionImages[i]) {
            Log::Error("Failed to load mode selection image " + std::to_string(i + 1));
            // 可以加入错误处理代码
        }
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
            case MODE_SELECTION:
                initModeSelectionScene();
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
                        //currentState = GAME;
                        currentState = MODE_SELECTION;
                        //menu.gameStart = true; // Update the flag in menu
                    }
                }
                menu.update();
                break;
                    
            case MODE_SELECTION:
                /*if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
                    int mouseX = ev.mouse.x;
                    int mouseY = ev.mouse.y;
                    for (int i = 0; i < NUM_MODES; ++i) {
                        if (mouseX >= i * 100 && mouseX < (i + 1) * 100) {
                            selectedMode = i;
                            Log::Info("Selected Mode: " + std::to_string(selectedMode + 1));
                            currentState = GAME;  // 切换到游戏状态
                            break;
                        }
                    }
                }*/
                // ... other code
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
            Log::Info("GAME START");
            //Log::Info("In Menu State");
            /*if (player.getHp() <= 0) {
                //Log::Info("Game Over!");
                currentState = GAME_OVER;
            }*/

            switch (ev.type) {
            case ALLEGRO_EVENT_TIMER:
                switch(selectedMode){
                    case 0: //MODE_1
                        game.mode1();
                        game.game_player();
                        break;
                    case 1: //MODE_2
                        game.mode2();
                        game.game_player();
                        break;
                    case 2: //MODE_3
                        game.mode3();
                        game.game_player();
                        break;
                }
                if (player.getHp() <= 0) {
                    Log::Info("Game Over!");
                    currentState = GAME_OVER;
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
        
        case MODE_SELECTION:
            // Draw mode selection screen
            // For example, draw each mode image
            /*for (int i = 0; i < NUM_MODES; ++i) {
                al_draw_bitmap(modeSelectionImages[i], i * 100, 0, 0);
            }*/
            break;

        case GAME:
            //game.draw();
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
                    if (game.checkCollision(bullet, player)) continue;
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

void GameWindow::setCurrentState(GameState state) {
    currentState = state;
}
