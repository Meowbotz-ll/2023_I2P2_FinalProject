#include "bullet.h"

Bullet::Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy), alive(true), damage(Bullet::DEFAULT_DAMAGE), hit(false) {}

void Bullet::update() {
    x += dx; // 假設 speed 是成員變量
    y += dy;
    // Added: Check if the bullet is out of screen bounds
    if (x < 0 || x > 800 || y < 0 || y > 600) {
        alive = false;
    }
}

void Bullet::draw() {
    if(!alive) return;
    al_draw_filled_rectangle(x, y, x + size, y + size, al_map_rgb(255, 255, 255));
}

void Bullet::setAlive(bool status) {
    alive = status;
}
