#include "gameWindow.h"
#include "allegro_setup.h"

GameWindow::GameWindow() : doexit(false) {
    init();
}

GameWindow::~GameWindow() {
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

void GameWindow::init() {
    display = al_create_display(800, 600);
    event_queue = al_create_event_queue();
    player.init(400, 500);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void GameWindow::run() {
    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            player.update();
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            key_state[ev.keyboard.keycode] = true;
        } else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            key_state[ev.keyboard.keycode] = false;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                doexit = true;
            }
        }

        draw();
    }
}

void GameWindow::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    player.draw();
    al_flip_display();
}
