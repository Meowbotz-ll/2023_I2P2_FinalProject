// File: .\bullet.h
#pragma once
#include <allegro5/allegro.h>
#include "allegro_setup.h"

class Bullet {
public:
    float x, y;
    float dx, dy;
    static const int SIZE = 10;

    Bullet(float x, float y, float dx, float dy);
    void update();
    void draw();
};
