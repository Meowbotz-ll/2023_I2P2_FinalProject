#ifndef ENEMY_H
#define ENEMY_H

#include "allegro_setup.h"
class Enemy {
public:
    float x, y;
    static int DEFAULT_HP;  // 默认的敌人生命值
    Enemy(float x, float y);
    void update();
    void draw();
    void hit(int damage);
    bool isAlive() const;
    static const int RADIUS = 10;

private:
    
    int hp;
    ALLEGRO_COLOR color;
    bool alive;
};



#endif // ENEMY_H