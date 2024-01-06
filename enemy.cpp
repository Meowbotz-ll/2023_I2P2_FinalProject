#include "Enemy.h"

Enemy::Enemy(float x, float y, int initialHP) 
    : x(x), y(y), hp(initialHP), color(al_map_rgb(255, 255, 0)), alive(true) {} // Changed color to yellow, added alive initialization

void Enemy::update() {
    // Update logic for enemy (optional for this example)
}

void Enemy::draw() {
    if(!alive) return; // Do not draw if not alive
    al_draw_filled_circle(x, y, RADIUS, color); 
}

void Enemy::hit(int damage) {
    hp -= damage;
    if (hp <= 0) {
        alive = false; // Mark enemy as dead
    }
}

bool Enemy::isAlive() const {
    return hp > 0;
}