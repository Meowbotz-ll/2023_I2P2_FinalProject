#ifndef BULLET_H
#define BULLET_H

#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "allegro_setup.h"
#include "circle.h"
/*class Bullet {
public:
    float x, y;
    float dx, dy;
    static const int SIZE = 10;

    Bullet(float x, float y, float dx, float dy);
    void update();
    void draw();
};
*/



class Bullet {
public:
    Bullet(float x, float y, float dx, float dy);
    
    ~Bullet();
    void update();
    void draw() const;

    bool isAlive() const;  // Add isAlive function
    void hit();            // Add hit function
    float getX() const;    // Add getX function
    float getY() const;    // Add getY function
    Circle hitbox;

private:
    
    float x;
    float y;
    float dx;
    float dy;

    static const float SIZE;
    bool alive;
    ALLEGRO_BITMAP* image;  // 声明 image 变量
};

#endif // BULLET_H
