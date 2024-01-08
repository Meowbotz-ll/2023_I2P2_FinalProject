#ifndef GAME_H
#define GAME_H

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
//#include "gameWindow.h"


class Game {
public:
    //Game();
    //~Game();
    bool checkCollision(const Bullet& bullet, const Enemy& enemy);  // 添加参数
    std::vector<Bullet>& getBullets();  // 获取子弹的方法
    bool checkCollision(const Bullet& bullet, const Player& player);
    void sky_enemy();
    void ground_enemy();
    void game_player();
    bool checkCollision(const Player& player, const Enemy& enemy);
    void mode1();
    void mode2();
    void mode3();
    void draw();
private:
    //GameWindow window;
    Player player;
    std::vector<Enemy> enemies;  // 敌人的容器
    std::vector<Bullet> bullets;  // 将其作为成员变量
    Menu menu;
    //void update();
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* ui_font; // Font for drawing UI text
    int score=0;             // Player's current score
    //double startTime;      // Time when the game started
    //double currentTime;    // Current game time
    ALLEGRO_SAMPLE *gameMusic;
    const char* enemyGif[4];
};

#endif // GAME_WINDOW_H
