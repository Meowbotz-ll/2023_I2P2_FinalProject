#include "player.h"
#include "global.h"
#include "bullet.h"
#include <vector>
#include <cmath>
#include <algorithm>

std::vector<Bullet> bullets;
const float GRAVITY = 0.5;
const float JUMP_STRENGTH = -10.0;
const int PLAYER_SIZE = 32;

Player::Player() : x(0), y(0), dy(0), onGround(true), facingRight(true), bulletSpeed(15.0f) {
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

    // Update bullets
    for (auto& bullet : bullets) {
        bullet.update();
    }
    // 移除不活跃的子弹
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
                [](const Bullet& bullet) { return !bullet.isAlive(); }), bullets.end());
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

std::vector<Bullet>& Player::getBullets() {
    return bullets;
}

