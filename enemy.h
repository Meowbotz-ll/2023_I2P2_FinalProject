#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro_primitives.h>
#include "circle.h"

class Enemy {
public:
    Enemy();
    ~Enemy();

    Circle hitbox;

    void initialize();
    void update();
    void draw() const;

    bool isAlive() const;
    void hit();

    float getX() const;  // Add getX function
    float getY() const;  // Add getY function

private:
    float x;
    float y;
    float speed;
    bool alive;

    ALLEGRO_COLOR color;

    void respawn();
};

#endif // ENEMY_H
