#include "bullet.h"

int Bullet::DEFAULT_DAMAGE = 2; // 初始化静态成员变量

Bullet::Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy), alive(true), damage(Bullet::DEFAULT_DAMAGE), hit(false) {}

void Bullet::update() {
    x += dx;
    y += dy;
    // Added: Check if the bullet is out of screen bounds
    if (x < 0 || x > 800 || y < 0 || y > 600) {
        alive = false;
    }
}

void Bullet::draw() {
    if(!alive) return;
    al_draw_filled_rectangle(x, y, x + SIZE, y + SIZE, al_map_rgb(255, 255, 255));
}

void Bullet::setAlive(bool status) {
    alive = status;
}
