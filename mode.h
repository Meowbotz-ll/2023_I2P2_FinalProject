// mode.h
#ifndef MODE_H
#define MODE_H

#include "menu.h"
#include "gameWindow.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h> 
#include <map>

//extern enum GameState currentState;

class GameWindow;

enum GameMode {
    MODE_GROUND,
    MODE_AIR,
    MODE_BOTH,
    RETURN
};

typedef struct Button {
    int x;
    int y;
    int width;
    int height;
    char text[20];
} BUTTON;

class MODE {
public:
    MODE();
    ~MODE();
    void show_MODE();
    void setup_buttons();
    void load_mode_images();
    void draw_mode_background();
    void draw_mode_button();
    void draw_back_button();
    void play_mode_music();
    void choose_MODE();
    void init();
    void cleanup();
    void playModeMusic(GameMode mode);


private:
    GameMode selectedMode;
    Menu menu;
    std::map<GameMode, ALLEGRO_SAMPLE*> modeMusicSamples;
    GameWindow window;
    //GameWindow window;
};

#endif // MODE_H
