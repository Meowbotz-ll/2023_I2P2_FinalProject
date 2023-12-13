#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> // For drawing primitives
#include <allegro5/allegro_image.h>      // For image loading

const float FPS = 60;
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int PLAYER_SIZE = 32;
const float GRAVITY = 0.5;
const float JUMP_STRENGTH = -10.0;  // Negative for upward movement

enum MYKEYS {
    KEY_LEFT, KEY_RIGHT, KEY_JUMP
};

struct Player {
    float x, y;
    float dy;
    bool onGround;
    bool facingRight;
};

Player player;

int main() {
    // Initialize Allegro
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();

    // Create display
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);

    // Setup timer
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    al_start_timer(timer);

    // Event queue
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Initialize player
    player.x = SCREEN_W / 2.0 - PLAYER_SIZE / 2.0;
    player.y = SCREEN_H - 100 - PLAYER_SIZE;  // Position player above the platform
    player.dy = 0;
    player.onGround = true;
    player.facingRight = true;

    bool key[3] = { false, false, false };
    bool redraw = true;
    bool doexit = false;

    // Game loop
    while (!doexit) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Gravity effect
            if (!player.onGround) {
                player.dy += GRAVITY;
                player.y += player.dy;
            }

            // Check if player lands on the platform
            if (player.y >= SCREEN_H - 100 - PLAYER_SIZE) {
                player.y = SCREEN_H - 100 - PLAYER_SIZE;
                player.dy = 0;
                player.onGround = true;
            }

            // Player movement
            if (key[KEY_LEFT] && player.x >= 4.0) {
                player.x -= 4.0;
                player.facingRight = false;
            }

            if (key[KEY_RIGHT] && player.x <= SCREEN_W - PLAYER_SIZE - 4.0) {
                player.x += 4.0;
                player.facingRight = true;
            }

            if (key[KEY_JUMP] && player.onGround) {
                player.dy = JUMP_STRENGTH;
                player.onGround = false;
            }

            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_A:
                    key[KEY_LEFT] = true;
                    break;
                case ALLEGRO_KEY_D:
                    key[KEY_RIGHT] = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_JUMP] = true;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_A:
                    key[KEY_LEFT] = false;
                    break;
                case ALLEGRO_KEY_D:
                    key[KEY_RIGHT] = false;
                    break;
                case ALLEGRO_KEY_SPACE:
                    key[KEY_JUMP] = false;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    doexit = true;
                    break;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0,0,0));

            // Draw player
            al_draw_filled_rectangle(player.x, player.y, player.x + PLAYER_SIZE, player.y + PLAYER_SIZE, al_map_rgb(255, 0, 0));

            // Draw platform
            al_draw_filled_rectangle(0, SCREEN_H - 100, SCREEN_W, SCREEN_H - 80, al_map_rgb(255, 255, 255));

            al_flip_display();
        }
    }

    // Cleanup
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}

//haha
