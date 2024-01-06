#include "bullet.h"

Bullet::Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}

void Bullet::update() {
    x += dx;
    y += dy;
    // Added: Check if the bullet is out of screen bounds
    if (x < 0 || x > 800 || y < 0 || y > 600) {
        alive = false;
    }
}

void Bullet::draw() {
    al_draw_filled_rectangle(x, y, x + SIZE, y + SIZE, al_map_rgb(255, 255, 255));
}