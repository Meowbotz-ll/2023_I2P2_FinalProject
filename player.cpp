#include "player.h"
#include "global.h"
#include "bullet.h"
#include "log.h"
#include <vector>
#include <cmath>
#include <algorithm>

const float GRAVITY = 0.5;
const float JUMP_STRENGTH = -10.0;
const int INITIAL_HP = 100; // Initial health points
const int INITIAL_X =400;
const int INITIAL_Y =500;
Player::Player() {
    for (int i = 0; i < 4; ++i) {
        walkingGif[i] = nullptr;
    }
}

Player::~Player() {
    for (int i = 0; i < 2; ++i) {
        if (walkingGif[i] != nullptr) {
            algif_destroy_animation(walkingGif[i]);
        }
    }
}
void Player::init(float startX, float startY,const char* gifFile[2]) {
    x = startX;
    y = startY;
    dy = 0;
    onGround = true;
    facingRight = true;
    bulletSpeed = 15.0f;
    dashing = false;
    lastDashTime = 0.0;
    currentWeapon = PISTOL;
    hp = INITIAL_HP;

    for(int i = 0; i < 2; i++) {
        walkingGif[i] = algif_load_animation(gifFile[i]);
        if (!walkingGif[i]) {
            Log::Error("Failed to load GIF: " + std::string(gifFile[i]));
        }
    }
    
}

float Player::getX() const{
    return x;
}

float Player::getY() const{
    return y;
}


void Player::getHit(int damage) {
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
    }
}

void Player::update(int score) {

    if (key_state[ALLEGRO_KEY_1]) {
        currentWeapon = PISTOL;
        Log::Info("Weapon change to pistol");
    }
    if (key_state[ALLEGRO_KEY_2] && score>=5) {
        currentWeapon = SHOTGUN;
        Log::Info("Weapon change to shotgun");
    }
    if (key_state[ALLEGRO_KEY_3] && score>=10) {
        currentWeapon = BAZOOKA;
        Log::Info("Weapon change to bazooka");
    }

    if (!onGround) {
        dy += GRAVITY;
        y += dy;
    }

    if (y >= 600 - 100 - PLAYER_SIZE) {
        y = 600 - 100 - PLAYER_SIZE;
        dy = 0;
        onGround = true;
    }

    if (key_state[ALLEGRO_KEY_A]) {
        x -= 4.0;
        facingRight = false;
        currentDirection = 0; // Assuming left direction
    }
    if (key_state[ALLEGRO_KEY_D]) {
        x += 4.0;
        facingRight = true;
        currentDirection = 1; // Assuming left direction
    }

    if (key_state[ALLEGRO_KEY_SPACE] && onGround) {
        dy = JUMP_STRENGTH;
        onGround = false;
    }

    if (key_state[ALLEGRO_KEY_F] && !dashing) {
        // Implement dash behavior here
        if(facingRight)
        {
        x += 20.0;
        }
        else if(!facingRight)
        {
        x-=20.0;
        }
        dashing = false;
        lastDashTime = al_get_time(); // Record the time of the dash
    }

        // Check if the cooldown is over and reset the dash flag
    if (al_get_time() - lastDashTime >= DASH_COOLDOWN) {
        dashing = false;
    }

    // Update bullets
    for (auto& bullet : bullets) {
        bullet.update();
    }


}

void Player::draw() {
     //al_draw_filled_rectangle(x, y, x + PLAYER_SIZE, y + PLAYER_SIZE, al_map_rgb(255, 0, 0));

     ALLEGRO_BITMAP* frameBitmap = algif_get_bitmap(walkingGif[currentDirection], al_get_time());
     if (frameBitmap) {
        // Get the dimensions of the bitmap
        int bitmapWidth = al_get_bitmap_width(frameBitmap);
        int bitmapHeight = al_get_bitmap_height(frameBitmap);

        // Calculate the position to draw the bitmap so that it's centered on the player's coordinates
        float drawX = x - bitmapWidth / 2.0f;
        float drawY = y - bitmapHeight / 2.0f;

        // Draw the bitmap
        al_draw_bitmap(frameBitmap, drawX, drawY, 0);
    }
        // Draw each bullet
    for (auto& bullet : bullets) {
        bullet.draw();
    }
}

void Player::shoot(float target_x, float target_y) {
    float dx = target_x - x;
    float dy = target_y - y;
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length; // Normalize
    dy /= length;

    dx *= bulletSpeed;
    dy *= bulletSpeed;

    switch (currentWeapon) {
        case PISTOL: {
            Bullet newBullet(x, y, dx, dy);
            newBullet.setDamage(2);
            newBullet.setSize(10);
            bullets.push_back(newBullet);
            break;
        }
        case SHOTGUN: {
            for (int i = 0; i < 3; ++i) {
                float angle = i * 0.1f - 0.1f; // Adjust angle for each bullet
                float newDx = dx * std::cos(angle) - dy * std::sin(angle);
                float newDy = dy * std::cos(angle) + dx * std::sin(angle);
                Bullet newBullet(x, y, newDx, newDy);
                newBullet.setSize(20);
                newBullet.setDamage(1);
                bullets.push_back(newBullet);
            }
            break;
        }
        case BAZOOKA: {
            Bullet bazookaBullet(x, y, dx, dy);
            bazookaBullet.setSize(50); // Adjusted size for the bazooka bullet
            bazookaBullet.setDamage(100);
            bullets.push_back(bazookaBullet);
            break;
        }
    }
}

void Player::markBulletDead(int index) {
    // 将 index 转换为无符号类型
    auto unsignedIndex = static_cast<std::vector<Bullet>::size_type>(index);

    if (index >= 0 && unsignedIndex < bullets.size()) {
        // 使用公共方法来设置子弹的状态
        bullets[unsignedIndex].setAlive(false);
    }
}

void Player::reset()
{
    hp=INITIAL_HP;
    x=INITIAL_X;
    y=INITIAL_Y;
}