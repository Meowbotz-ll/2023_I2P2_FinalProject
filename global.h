#ifndef GLOBAL_H
#define GLOBAL_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


extern bool key_state[ALLEGRO_KEY_MAX];

extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_FONT* font;
extern ALLEGRO_BITMAP* background;
extern ALLEGRO_EVENT_QUEUE* event_queue;

#endif // GLOBAL_H
