#include "Enemy.h"
#include "player.h"

#include <cmath>
#include <algorithm>

Enemy::Enemy(float x, float y, float vx, EnemyType type) 
    : x(x), y(y), hp(DEFAULT_HP), vx(vx), color(al_map_rgb(255, 255, 0)), alive(true), offScreen(false), type(type), lastShootTime(0) {}


void Enemy::update() {
    if(type == AIR) x += vx;
    else x += 2*vx;
    
    // 检查是否离开屏幕
    if (x < 0 || x > 800) {  // 假设屏幕宽度为800
        offScreen = true;
    }

    // Update bullets
    for (auto& bullet : bullets) {
        bullet.update();
    }
    
}

void Enemy::draw() {
    if(!alive) return;
    ALLEGRO_COLOR color = (type == AIR) ? al_map_rgb(173, 216, 230) : al_map_rgb(255, 165, 0);
    al_draw_filled_circle(x, y, RADIUS, color); // 根据类型绘制不同颜色的圆
    // Draw each bullet
    for (auto& bullet : bullets) {
        bullet.draw();
    }
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

void Enemy::shootAtPlayer(const Player& player) {
    //static double lastShootTime = 0;
    double shootInterval = 2.0; // 子弹发射间隔，以秒为单位

    // 当前时间
    double currentTime = al_get_time();

    // 检查是否到达发射间隔
    if (currentTime - lastShootTime > shootInterval) {
        // 重置发射时间
        lastShootTime = currentTime;

        // 发射子弹的逻辑
        if (type == AIR) {
            float dx = player.getX() - x;
            float dy = player.getY() - y;
            float distance = sqrt(dx * dx + dy * dy);
            dx /= distance;
            dy /= distance;

            // 设置子弹速度
            float bulletSpeed = 2.0f; // 减慢子弹速度

            Bullet newBullet(x, y, dx * bulletSpeed, dy * bulletSpeed);
            newBullet.setSize(10); // 例如，设置子弹大小为10
            bullets.push_back(newBullet);
        }
    }
}


  
    

void Enemy::drawBullets() {
    if(type == AIR) {
    al_draw_filled_rectangle(x, y, x + 10, y + 10, al_map_rgb(255, 255, 255));
    }
}


EnemyType Enemy::getType() const {
    return type;
}

void Enemy::removeInactiveBullets() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                   [](const Bullet& bullet) { return !bullet.isAlive(); }),
                   bullets.end());
}

