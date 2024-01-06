// File: .\bullet.h
#pragma once
#include <allegro5/allegro.h>
#include "allegro_setup.h"
#include <vector>

class Bullet {
public:
    float x, y;
    float dx, dy;
    static const int SIZE = 10;
    bool alive;
    bool isAlive() const{
        return alive;
    }
    Bullet(float x, float y, float dx, float dy); // 现有构造函数
    void update();
    void draw();
    

    
};