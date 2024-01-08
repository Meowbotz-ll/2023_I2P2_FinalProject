#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "player.h"
#include "global.h"
#include "bullet.h"
#include "enemy.h"
#include "log.h"
#include "allegro_setup.h"
#include <string>
#include <vector>
#include <sstream> // Include this header for ostringstream
#include <iomanip>
#include "menu.h"
#include <fstream>
#include <algorithm>

using namespace std;
enum GameState {
    MENU,
    MODE_SELECTION,
    GAME,
    GAME_OVER,
    LEADERBOARD
};

enum MODE {
    MODE_1,
    MODE_2,
    MODE_3
};

class GameWindow {
public:
    GameWindow();
    ~GameWindow();
    void init();
    void run();
    bool checkCollision(const Bullet& bullet, const Enemy& enemy);  // 添加参数
    std::vector<Bullet>& getBullets();  // 获取子弹的方法
    void draw();
    void initScene();
    void initMenuScene();
    void initGameScene();
    void initGameOverScene();
    void updateGameOverScene();
    bool checkCollision(const Bullet& bullet, const Player& player);
    void sky_enemy();
    void ground_enemy();
    void game_player();
    bool checkCollision(const Player& player, const Enemy& enemy);
    void mode1();
    void mode2();
    void mode3();
    void initModeSelectionScene();
    void resetGame();
    void initLeaderboardScene();
    void updateLeaderBoard();
    void background_draw();
    
    void useBomb();
    void checkBombCooldown();
    void createExplosion(float x,float y);
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    Player player;
    std::vector<Enemy> enemies;  // 敌人的容器
    std::vector<Bullet> bullets;  // 将其作为成员变量
    GameState currentState;
    GameState previousState;
    ALLEGRO_FONT* font;
    ALLEGRO_SAMPLE *menuMusic;
    ALLEGRO_SAMPLE *gameMusic;
    ALLEGRO_SAMPLE *loseMusic;
    ALLEGRO_SAMPLE *bombSound;
    ALLEGRO_BITMAP *backgroundImage;
    ALGIF_ANIMATION* backgroundGIF;
    Menu menu;
    const char* enemyGif[4];
    bool doexit;
    void update();
    ALLEGRO_FONT* ui_font; // Font for drawing UI text
    int score=0;             // Player's current score
    double startTime;      // Time when the game started
    double currentTime;    // Current game time
    bool gameSceneInitialized;
    float back_button_x, back_button_y;  // 返回按鈕位置
    float exit_button_x, exit_button_y;  // 退出按鈕位置
    float button_width, button_height;   // 按鈕大小
    vector<int> leaderboardScores;
    bool bombAvailable = true;  // Initially available, can add cooldown logic
    const int bombDamage = 10000;
    double lastBombTime;
    const int bombCooldown=30;
};

#endif // GAME_WINDOW_H
