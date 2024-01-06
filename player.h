#ifndef PLAYER_H
#define PLAYER_H

#include "allegro_setup.h"
#include "bullet.h"
#include <vector>

class Player {
public:
    Player();
    void init(float x, float y);
    void update();
    void draw();
    void shoot(float target_x, float target_y);
    std::vector<Bullet>& getBullets();  // 获取子弹的方法
private:
    float x, y, dy;
    bool onGround, facingRight;
    float bulletSpeed;
    std::vector<Bullet> bullets;  // 将其作为成员变量
};



#endif // PLAYER_H
