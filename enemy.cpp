#include "Enemy.h"

int Enemy::DEFAULT_HP = 7;  // 初始化静态成员变量

Enemy::Enemy(float x, float y) 
    : x(x), y(y), hp(Enemy::DEFAULT_HP), color(al_map_rgb(255, 255, 0)), alive(true) {} // 初始化alive为true // Blue color

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
        alive = false; // 標記敵人為死亡，不再繪製或更新
    }
}

bool Enemy::isAlive() const {
    return hp > 0;
}