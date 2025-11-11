#ifndef BLOCK_H
#define BLOCK_H

#include <GL/freeglut.h>

class Block {
private:
    float x, y, width, height;
    float color[3];
    float alpha;
    bool isActive;
    bool isIndestructible;
    int hitPoints;

public:
    Block(float x, float y, float width, float height, 
          float r, float g, float b, float alpha = 1.0f, 
          bool isBomb = false, int hitPoints = 1, bool isIndestructible = false);
    
    void draw();
    bool checkCollision(float ballX, float ballY, float ballRadius);
    void setActive(bool active);
    bool getActive() const;
    bool getIsIndestructible() const;
    void reduceHitPoints();
    int getHitPoints() const;
    
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

#endif