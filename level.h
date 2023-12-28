#ifndef LEVEL_H
#define LEVEL_H

class Level {
public:
    Level();
    void loadLevel(const char* filename);
    void update();
    void render();
};

#endif // LEVEL_H
