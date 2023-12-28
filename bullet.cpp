#include "bullet.h"

Bullet::Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy) {}

void Bullet::update() {
    x += dx;
    y += dy;
}

void Bullet::draw() {
    al_draw_filled_rectangle(x, y, x + SIZE, y + SIZE, al_map_rgb(255, 255, 255));
}
