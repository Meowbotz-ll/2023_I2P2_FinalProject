#include "player.h"
#include "global.h"
const float GRAVITY = 0.5;
const float JUMP_STRENGTH = -10.0;
const int PLAYER_SIZE = 32;

Player::Player() : x(0), y(0), dy(0), onGround(true), facingRight(true) {}

void Player::init(float x, float y) {
    this->x = x;
    this->y = y;
}

void Player::update() {
    if (!onGround) {
        dy += GRAVITY;
        y += dy;
    }

    if (y >= 600 - 100 - PLAYER_SIZE) {
        y = 600 - 100 - PLAYER_SIZE;
        dy = 0;
        onGround = true;
    }

    if (key_state[ALLEGRO_KEY_A]) {
        x -= 4.0;
        facingRight = false;
    }
    if (key_state[ALLEGRO_KEY_D]) {
        x += 4.0;
        facingRight = true;
    }

    if (key_state[ALLEGRO_KEY_SPACE] && onGround) {
        dy = JUMP_STRENGTH;
        onGround = false;
    }
}

void Player::draw() {
    al_draw_filled_rectangle(x, y, x + PLAYER_SIZE, y + PLAYER_SIZE, al_map_rgb(255, 0, 0));
}
