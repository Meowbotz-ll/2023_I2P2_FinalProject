#ifndef PLAYER_H
#define PLAYER_H

#include "allegro_setup.h"
#include <allegro5/allegro_image.h>

class Player {
public:
    Player();
    ~Player();
    void init(float x, float y);
    void update();
    void draw() const;
    void shoot(float target_x, float target_y);
private:
    float x, y, dy;
    bool onGround, facingRight;
    float bulletSpeed;
    ALLEGRO_BITMAP* image;  // 声明 image 变量
};

#endif // PLAYER_H
