#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "player.h"
#include "global.h"
#include "log.h"

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
    bool doexit;
    void update();
    void draw();
};

#endif // GAME_WINDOW_H
