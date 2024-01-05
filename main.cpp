#include "gameWindow.h"
#include "allegro_setup.h"
#include <iostream>
using namespace std;

int main() {
    GameWindow window;
    window.init();  // Make sure this calls menu.init()
    window.run();

    return 0;
}
