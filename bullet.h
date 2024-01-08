// File: .\bullet.h
#pragma once
#include <allegro5/allegro.h>
#include "allegro_setup.h"
#include <vector>
#include <math.h>
#include <algorithm>
class Bullet {
public:
    float x, y;
    static const int DEFAULT_DAMAGE=1;
    static const int DEFAULT_SIZE = 10;
    bool isAlive() const{
        return alive;
    }
    void setDamage(int newDamage) { damage = newDamage; }
    void setSize(int newSize) { size = newSize; }
    int getDamage() const { return damage; }
    int getSize() const { return size; }
    Bullet(float x, float y, float dx, float dy); // 现有构造函数
    void update();
    void draw();
    void setAlive(bool status); // 声明 setAlive 方法
    void setHit(bool status) { hit = status; }
    bool isHit() const { return hit; }
    void setEnemyBullet(bool isEnemy) {
        isEnemyBullet = isEnemy;
    }

    bool getIsEnemyBullet() const {
        return isEnemyBullet;
    }
    bool isOffScreen() const { return offScreen; }
    bool is_Alive() const;
    void setExploding(bool value){
        exploding = value;
    }
    void setBouncing(bool value){
        isBouncing =value;
    }
    bool isExploding() const {
        return exploding;
    }
    void setBounceCount(int value)
    {
        bounceCount=value;
    }
    bool isBouncing=false;
    float dx, dy;
    int bounceCount=0; // To limit the number of bounces

private:
    bool alive;
    int damage;  // 存储每个子弹的伤害值
    bool hit; // 新增成员变量，用于跟踪子弹是否已经击中敌人
    int size;
    bool isEnemyBullet;
    bool offScreen;
    bool exploding=false;
};