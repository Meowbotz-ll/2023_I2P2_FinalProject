#include "gameWindow.h"
#include "allegro_setup.h"
#include <iostream>

GameWindow::GameWindow() : doexit(false) {
    std::cout << "GameWindow Created" << std::endl;
    init();
}

GameWindow::~GameWindow() {
    std::cout << "GameWindow Deleted" << std::endl;
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

void GameWindow::init() {
    log("Init Started:\n");
    display = al_create_display(800, 600);
    if (display) {
        log("Display created\n");
        return;
    }else{
        log("Display failed created\n");
    }
    // Create and start a timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (timer) {
        log("Timer created\n");
        return;
    }else{
        log("Timer created failed\n");
    }
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    
    event_queue = al_create_event_queue();
    if (event_queue) {
        log("event queue created\n");
        return;
    }else{
        log("event queue created failed\n");
    }
    player.init(400, 500);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    log("Game Init\n");
}

void GameWindow::run() {
    log("Game Started!\n");
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
