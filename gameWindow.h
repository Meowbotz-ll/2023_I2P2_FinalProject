#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "player.h"
#include "global.h"
#include "enemy.h"
#include "log.h"
#include "bullet.h"

#include <vector>

class GameWindow {
public:
    GameWindow();
    ~GameWindow();
    void init();
    void run();
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    Player player;
    std::vector<Enemy> enemies; // Store multiple enemies
    std::vector<Bullet> bullets; // Declare the bullets vector
    bool doexit;
    bool key_state[ALLEGRO_KEY_MAX];
    void update();
    void draw() const;
    // Function to check collision between Bullet and Enemy
    bool checkCollision(const Bullet& bullet, const Enemy& enemy) const;
};

#endif // GAME_WINDOW_H
