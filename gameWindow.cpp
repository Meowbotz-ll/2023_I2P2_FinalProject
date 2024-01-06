#include "gameWindow.h"
#include "allegro_setup.h"
#include "log.h"  // Include your log class

GameWindow::GameWindow() : currentState(MENU),doexit(false){
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
    // Initialize font add-ons
    al_init_font_addon(); // Initialize the font addon
    if (!al_init_ttf_addon()) { // Initialize the TrueType font addon
        Log::Error("Failed to initialize TTF font addon");
        return;
    }
// Initialize the font for the menu
    Log::Info("Loading font...");
    font = al_load_font("fonts/ARCADE.TTF", 36, 0);
    if (!font) {
        Log::Error("Failed to load font at fonts/ARCADE.TTF");
    }
    menu.init(font);
// Initialize audio subsystem
    if (!al_install_audio()) {
        Log::Error("Failed to initialize audio");
        return;
    }
    if (!al_init_acodec_addon()) {
        Log::Error("Failed to initialize audio codec addon");
        return;
    }
    if (!al_reserve_samples(16)) {  // Reserve enough sample instances for your game
        Log::Error("Failed to reserve audio samples");
        return;
    }
    // Init background Music
    menuMusic = al_load_sample("audio/Electroman-Adventures.ogg");
    gameMusic = al_load_sample("audio/Electroman-Adventures.ogg");

if (!menuMusic || !gameMusic) {
    Log::Error("Failed to load music files");
    return;
}

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
        switch (currentState) {
            case MENU:
            //Log::Info("In Menu State");
                if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        Log::Info("Enter Key Pressed - Switching to Game State"); // Debug message for pressing Enter
                        al_stop_samples(); // Stop any currently playing music
                        currentState = GAME;
                        menu.gameStart = true; // Update the flag in menu
                    }
                }
                menu.update();
                break;

            case GAME:
            //Log::Info("In Menu State");
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
                    Log::Info("Escape Key Pressed - Switching to Menu State"); // Debug message for pressing Escape
                    al_stop_samples(); // Stop any currently playing music
                    currentState = MENU;
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
        break;

        }

        draw();
    }
}


void GameWindow::draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    switch (currentState) {
        case MENU:
            al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            menu.draw();
            break;

        case GAME:
            al_play_sample(menuMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
            player.draw();
            // Draw other game elements
            break;
    }

    al_flip_display();
}