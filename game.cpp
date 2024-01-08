#include "game.h"
#include "gameWindow.h"
#include "log.h"  // Include your log class
#include "algorithm"
#include "bullet.h"
#include <iterator>

using namespace std;

//extern std::vector<std::string> enemyGif;  // 假設 enemyGif 是字符串的向量
//extern ALLEGRO_SAMPLE* gameMusic;  // 假設 gameMusic 是 Allegro 的音樂樣本


void Game::mode1()
{
    enemyGif[0] = "enemy_gif/flying.gif";
    enemyGif[1] = "enemy_gif/walking_r.gif";
    enemyGif[2] = "enemy_gif/walking_l.gif";
    enemyGif[3] = "enemy_gif/walking_r.gif";
    //const char** enemyGif = window.getEnemyGif();
    static double last_spawn_time = 0;
    static double enemySpawnInterval = 2.0;     
    if (al_get_time() - last_spawn_time > enemySpawnInterval) {
        last_spawn_time = al_get_time();
        float spawnY;
        float velocityX;
        EnemyType type;

        // 生成地上的敌人
        spawnY = 460; // 与玩家相同的高度
        velocityX = 1.0; // 天上敌人速度的两倍
        type = EnemyType::GROUND;
    

        // 随机决定敌人从左侧或右侧出现
        float spawnX;
        if (rand() % 2 == 0) {
            spawnX = -1; // 从屏幕左侧出现
        } else {
            spawnX = 801; // 从屏幕右侧出现
            velocityX = -velocityX; // 改变移动方向
        }

        // 添加敌人到列表
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type,enemyGif));//air
        enemySpawnInterval = std::max(1.0, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    ground_enemy();
    //game_player();
}

void Game::mode2()
{
    //const char** enemyGif = window.getEnemyGif();
    static double last_spawn_time = 0;
    static double enemySpawnInterval = 5.0;     
    if (al_get_time() - last_spawn_time > enemySpawnInterval) {
        last_spawn_time = al_get_time();
        float spawnY;
        float velocityX;
        EnemyType type;

        // 生成天上的敌人
        spawnY = static_cast<float>(rand() % 300); // 屏幕上半部
        velocityX = 1.0; // 初始速度
        type = EnemyType::AIR;

        // 随机决定敌人从左侧或右侧出现
        float spawnX;
        if (rand() % 2 == 0) {
            spawnX = -1; // 从屏幕左侧出现
        } else {
            spawnX = 801; // 从屏幕右侧出现
            velocityX = -velocityX; // 改变移动方向
        }

        // 添加敌人到列表
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type,enemyGif));//air
        enemySpawnInterval = std::max(1.0, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    sky_enemy();
}

void Game::sky_enemy()
{
    for (auto& enemy : enemies) {
        enemy.update(); // 更新敌人位置和状态

        // 天空中的敌人继续射击
        if (enemy.getType() == EnemyType::AIR && enemy.isAlive()) {
            enemy.shootAtPlayer(player);
        }
    }

    // 遍历每个敌人的子弹
    for (auto& enemy : enemies) {
        for (auto& bullet : enemy.getBullets()) {
            bullet.update(); // 更新子弹位置
            // 检查子弹是否击中玩家
            if (bullet.is_Alive() && checkCollision(bullet, player)) {
                player.getHit(1);
                bullet.setAlive(false); // 子弹击中玩家后消失
            }
        }
    }

    // 删除所有非活动的子弹
    for (auto& enemy : enemies) {
        enemy.removeInactiveBullets();
    }
}

void Game::ground_enemy()
{
    for (auto& enemy : enemies) {
        enemy.update();
        if (enemy.isAlive() && checkCollision(player, enemy)) {
            player.getHit(1);
            enemy.set_Alive(false);
        }
    }
}

void Game::mode3()
{
    //const char** enemyGif = window.getEnemyGif();
    static double last_spawn_time = 0;
    static double enemySpawnInterval = 2.0;     
    if (al_get_time() - last_spawn_time > enemySpawnInterval) {
        last_spawn_time = al_get_time();
        float spawnY;
        float velocityX;
        EnemyType type;

        // 随机决定生成哪种类型的敌人
        if (rand() % 2 == 0) {
            // 生成天上的敌人
            spawnY = static_cast<float>(rand() % 300); // 屏幕上半部
            velocityX = 1.0; // 初始速度
            type = EnemyType::AIR;
        } else {
            // 生成地上的敌人
            spawnY = 460; // 与玩家相同的高度
            velocityX = 1.0; // 天上敌人速度的两倍
            type = EnemyType::GROUND;
        }

        // 随机决定敌人从左侧或右侧出现
        float spawnX;
        if (rand() % 2 == 0) {
            spawnX = -1; // 从屏幕左侧出现
        } else {
            spawnX = 801; // 从屏幕右侧出现
            velocityX = -velocityX; // 改变移动方向
        }

        // 添加敌人到列表
        enemies.push_back(Enemy(spawnX, spawnY, velocityX, type,enemyGif));//air
        enemySpawnInterval = std::max(1.0, enemySpawnInterval - 0.1); // 逐渐减少间隔时间
    }
    ground_enemy();
    sky_enemy();
}


