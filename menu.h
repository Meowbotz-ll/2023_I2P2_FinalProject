// menu.h
#ifndef MENU_H
#define MENU_H

#include "allegro_setup.h"

class Menu {
public:
    Menu();
    ~Menu();

    void init(ALLEGRO_FONT* font);
    void update();
    void draw();
    bool startGame() const;
    bool gameStart; // Flag to check if the game should start

private:
    ALLEGRO_FONT* font;
};

#endif // MENU_H