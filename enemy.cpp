#include "Enemy.h"

Enemy::Enemy(float x, float y, int initialHP) 
    : x(x), y(y), hp(initialHP), color(al_map_rgb(0, 0, 255)) {} // Blue color

void Enemy::update() {
    // Update logic for enemy (optional for this example)
}

void Enemy::draw() {
    al_draw_filled_circle(x, y, 20, color); // Drawing the enemy as a circle
}

void Enemy::hit(int damage) {
    hp -= damage;
    if (hp > 0) {
        color = al_map_rgb(0, 0, 200); // Change color to indicate damage
    }
}

bool Enemy::isAlive() const {
    return hp > 0;
}