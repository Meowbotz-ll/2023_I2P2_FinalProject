#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "circle.h"

const int ENEMY_RADIUS = 100;

class Enemy {
public:
    Enemy();
    ~Enemy();

    void initialize();
    void update();
    void draw() const;

    bool isAlive() const;
    void hit();

    float getX() const;  // Add getX function
    float getY() const;  // Add getY function
    
    const Circle& getHitbox() const { return hitbox; }


private:
    float x;
    float y;
    float speed;
    bool alive;

    ALLEGRO_COLOR color;
    ALLEGRO_BITMAP* image;  // 声明 image 变量
    Circle hitbox;

    void respawn();
};

#endif // ENEMY_H
