#include "bullet.h"


const float Bullet::SIZE = 10.0f;

Bullet::Bullet(float x, float y, float dx, float dy)
    : x(x), y(y), dx(dx), dy(dy), alive(true) {}

void Bullet::update() {
    if (alive) {
        x += dx;
        y += dy;
    }
}

void Bullet::draw() const {
    if (alive) {
        al_draw_filled_rectangle(x, y, x + SIZE, y + SIZE, al_map_rgb(255, 255, 255));
    }
}

bool Bullet::isAlive() const {
    return alive;
}

void Bullet::hit() {
    alive = false;
}

float Bullet::getX() const {
    return x;
}

float Bullet::getY() const {
    return y;
}
