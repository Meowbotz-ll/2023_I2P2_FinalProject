#include "player.h"
#include "global.h"
#include "bullet.h"
#include "log.h"
#include <vector>
#include <cmath>

std::vector<Bullet> bullets;
const float GRAVITY = 0.5;
const float JUMP_STRENGTH = -10.0;
const int PLAYER_SIZE = 32;

Player::Player() : x(0), y(0), dy(0), onGround(true), facingRight(true), bulletSpeed(15.0f) {
    image = al_load_bitmap("pic/player.png");
    if (!image) {
        Log::Error("Failed to load player image");
    }
}

Player::~Player() {
    if (image) {
        al_destroy_bitmap(image);
    }
    // 注意：如果 Player 類中也有子彈的列表，也需要釋放這些子彈的資源。
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
        dy = JUMP_STRENGTH; // 設定向上的速度
        onGround = false;   // 標記玩家不再在地面上
    }

    // Update bullets
    for (auto& bullet : bullets) {
        bullet.update();
    }
}

void Player::draw() const {
    if (image) {
        al_draw_bitmap(image, x, y, 0);
    }
    // 繪製子彈
    for (const auto& bullet : bullets) {
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

