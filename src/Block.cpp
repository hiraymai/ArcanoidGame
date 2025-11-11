#include "Block.h"
#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>

Block::Block(float x, float y, float width, float height,
    float r, float g, float b, float alpha,
    bool isBomb, int hitPoints, bool indestructible)
    : x(x), y(y), width(width), height(height),
    alpha(alpha), isActive(true),
    isIndestructible(indestructible), hitPoints(hitPoints) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

void Block::draw() {
    if (!isActive) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (isIndestructible) {
        glColor4f(0.4f, 0.4f, 0.5f, alpha);
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();

        glColor4f(0.6f, 0.6f, 0.7f, alpha * 0.5f);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 1; i < 4; i++) {
            glVertex2f(x + i * width / 4, y);
            glVertex2f(x + i * width / 4, y + height);
            glVertex2f(x, y + i * height / 4);
            glVertex2f(x + width, y + i * height / 4);
        }
        glEnd();
    }
    else {
        glBegin(GL_QUADS);
        glColor4f(color[0] * 0.8f, color[1] * 0.8f, color[2] * 0.8f, alpha);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glColor4f(color[0], color[1], color[2], alpha);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();

        glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.3f);
        glVertex2f(x + 2, y + height - 5);
        glVertex2f(x + width - 2, y + height - 5);
        glColor4f(1.0f, 1.0f, 1.0f, alpha * 0.1f);
        glVertex2f(x + width - 2, y + height);
        glVertex2f(x + 2, y + height);
        glEnd();
    }

    glColor4f(0.0f, 0.0f, 0.0f, alpha * 0.5f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_BLEND);

    if (hitPoints > 1 && !isIndestructible) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(x + width * 0.4f, y + height * 0.6f);
        char hpText[3];
        sprintf(hpText, "%d", hitPoints);
        for (char* c = hpText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }
    }
}

bool Block::checkCollision(float ballX, float ballY, float ballRadius) {
    if (!isActive) return false;

    return (ballX + ballRadius > x &&
        ballX - ballRadius < x + width &&
        ballY + ballRadius > y &&
        ballY - ballRadius < y + height);
}

void Block::setActive(bool active) {
    isActive = active;
}

bool Block::getActive() const {
    return isActive;
}

bool Block::getIsIndestructible() const {
    return isIndestructible;
}

void Block::reduceHitPoints() {
    if (!isIndestructible) {
        hitPoints--;
        if (hitPoints <= 0) {
            isActive = false;
        }
    }
}

int Block::getHitPoints() const {
    return hitPoints;
}