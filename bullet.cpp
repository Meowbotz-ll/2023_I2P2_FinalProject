#include "bullet.h"

Bullet::Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy), alive(true), damage(Bullet::DEFAULT_DAMAGE), hit(false) {}

void Bullet::update() {
    x += dx;
    y += dy;

    // 如果子弹是敌人的子弹，且超出屏幕，不设置为不活跃
    if (isEnemyBullet && (x < 0 || x > 800 || y < 0 || y > 600)) {
        // 可能不需要做任何事情
        
    } else {
        if (x < 0 || x > 800 || y < 0 || y > 600) {
            alive = false;
        }
    }
}


void Bullet::draw() {
    if(!alive) return;
    al_draw_filled_rectangle(x, y, x + size, y + size, al_map_rgb(255, 255, 255));
}

void Bullet::setAlive(bool status) {
    alive = status;
}

bool Bullet::is_Alive() const {
    return alive;
}
