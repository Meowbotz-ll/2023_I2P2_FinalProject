#include "global.h"
#include "mode.h"
#include "log.h"
#inclu <allegro5/allegro_prigit 

#include "gameWindow.h"

ALLEGRO_BITMAP* mode1Image;
ALLEGRO_BITMAP* mode2Image;
ALLEGRO_BITMAP* mode3Image;
ALLEGRO_SAMPLE_INSTANCE* modeMusic;
//初始化素材

//初始化參數
const int MODE1_X = 200;
const int MODE2_X = 400;
const int MODE3_X = 600;
const int MODE_Y = 300;
const int MODE_WIDTH = 800;
const int MODE_HEIGHT = 600;

//初始化選項按鈕
BUTTON MODE1Button;
BUTTON MODE2Button;
BUTTON MODE3Button;
BUTTON backButton2;

void MODE::show_MODE() {
    setup_buttons();
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // 賦值選項圖片
    MODE1Image = al_load_bitmap("mode1.png");
    MODE2Image = al_load_bitmap("mode2.jpg");
    MODE3Image = al_load_bitmap("mode3.png");

    // 繪製背景圖
    ALLEGRO_BITMAP* MODE_background = al_load_bitmap("MODE_background.jpg");
    al_draw_scaled_bitmap(MODE_background, 0, 0, al_get_bitmap_width(MODE_background), al_get_bitmap_height(MODE_background),
        0, 0, al_get_display_width(display), al_get_display_height(display), 0);

    //繪製按鈕文組
    al_draw_text(font, al_map_rgb(135, 206, 250), 1000, 100, ALLEGRO_ALIGN_CENTER, "MODE SELECTION");
    al_draw_text(font, al_map_rgb(255, 165, 0), MODE1Button.x + MODE1Button.width / 2, MODE_Y + MODE_HEIGHT + 10, ALLEGRO_ALIGN_CENTER, "DESERT");
    al_draw_text(font, al_map_rgb(255, 165, 0), MODE2Button.x + MODE2Button.width / 2, MODE_Y + MODE_HEIGHT + 10, ALLEGRO_ALIGN_CENTER, "CITY");
    al_draw_text(font, al_map_rgb(255, 165, 0), MODE3Button.x + MODE3Button.width / 2, MODE_Y + MODE_HEIGHT + 10, ALLEGRO_ALIGN_CENTER, "SEA");

    // 繪製場景選項圖
    al_draw_scaled_bitmap(MODE1Image, 0, 0, al_get_bitmap_width(MODE1Image), al_get_bitmap_height(MODE1Image),
        MODE1Button.x, MODE_Y, MODE_WIDTH, MODE_HEIGHT, 0);

    al_draw_scaled_bitmap(MODE2Image, 0, 0, al_get_bitmap_width(MODE2Image), al_get_bitmap_height(MODE2Image),
        MODE2Button.x, MODE_Y, MODE_WIDTH, MODE_HEIGHT, 0);

    al_draw_scaled_bitmap(MODE3Image, 0, 0, al_get_bitmap_width(MODE3Image), al_get_bitmap_height(MODE3Image),
        MODE3Button.x, MODE_Y, MODE_WIDTH, MODE_HEIGHT, 0);

    // 繪製'back'按鈕
    al_draw_text(font, al_map_rgb(255, 255, 255), backButton2.x + backButton2.width / 2, backButton2.y + (backButton2.height - al_get_font_line_height(font)) / 2, ALLEGRO_ALIGN_LEFT, backButton2.text);

    //背景音效
    ALLEGRO_SAMPLE* backgroundSample = al_load_sample("MODE.mp3");
    MODEMusic = al_create_sample_instance(backgroundSample);
    al_set_sample_instance_playmode(MODEMusic, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(MODEMusic, al_get_default_mixer());
    al_play_sample_instance(MODEMusic);

    // 將前buffer的畫面交換到後臺顯示出來
    al_flip_display();
}


void MODE::setup_buttons() {
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

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

void MODE::choose_MODE(GameMode &curretMode) {
    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            int mouseX = event.mouse.x;
            int mouseY = event.mouse.y;

            if (mouseX >= MODE1Button.x && mouseX <= MODE1Button.x + MODE1Button.width &&
                mouseY >= MODE1Button.y && mouseY <= MODE1Button.y + MODE1Button.height) {
                selectedMODE = MODE1;
                running = false;
            } else if (mouseX >= MODE2Button.x && mouseX <= MODE2Button.x + MODE2Button.width &&
                       mouseY >= MODE2Button.y && mouseY <= MODE2Button.y + MODE2Button.height) {
                selectedMODE = MODE2;
                running = false;
            } else if (mouseX >= MODE3Button.x && mouseX <= MODE3Button.x + MODE3Button.width &&
                       mouseY >= MODE3Button.y && mouseY <= MODE3Button.y + MODE3Button.height) {
                selectedMODE = MODE3;
                running = false;
            } else if (mouseX >= backButton2.x && mouseX <= backButton2.x + backButton2.width &&
                       mouseY >= backButton2.y && mouseY <= backButton2.y + backButton2.height) {
                // 执行返回菜单的操作
                selectedMODE = RETURN;
                running = false;
            }
        }
    }


    if (selectedMODE == MODE1) {
        // 执行游戏进行的操作
        //background = al_load_bitmap("desert_1.jpg");// 加载沙漠位图
        //cactus = al_load_bitmap("cactus.png");// 加载仙人掌位图
        //crow = al_load_bitmap("crow.png");// 加载乌鸦位图
        al_stop_sample_instance(MODEMusic);
        newBackgroundSample = al_load_sample("mixkit-comical-2.mp3"); //加載背景音樂
        newBackgroundMusic = al_create_sample_instance(newBackgroundSample);
        //CountDown(); //倒數
        game_run(); //遊戲開始
    } else if (selectedMODE == MODE2) {
        // 执行游戏进行的操作
        //background = al_load_bitmap("city_1.jpg");// 加载城市位图
        //cactus = al_load_bitmap("car.png");// 加载汽車位图
        //crow = al_load_bitmap("plane.png");// 加载飛機位图
        al_stop_sample_instance(MODEMusic);
        newBackgroundSample = al_load_sample("city.mp3"); //加載背景音樂
        newBackgroundMusic = al_create_sample_instance(newBackgroundSample);
        //CountDown(); //倒數
        game_run(); //遊戲開始
    } else if (selectedMODE == MODE3) {
        // 执行倒计时的操作
        //background = al_load_bitmap("sea_1.jpg");// 加载海洋位图
        //cactus = al_load_bitmap("grass.png");// 加载海草位图
        //crow = al_load_bitmap("fish.png");// 加载魚魚位图
        al_stop_sample_instance(MODEMusic);
        newBackgroundSample = al_load_sample("sea.mp3"); //加載背景音樂
        newBackgroundMusic = al_create_sample_instance(newBackgroundSample);
        //CountDown(); //倒數
        game_run(); //遊戲開始
    }
    //點'return'回菜單
    else if (selectedMODE == RETURN){
        al_stop_sample_instance(MODEMusic);
        returnToMenu();
    }
}


