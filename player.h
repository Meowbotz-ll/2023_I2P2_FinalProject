#ifndef PLAYER_H
#define PLAYER_H

#include "allegro_setup.h"
#include "bullet.h"
#include <vector>
#include <string>

enum WeaponType {
        PISTOL,
        SHOTGUN,
        BAZOOKA,
        HEXA_BLASTER,
        BOUNCING_GUN
    };

class Player {
public:
    ~Player();
    Player();
    void init(float x, float y,const char* gifFile[4]);
    void update(int score, ALLEGRO_FONT* font);
    void draw();
    void reset();
    void shoot(float target_x, float target_y);
    std::vector<Bullet>& getBullets(){
        return bullets;
    };  // 获取子弹的方法
    void markBulletDead(int index); // 添加一個新方法來標記特定子彈為不活動
    float getX() const;
    float getY() const;
    int getHp() const { return hp; } // Getter for player's health
    void getHit(int damage); // Method to reduce player's health
    const int PLAYER_SIZE = 32;
    const int INITIAL_HP = 10; // Initial health points
    void drawUnlockMessage(ALLEGRO_FONT* font);
private:
    float x, y, dy;
    bool onGround, facingRight;
    int currentDirection=0;
    float bulletSpeed;
    std::vector<Bullet> bullets;  // 将其作为成员变量
    bool dashing;
    float lastDashTime;
    const float DASH_COOLDOWN = 4.0; // 4-second cooldown
    ALGIF_ANIMATION *walkingGif[2];
    WeaponType currentWeapon;
    int hp; // Player's health points
    bool shotgunUnlocked = false;
    bool bouncingGunUnlocked = false;
    bool hexaBlasterUnlocked = false;
    bool bazookaUnlocked = false;
    std::string unlockMessage = "";
    double messageDisplayTime = 0.0;
};



#endif // PLAYER_H
