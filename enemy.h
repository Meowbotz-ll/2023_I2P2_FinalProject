#ifndef ENEMY_H
#define ENEMY_H

#include "allegro_setup.h"
#include "player.h"
#include "bullet.h"

#include <vector>

class Player;

enum EnemyType{
    GROUND,
    AIR
};

const int DEFAULT_HP=7;
class Enemy {
public:
    float x, y;
    int hp;  // 默认的敌人生命值
    //EnemyType type;
    Enemy(float x, float y, float vx, EnemyType type);
    void update();
    void draw();
    void hit(int damage);
    bool isAlive() const;
    static const int RADIUS = 10;
    bool isOffScreen() const { return offScreen; }
    void shootAtPlayer(const Player &player);
    EnemyType getType() const; // 新的成员函数
    void drawBullets();
    const std::vector<Bullet>& getBullets() const{
        return bullets;
    };  // 获取子弹的方法
    void removeInactiveBullets();
    void clearBullets();

private:
    float vx, vy; // 新增：敌人的速度
    ALLEGRO_COLOR color;
    bool alive;
    bool offScreen; // 新增：敌人是否已离开屏幕
    EnemyType type;
    std::vector<Bullet> bullets;  // 将其作为成员变量
    double lastShootTime;
    
};



#endif // ENEMY_H