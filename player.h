#ifndef PLAYER_H
#define PLAYER_H

#include "allegro_setup.h"

class Player {
public:
    Player();
    void init(float x, float y);
    void update();
    void draw();
private:
    float x, y, dy;
    bool onGround, facingRight;
};

#endif // PLAYER_H
