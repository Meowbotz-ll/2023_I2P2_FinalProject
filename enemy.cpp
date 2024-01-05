#include "enemy.h"
#include "global.h"
#include "bullet.h"
#include "log.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy()
    : x(0), y(0), speed(2.0), alive(false), color(al_map_rgb(255, 255, 0)) ,hitbox(x, y, ENEMY_RADIUS){ // Yellow color
    std::srand(std::time(0)); // Seed for random number generation.
    //image = al_load_bitmap("pic/enemy.jpg");
    if (!image) {
        Log::Error("Failed to load enemy image");
    }
    respawn();
}

Enemy::~Enemy() {
    if (image) {
        al_destroy_bitmap(image);
    }
}

void Enemy::initialize() {
    respawn();
}

void Enemy::update() {
    if (alive) {
        x += speed;

        // Check if the enemy is out of the screen.
        if (x > al_get_display_width(al_get_current_display())) {
            respawn();
        }
        hitbox.x = x;
        hitbox.y = y;
    }
}

void Enemy::draw() const {
    if (alive && image) {
        al_draw_bitmap(image, x, y, 0);
    }
}

bool Enemy::isAlive() const {
    return alive;
}

void Enemy::hit() {
        alive = false;
}

float Enemy::getX() const {
    return x;
}

float Enemy::getY() const {
    return y;
}

void Enemy::respawn() {
    x = 0 - rand() % 400; // Randomize initial x position off the screen.
    y = rand() % (al_get_display_height(al_get_current_display()) - 20);
    alive = true;
    hitbox.x = x;
    hitbox.y = y;
}
