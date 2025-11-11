#ifndef BOMB_H
#define BOMB_H

#include <GL/freeglut.h>

class Bomb {
private:
    float x, y;
    float radius;
    bool isActive;
    float animationTime;

public:
    Bomb(float startX, float startY, float r);
    void draw();
    bool checkCollision(float ballX, float ballY, float ballRadius);
    void setActive(bool active);
    bool getActive() const;
    float getX() const { return x; }
    float getY() const { return y; }
    float getRadius() const { return radius; }
};

#endif