#ifndef ENEMY_H
#define ENEMY_H

#include "allegro_setup.h"

const int DEFAULT_HP=7;
class Enemy {
public:
    float x, y;
    int hp;  // 默认的敌人生命值
    Enemy(float x, float y);
    void update();
    void draw();
    void hit(int damage);
    bool isAlive() const;
    static const int RADIUS = 10;

private:
    ALLEGRO_COLOR color;
    bool alive;
};



#endif // ENEMY_H