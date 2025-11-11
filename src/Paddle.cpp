#include "Paddle.h"

Paddle::Paddle(float startX, float startY, float w, float h)
    : x(startX), y(startY), width(w), height(h), speed(10.0f), movingLeft(false), movingRight(false) {
}

void Paddle::update() {
    if (movingLeft && x > 0) {
        x -= speed;
    }
    if (movingRight && x + width < 800) {
        x += speed;
    }
}

void Paddle::draw() {
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Paddle::moveLeft() {
    movingLeft = true;
    movingRight = false;
}

void Paddle::moveRight() {
    movingRight = true;
    movingLeft = false;
}

void Paddle::stop() {
    movingLeft = false;
    movingRight = false;
}