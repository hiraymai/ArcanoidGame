#ifndef BALL_H
#define BALL_H

#include <GL/freeglut.h>

class Ball {
private:
    float x, y;
    float dx, dy;
    float radius;

public:
    Ball(float startX, float startY, float startDX, float startDY, float r);
    void update();
    void reverseX();
    void reverseY();
    void draw();
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getDX() const { return dx; }
    float getDY() const { return dy; }
    float getRadius() const { return radius; }
    void setDX(float newDX) { dx = newDX; }
    void setDY(float newDY) { dy = newDY; }
};

#endif