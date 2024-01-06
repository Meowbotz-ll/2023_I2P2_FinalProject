#include "Enemy.h"


Enemy::Enemy(float x, float y, float vx) 
    : x(x), y(y), vx(vx), hp(DEFAULT_HP), color(al_map_rgb(255, 255, 0)), alive(true), offScreen(false) {}

void Enemy::update() {
    x += vx;

    // 检查是否离开屏幕
    if (x < 0 || x > 800) {  // 假设屏幕宽度为800
        offScreen = true;
    }
    
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