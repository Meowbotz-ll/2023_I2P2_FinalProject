#include "player.h"
#include "global.h"
#include "bullet.h"
#include <vector>
#include <cmath>

std::vector<Bullet> bullets;
const float GRAVITY = 0.5;
const float JUMP_STRENGTH = -10.0;
const int PLAYER_SIZE = 32;

Player::Player() : x(0), y(0), dy(0), onGround(true), facingRight(true), bulletSpeed(15.0f),dashing(false),lastDashTime(0.0){
}

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

    if (key_state[ALLEGRO_KEY_F] && !dashing) {
        // Implement dash behavior here
        if(facingRight)
        {
        x += 20.0;
        }
        else if(!facingRight)
        {
        x-=20.0;
        }
        dashing = false;
        lastDashTime = al_get_time(); // Record the time of the dash
    }

        // Check if the cooldown is over and reset the dash flag
    if (al_get_time() - lastDashTime >= DASH_COOLDOWN) {
        dashing = false;
    }

    // Update bullets
    for (auto& bullet : bullets) {
        bullet.update();
    }
}

void Player::draw() {
    al_draw_filled_rectangle(x, y, x + PLAYER_SIZE, y + PLAYER_SIZE, al_map_rgb(255, 0, 0));
        // Draw each bullet
    for (auto& bullet : bullets) {
        bullet.draw();
    }
}

void Player::shoot(float target_x, float target_y) {
    float dx = target_x - x;
    float dy = target_y - y;
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length; // Normalize
    dy /= length;
    
    // Use bulletSpeed to determine the velocity of the bullet
    dx *= bulletSpeed;
    dy *= bulletSpeed;

    bullets.push_back(Bullet(x, y, dx, dy));
}

