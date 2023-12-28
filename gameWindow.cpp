#include "gameWindow.h"
#include "allegro_setup.h"
#include "log.h"  // Include your log class

GameWindow::GameWindow() : doexit(false) {
    Log::Info("GameWindow Created");
    init();
}

GameWindow::~GameWindow() {
    Log::Info("GameWindow Deleted");
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}

void GameWindow::init() {
    Log::Info("Init Started:");

    // Initialize Allegro subsystems
    if (!al_init()) {
        Log::Error("Failed to initialize Allegro");
        return;
    }
    if (!al_install_keyboard()) {
        Log::Error("Failed to initialize keyboard");
        return;
    }
    if (!al_init_image_addon()) {
        Log::Error("Failed to initialize image addon");
        return;
    }
    if (!al_init_primitives_addon()) {
        Log::Error("Failed to initialize primitives addon");
        return;
    }
    if (!al_install_mouse()) {
        Log::Error("Failed to initialize mouse");
        return;
    }
    // Initialize other necessary Allegro add-ons here

    // Create display
    display = al_create_display(800, 600);
    if (!display) {
        Log::Error("Display failed to be created");
        return;
    } else {
        Log::Info("Display created");
    }

    // Create timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        Log::Error("Timer failed to be created");
        return;
    } else {
        Log::Info("Timer created");
    }

    // Create event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        Log::Error("Event queue failed to be created");
        return;
    } else {
        Log::Info("Event queue created");
    }

    // Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    player.init(400, 500);

    Log::Info("GameWindow initialization complete");
}


void GameWindow::run() {
    Log::Info("Game Started!");
    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        switch (ev.type) {
            case ALLEGRO_EVENT_TIMER:
                player.update();
                // Add any other updates here, e.g., for game world, enemies, etc.
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                doexit = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key_state[ev.keyboard.keycode] = true;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key_state[ev.keyboard.keycode] = false;
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    doexit = true;
                }
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (ev.mouse.button & 1) { // Left mouse button
                    Log::Info("Left Mouse Button Clicked");
                    player.shoot(ev.mouse.x, ev.mouse.y);
                }
                break;

            // Add additional cases here for other types of events
        }

        draw();
    }
}


void GameWindow::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    player.draw();
    al_flip_display();
}
