#include "global.h"
#include "mode.h"
#include "log.h"
#include <allegro5/allegro_primitives.h>

#include "gameWindow.h"

#include <allegro5/allegro_audio.h>
#include <map>
#include <string>

// Define a map to hold music samples for each mode
std::map<GameMode, ALLEGRO_SAMPLE*> modeMusicSamples;

ALLEGRO_BITMAP* mode1Image;
ALLEGRO_BITMAP* mode2Image;
ALLEGRO_BITMAP* mode3Image;
ALLEGRO_SAMPLE_INSTANCE* modeMusic;
//初始化素材

//初始化參數
// Define constants for mode positions and dimensions
const int MODE1_X = 200;
const int MODE2_X = 400;
const int MODE3_X = 600;
const int MODE_Y = 300;
const int MODE_WIDTH = 200;
const int MODE_HEIGHT = 100;

//初始化選項按鈕
BUTTON MODE1Button;
BUTTON MODE2Button;
BUTTON MODE3Button;
BUTTON backButton2;

//enum GameMode seletedMode = MODE_GROUND;

// Constructor
MODE::MODE() : selectedMode(MODE_GROUND) {
    // Initialize mode images and music to nullptr
    mode1Image = nullptr;
    mode2Image = nullptr;
    mode3Image = nullptr;
    modeMusic = nullptr;
}

// Destructor
MODE::~MODE() {
    // Clean up loaded resources
    if (mode1Image) al_destroy_bitmap(mode1Image);
    if (mode2Image) al_destroy_bitmap(mode2Image);
    if (mode3Image) al_destroy_bitmap(mode3Image);
    //if (modeMusic) al_destroy_sample_instance(modeMusic);
    cleanup();
}

void MODE::show_MODE() {
    setup_buttons();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    load_mode_images(); 
    draw_mode_background(); 
    draw_mode_button(); 
    draw_back_button(); 

    play_mode_music(); 

    al_flip_display();
}

void MODE::init() {
    // Load different music for each mode
    modeMusicSamples[MODE_GROUND] = al_load_sample("1.mp3");
    modeMusicSamples[MODE_AIR] = al_load_sample("2.mp3");
    modeMusicSamples[MODE_BOTH] = al_load_sample("4.mp3");

    // Check for loading errors
    for (auto &pair : modeMusicSamples) {
        if (!pair.second) {
            Log::Error("Failed to load music for mode: " + std::to_string(pair.first));
        }
    }
}

void MODE::cleanup() {
    // Destroy all loaded samples
    for (auto &pair : modeMusicSamples) {
        if (pair.second) {
            al_destroy_sample(pair.second);
        }
    }
}

void MODE::playModeMusic(GameMode mode) {
    ALLEGRO_SAMPLE *sample = modeMusicSamples[mode];
    if (sample) {
        al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, nullptr);
    }
}

void MODE::setup_buttons() {
    // 初始化按鈕
    MODE1Button.x = MODE1_X;
    MODE1Button.y = MODE_Y;
    MODE1Button.width = MODE_WIDTH;
    MODE1Button.height = MODE_HEIGHT;
    strcpy(MODE1Button.text, "MODE 1");

    MODE2Button.x = MODE2_X;
    MODE2Button.y = MODE_Y;
    MODE2Button.width = MODE_WIDTH;
    MODE2Button.height = MODE_HEIGHT;
    strcpy(MODE2Button.text, "MODE 2");

    MODE3Button.x = MODE3_X;
    MODE3Button.y = MODE_Y;
    MODE3Button.width = MODE_WIDTH;
    MODE3Button.height = MODE_HEIGHT;
    strcpy(MODE3Button.text, "MODE 3");

    backButton2.x = 20;
    backButton2.y = 20;
    backButton2.width = 60;
    backButton2.height = 30;
    strcpy(backButton2.text, "Back");
}