void Game::game_player()
{
    player.update();
    // Check if the player's health is 0 or less
    /*if (player.getHp() <= 0) {
        //Log::Info("Game Over!");
        window.setCurrentState(GAME_OVER);
    }*/
    // 检测子弹与敌人的碰撞
    for (auto& bullet : player.getBullets()) {
        if (bullet.isHit()) continue; // 如果子弹已经击中了敌人，跳过这个子弹
        for (auto& enemy : enemies) {
            if (checkCollision(bullet, enemy) && enemy.isAlive()) {
                enemy.hit(bullet.getDamage());
                bullet.setHit(true); // 标记子弹已经击中敌人
                bullet.setAlive(false); // 设置子弹为不活跃
                if(enemy.hp<=0){
                    score++;
                    enemy.set_Alive(false);
                }
                    break; // 退出内部循环
            }
        }
    }
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) {
            for (auto& bullet : enemy.getBullets()) {
                bullet.update();
                // 也许可以在这里检查子弹是否超出屏幕范围，并将其标记为不活跃
                if (checkCollision(bullet, player)) {
                    player.getHit(bullet.getDamage()); // 玩家受到伤害
                    bullet.setAlive(false); // 子弹消失
                }
            }
        }
    }

}

bool Game::checkCollision(const Bullet &bullet, const Enemy &enemy) {
    // Simple circle-rectangle collision detection
    float distX = std::abs(bullet.x - enemy.x);
    float distY = std::abs(bullet.y - enemy.y);

    if (distX > (bullet.getSize()/ 2 + Enemy::RADIUS)) { return false; }
    if (distY > (bullet.getSize()/ 2 + Enemy::RADIUS)) { return false; }

    if (distX <= (bullet.getSize()/ 2)) { return true; } 
    if (distY <= (bullet.getSize()/ 2)) { return true; }

    // Corner collision
    float dx = distX - bullet.getSize()/ 2;
    float dy = distY - bullet.getSize()/ 2;
    return (dx * dx + dy * dy <= (Enemy::RADIUS * Enemy::RADIUS));
}

bool Game::checkCollision(const Bullet& bullet, const Player& player) {
    // Simple AABB (Axis-Aligned Bounding Box) collision check
    float playerLeft = player.getX();
    float playerRight = player.getX() + player.PLAYER_SIZE;
    float playerTop = player.getY();
    float playerBottom = player.getY() + player.PLAYER_SIZE;

    float bulletLeft = bullet.x;
    float bulletRight = bullet.x + bullet.getSize();
    float bulletTop = bullet.y;
    float bulletBottom = bullet.y + bullet.getSize();

    return (playerLeft < bulletRight && playerRight > bulletLeft &&
            playerTop < bulletBottom && playerBottom > bulletTop);
}

bool Game::checkCollision(const Player& player, const Enemy& enemy) {
    // 獲取玩家和敵人的位置和大小
    float playerLeft = player.getX();
    float playerRight = player.getX() + player.PLAYER_SIZE;
    float playerTop = player.getY();
    float playerBottom = player.getY() + player.PLAYER_SIZE;

    float enemyLeft = enemy.get_X();
    float enemyRight = enemy.get_X() + enemy.ENEMY_SIZE;  // 假設敵人有一個SIZE常量
    float enemyTop = enemy.get_Y();
    float enemyBottom = enemy.get_Y() + enemy.ENEMY_SIZE;

    // 檢查矩形是否重疊
    return !(playerLeft > enemyRight || playerRight < enemyLeft ||
             playerTop > enemyBottom || playerBottom < enemyTop);
}

/*void Game::draw(){
    //ALLEGRO_SAMPLE* gameMusic = window.getGameMusic();
    gameMusic = al_load_sample("audio/Electroman-Adventures.mp3");
    std::ostringstream timeStream;
    std::string timeText;
    std::string scoreText;
    const int maxHealthWidth = 750; // Maximum width of the health bar
    int currentHealthWidth;

    currentTime = static_cast<int>(al_get_time() - startTime);
    timeStream << "Time: " << currentTime;
    timeText = timeStream.str();
    scoreText = "Score: " + std::to_string(score);

    // Draw Health Bar
    currentHealthWidth = (player.getHp() * maxHealthWidth) / 100;
    al_draw_filled_rectangle(10, 600 - 30, 10 + currentHealthWidth, 600 - 10, al_map_rgb(255, 0, 0));

     ui_font = al_load_font("fonts/ARCADE.TTF", 24, 0); // Load a font
    // Draw text
    al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 10, 0, timeText.c_str());
    al_draw_text(ui_font, al_map_rgb(255, 255, 255), 10, 40, 0, scoreText.c_str());
            
    al_play_sample(gameMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    player.draw();
    for (auto& enemy : enemies) {
        // 这里只绘制活着的敌人
        if (enemy.isAlive()) {
            enemy.draw();
        }
    // 但是无论敌人是否活着，都绘制其子弹
        for (auto& bullet : enemy.getBullets()) {
            if (checkCollision(bullet, player)) continue;
            else bullet.draw();
                        
        }
    }
}*/
