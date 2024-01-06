#ifndef PLAYER_H
#define PLAYER_H

#include "allegro_setup.h"
#include "bullet.h"
#include <vector>

class Player {
public:
    ~Player();
    Player();
    void init(float x, float y,const char* gifFile[4]);
    void update();
    void draw();
    void shoot(float target_x, float target_y);
    std::vector<Bullet>& getBullets(){
        return bullets;
    };  // 获取子弹的方法
    void markBulletDead(int index); // 添加一個新方法來標記特定子彈為不活動
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
    
};



#endif // PLAYER_H
