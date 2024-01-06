#ifndef ENEMY_H
#define ENEMY_H

#include "allegro_setup.h"
class Enemy {
public:
    Enemy(float x, float y, int initialHP);
    void update();
    void draw();
    void hit(int damage);
    bool isAlive() const;

private:
    float x, y;
    int hp;
    ALLEGRO_COLOR color;
};

#endif // ENEMY_H