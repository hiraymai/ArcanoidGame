#include "Bomb.h"
#include <cmath>

Bomb::Bomb(float startX, float startY, float r)
    : x(startX), y(startY), radius(r), isActive(true), animationTime(0.0f) {
}

void Bomb::draw() {
    if (!isActive) return;

    animationTime += 0.05f;
    float pulse = 0.8f + 0.2f * sin(animationTime * 8.0f);
    float flash = (sin(animationTime * 10.0f) + 1.0f) / 2.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.1f, 0.1f, 0.1f, 0.9f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 32; i++) {
        float angle = 2.0f * 3.14159f * float(i) / 32.0f;
        glVertex2f(x + cos(angle) * radius * pulse, y + sin(angle) * radius * pulse);
    }
    glEnd();

    glColor4f(0.3f, 0.3f, 0.3f, 0.9f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 32; i++) {
        float angle = 2.0f * 3.14159f * float(i) / 32.0f;
        glVertex2f(x + cos(angle) * radius * pulse, y + sin(angle) * radius * pulse);
    }
    glEnd();

    glColor4f(1.0f, 0.5f + flash * 0.5f, 0.0f, 1.0f);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y + radius);
    glVertex2f(x, y + radius * 1.8f);
    glEnd();

    glColor4f(1.0f, 1.0f, 0.0f, flash);
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    glVertex2f(x, y + radius * 1.8f);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(x - 3.0f, y - 2.0f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '!');

    glDisable(GL_BLEND);
}

bool Bomb::checkCollision(float ballX, float ballY, float ballRadius) {
    if (!isActive) return false;

    float dx = ballX - x;
    float dy = ballY - y;
    float distance = sqrt(dx * dx + dy * dy);

    return distance < (ballRadius + radius);
}

void Bomb::setActive(bool active) {
    isActive = active;
}

bool Bomb::getActive() const {
    return isActive;
}