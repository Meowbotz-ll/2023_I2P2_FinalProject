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
    Enemy(float x, float y, int hp, float vx, EnemyType type,const char* gifFile[4]);
    void update();
    void draw();
    void hit(int damage);
    bool isAlive() const;
    void set_Alive(bool status); 
    static const int RADIUS = 10;
    bool isOffScreen() const { return offScreen; }
    void shootAtPlayer(const Player &player);
    EnemyType getType() const; // 新的成员函数
    void drawBullets();
    std::vector<Bullet>& getBullets(){
        return bullets;
    };  // 获取子弹的方法
    void removeInactiveBullets();
    void clearBullets();
    float get_X() const;
    float get_Y() const;
    static const int ENEMY_SIZE = 10;
    

private:
    float vx, vy; // 新增：敌人的速度
    ALLEGRO_COLOR color;
    bool alive;
    bool offScreen; // 新增：敌人是否已离开屏幕
    EnemyType type;
    std::vector<Bullet> bullets;  // 将其作为成员变量
    double lastShootTime;
    ALGIF_ANIMATION *walkingGif[4];
    std::vector<Enemy> enemies;

};



#endif // ENEMY_H