void MODE::load_mode_images() {
    // 載入模式圖片
    mode1Image = al_load_bitmap("1.jpg");
    mode2Image = al_load_bitmap("2.jpg");
    mode3Image = al_load_bitmap("3.png");
}

void MODE::draw_mode_background() {
    // 繪製背景圖
    ALLEGRO_BITMAP* MODE_background = al_load_bitmap("4.jpg");
    al_draw_scaled_bitmap(MODE_background, 0, 0, al_get_bitmap_width(MODE_background), al_get_bitmap_height(MODE_background),
        0, 0, al_get_display_width(display), al_get_display_height(display), 0);
}

void MODE::draw_mode_button() {
    // 繪製模式按鈕
    al_draw_scaled_bitmap(mode1Image, 0, 0, al_get_bitmap_width(mode1Image), al_get_bitmap_height(mode1Image),
        MODE1Button.x, MODE_Y, MODE_WIDTH, MODE_HEIGHT, 0);

    al_draw_scaled_bitmap(mode2Image, 0, 0, al_get_bitmap_width(mode2Image), al_get_bitmap_height(mode2Image),
        MODE2Button.x, MODE_Y, MODE_WIDTH, MODE_HEIGHT, 0);

    al_draw_scaled_bitmap(mode3Image, 0, 0, al_get_bitmap_width(mode3Image), al_get_bitmap_height(mode3Image),
        MODE3Button.x, MODE_Y, MODE_WIDTH, MODE_HEIGHT, 0);
}

void MODE::draw_back_button() {
    // 繪製返回按鈕
    al_draw_text(font, al_map_rgb(255, 255, 255), backButton2.x + backButton2.width / 2, backButton2.y + (backButton2.height - al_get_font_line_height(font)) / 2, ALLEGRO_ALIGN_LEFT, backButton2.text);
}

void MODE::play_mode_music() {
    // 播放背景音樂
    ALLEGRO_SAMPLE* backgroundSample = al_load_sample("3.mp3");
    modeMusic = al_create_sample_instance(backgroundSample);
    al_set_sample_instance_playmode(modeMusic, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(modeMusic, al_get_default_mixer());
    al_play_sample_instance(modeMusic);
}

void MODE::choose_MODE() {
    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            int mouseX = event.mouse.x;
            int mouseY = event.mouse.y;

            if (mouseX >= MODE1Button.x && mouseX <= MODE1Button.x + MODE1Button.width &&
                mouseY >= MODE1Button.y && mouseY <= MODE1Button.y + MODE1Button.height) {
                selectedMode = MODE_GROUND;
                running = false;
            } else if (mouseX >= MODE2Button.x && mouseX <= MODE2Button.x + MODE2Button.width &&
                       mouseY >= MODE2Button.y && mouseY <= MODE2Button.y + MODE2Button.height) {
                selectedMode = MODE_AIR;
                running = false;
            } else if (mouseX >= MODE3Button.x && mouseX <= MODE3Button.x + MODE3Button.width &&
                       mouseY >= MODE3Button.y && mouseY <= MODE3Button.y + MODE3Button.height) {
                selectedMode = MODE_BOTH;
                running = false;
            } else if (mouseX >= backButton2.x && mouseX <= backButton2.x + backButton2.width &&
                       mouseY >= backButton2.y && mouseY <= backButton2.y + backButton2.height) {
                // 执行返回菜单的操作
                selectedMode = RETURN;
                running = false;
            }
        }
    }
    // After mode selection
    switch (selectedMode) {
        case MODE_GROUND:
            window.game_enemy();
            window.game_player();
            window.currentState = GAME;
        case MODE_AIR:
            window.game_enemy();
            window.game_player();
            window.currentState = GAME;
        case MODE_BOTH:
            window.game_enemy();
            window.game_player();
            window.currentState = GAME;
            //playModeMusic(selectedMode); // Play music corresponding to the selected mode
            break;
        case RETURN:
            window.currentState = MENU;
    }

}


