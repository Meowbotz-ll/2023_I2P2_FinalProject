#include "bullet.h"
#include "log.h"


const float Bullet::SIZE = 10.0f;

Bullet::Bullet(float x, float y, float dx, float dy)
    : x(x), y(y), dx(dx), dy(dy), hitbox(x, y, Bullet::SIZE), alive(true) {
    image = al_load_bitmap("C:/I2P2_final_project/2023_I2P2_FinalProject/bullet.jpg");
    if (!image) {
        Log::Error("Failed to load bullet image");
    }
}

    
Bullet::~Bullet() {
    if (image) {
        al_destroy_bitmap(image);
    }
}

void Bullet::update() {
    if (alive) {
        x += dx;
        y += dy;
        hitbox.x = x;
        hitbox.y = y;
    }
}

void Bullet::draw() const {
    if (alive && image) {
        al_draw_bitmap(image, x, y, 0);
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
