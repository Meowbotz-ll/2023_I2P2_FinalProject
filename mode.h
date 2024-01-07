// mode.h
#ifndef MODE_H
#define MODE_H

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_FONT* font;
extern ALLEGRO_BITMAP* background;
extern ALLEGRO_EVENT_QUEUE* event_queue;

extern enum GameState currentState;

enum GameMode {
    MODE_GROUND,
    MODE_AIR,
    MODE_BOTH
};

typedef struct Button {
    int x;
    int y;
    int width;
    int height;
    char text[20];
} BUTTON;

// Function declarations (if needed)
void show_MODE();
void setup_buttons();
void choose_MODE();

#endif // MODE_H
