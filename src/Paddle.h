#ifndef PADDLE_H
#define PADDLE_H

#include <GL/freeglut.h>

class Paddle {
private:
    float x, y;
    float width, height;
    float speed;
    bool movingLeft, movingRight;

public:
    Paddle(float startX, float startY, float w, float h);
    void update();
    void draw();
    void moveLeft();
    void moveRight();
    void stop();
    void setX(float newX) { x = newX; }
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

#endif