#ifndef ENEMY_H
#define ENEMY_H

#include "allegro_setup.h"
class Enemy {
public:
    float x, y;
    static int DEFAULT_HP;  // 默认的敌人生命值
    Enemy(float x, float y, float vx);
    void update();
    void draw();
    void hit(int damage);
    bool isAlive() const;
    static const int RADIUS = 10;
    bool isOffScreen() const { return offScreen; }

private:
    float vx, vy; // 新增：敌人的速度
    int hp;
    ALLEGRO_COLOR color;
    bool alive;
    
    bool offScreen; // 新增：敌人是否已离开屏幕
};



#endif // ENEMY_H