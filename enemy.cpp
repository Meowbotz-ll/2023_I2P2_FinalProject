#include "enemy.h"
#include "global.h"
#include "bullet.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy()
    : speed(2.0), alive(false), color(al_map_rgb(255, 255, 0)) ,hitbox(0, 0, 0/*ENEMY_RADIUS*/){ // Yellow color
    std::srand(std::time(0)); // Seed for random number generation.
    respawn();
}

Enemy::~Enemy() {
    // Clean up resources if needed.
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
    }
}

void Enemy::draw() const{
    if (alive) {
        // Draw a yellow circle representing the enemy.
        al_draw_filled_circle(x + 10, y + 10, 10, color);
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
}
