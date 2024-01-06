// menu.cpp
#include "menu.h"
#include "log.h"
Menu::Menu() : gameStart(false), font(nullptr) {}

Menu::~Menu() {
    // Font is managed by GameWindow, so no need to destroy it here
}

void Menu::init(ALLEGRO_FONT* font) {
    this->font = font;
}

void Menu::update() {
    // Update logic for the menu (e.g., button highlights)
}

void Menu::draw() {
    if (!font) {
        //Log::Error("Font not loaded");
        return;
    }
    
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTER, "Press Enter to Start");
}

bool Menu::startGame() const {
    return gameStart;
}

// Implement any additional methods you need for the menu