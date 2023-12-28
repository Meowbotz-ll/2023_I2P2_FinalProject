#ifndef PLAYER_H
#define PLAYER_H

#include "allegro_setup.h"

class Player {
public:
    Player();
    void init(float x, float y);
    void update();
    void draw();
    void shoot(float target_x, float target_y);
private:
    float x, y, dy;
    bool onGround, facingRight;
    float bulletSpeed;
};

#endif // PLAYER_H
