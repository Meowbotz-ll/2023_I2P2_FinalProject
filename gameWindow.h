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

#include "menu.h"

enum GameState {
    MENU,
    GAME
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
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    Player player;
    std::vector<Enemy> enemies;  // 敌人的容器
    std::vector<Bullet> bullets;  // 将其作为成员变量
    GameState currentState;
    ALLEGRO_FONT* font;
    ALLEGRO_SAMPLE *menuMusic;
    ALLEGRO_SAMPLE *gameMusic;
    Menu menu;
    bool doexit;
    void update();
    ALLEGRO_FONT* ui_font; // Font for drawing UI text
    int score;             // Player's current score
    double startTime;      // Time when the game started
    double currentTime;    // Current game time
    
};

#endif // GAME_WINDOW_H
