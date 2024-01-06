#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "player.h"
#include "global.h"
#include "log.h"
#include "menu.h"

enum GameState {
    MENU,
    GAME
};

class GameWindow {
public:
    GameWindow();
    ~GameWindow();
    void init();
    void run();
    void draw();
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    Player player;
    GameState currentState;
    ALLEGRO_FONT* font;
    ALLEGRO_SAMPLE *menuMusic;
    ALLEGRO_SAMPLE *gameMusic;
    Menu menu;
    bool doexit;
    void update();
};

#endif // GAME_WINDOW_H
