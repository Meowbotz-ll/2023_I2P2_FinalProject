#ifndef ENEMY_H
#define ENEMY_H

#include "allegro_setup.h"
class Enemy {
public:
    float x, y;
    Enemy(float x, float y, int initialHP);
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