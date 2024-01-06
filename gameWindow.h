#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "player.h"
#include "global.h"
#include "bullet.h"
#include "enemy.h"
#include "log.h"

#include <vector>


class GameWindow {
public:
    GameWindow();
    ~GameWindow();
    void init();
    void run();
    bool checkCollision(const Bullet& bullet, const Enemy& enemy);  // 添加参数
    std::vector<Bullet>& getBullets();  // 获取子弹的方法
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    Player player;
    std::vector<Enemy> enemies;  // 敌人的容器
    std::vector<Bullet> bullets;  // 将其作为成员变量
    bool doexit;
    void update();
    void draw();
    
};

#endif // GAME_WINDOW_H
