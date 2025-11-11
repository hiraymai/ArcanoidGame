#include "Ball.h"
#include <cmath>

#define M_PI 3.14159265358979323846

Ball::Ball(float startX, float startY, float startDX, float startDY, float r)
    : x(startX), y(startY), dx(startDX), dy(startDY), radius(r) {
}

void Ball::update() {
    x += dx;
    y += dy;
}

void Ball::reverseX() {
    dx = -dx;
}

void Ball::reverseY() {
    dy = -dy;
}

void Ball::draw() {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x, y);

    for (int i = 0; i <= 32; i++) {
        float angle = 2.0f * M_PI * float(i) / 32.0f;
        float dx_component = radius * cos(angle);
        float dy_component = radius * sin(angle);

        if (i < 8)
            glColor3f(0.8f, 0.9f, 1.0f);
        else if (i < 16)
            glColor3f(0.6f, 0.8f, 1.0f);
        else if (i < 24)
            glColor3f(0.4f, 0.7f, 1.0f);
        else
            glColor3f(0.2f, 0.6f, 1.0f);

        glVertex2f(x + dx_component, y + dy_component);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(x - radius * 0.3f, y + radius * 0.3f);
    glEnd();
}