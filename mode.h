// mode.h
#ifndef MODE_H
#define MODE_H

#include <allegro5/allegro_font.h>

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_FONT* font;
extern ALLEGRO_BITMAP* background;
extern ALLEGRO_EVENT_QUEUE* event_queue;

extern enum GameState currentState;

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
    void choose_MODE(GameMode &currentMode);
    void init();
    void cleanup();
    void playModeMusic(GameMode mode);


private:
    GameMode selectedMode;
    Menu menu;
    std::map<GameMode, ALLEGRO_SAMPLE*> modeMusicSamples;
};

#endif // MODE_H
