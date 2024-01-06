#include "Enemy.h"

Enemy::Enemy(float x, float y, int initialHP) 
    : x(x), y(y), hp(initialHP), color(al_map_rgb(0, 0, 255)), alive(true) {} // 初始化alive为true // Blue color

void Enemy::update() {
    // Update logic for enemy (optional for this example)
}

void Enemy::draw() {
    if(!alive) return;
    al_draw_filled_circle(x, y, RADIUS, color); // Drawing the enemy as a circle
}

void Enemy::hit(int damage) {
    hp -= damage;
    if (hp <= 0) {
        color = al_map_rgb(0, 0, 200); // Change color to indicate damage
        alive = false; // 標記敵人為死亡，不再繪製或更新
    }
}

bool Enemy::isAlive() const {
    return hp > 0;
}