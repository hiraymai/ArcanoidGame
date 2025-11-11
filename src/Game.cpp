#include "Game.h"
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cstdio>

#define M_PI 3.14159265358979323846

Game::Game()
    : ball(400, 100, 0.0f, 0.0f, 10.0f),
    paddle(350, 50, 100, 20),
    score(0), lives(3), currentLevel(1),
    gameOver(false), gameWon(false), levelComplete(false),
    gameOverAlpha(0.0f), gameWonAlpha(0.0f), levelCompleteAlpha(0.0f),
    fadeDirection(1.0f), ballLaunched(false), ballSpeed(5.0f), timeCounter(0.0f) {
    srand(time(0));
    loadLevel(currentLevel);

    for (int i = 0; i < 50; i++) {
        snowflakes.push_back(std::make_pair(
            rand() % 800,
            rand() % 600
        ));
        snowflakeSpeeds.push_back(0.2f + (rand() % 100) / 400.0f);
    }

    for (int i = 0; i < 25; i++) {
        garlands.push_back(std::make_pair(
            i * 32.0f,
            80.0f
        ));
    }
}

void Game::init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::draw() {
    timeCounter += 0.01f;

    glBegin(GL_QUADS);
    switch (currentLevel) {
    case 1: glColor4f(0.0f, 0.1f, 0.3f, 1.0f); break;
    case 2: glColor4f(0.1f, 0.0f, 0.2f, 1.0f); break;
    case 3: glColor4f(0.2f, 0.0f, 0.1f, 1.0f); break;
    case 4: glColor4f(0.0f, 0.2f, 0.1f, 1.0f); break;
    case 5: glColor4f(0.3f, 0.1f, 0.0f, 1.0f); break;
    case 6: glColor4f(0.1f, 0.0f, 0.3f, 1.0f); break;
    default: glColor4f(0.0f, 0.1f, 0.3f, 1.0f);
    }
    glVertex2f(0, 0);
    glVertex2f(800, 0);
    switch (currentLevel) {
    case 1: glColor4f(0.0f, 0.05f, 0.15f, 1.0f); break;
    case 2: glColor4f(0.05f, 0.0f, 0.1f, 1.0f); break;
    case 3: glColor4f(0.1f, 0.0f, 0.05f, 1.0f); break;
    case 4: glColor4f(0.0f, 0.1f, 0.05f, 1.0f); break;
    case 5: glColor4f(0.15f, 0.05f, 0.0f, 1.0f); break;
    case 6: glColor4f(0.05f, 0.0f, 0.15f, 1.0f); break;
    default: glColor4f(0.0f, 0.05f, 0.15f, 1.0f);
    }
    glVertex2f(800, 600);
    glVertex2f(0, 600);
    glEnd();

    drawGarlands();
    drawSnowflakes();

    for (auto& block : blocks) {
        block.draw();
    }

    for (auto& bomb : bombs) {
        bomb.draw();
    }

    paddle.draw();
    ball.draw();

    drawTransparentRect(5, 555, 790, 40, 0.0f, 0.0f, 0.0f, 0.7f);

    float glow = 0.7f + 0.3f * sin(timeCounter * 8.0f);
    glColor3f(1.0f, glow, glow);
    glRasterPos2f(20, 575);
    std::string scoreText = "SCORE: " + std::to_string(score) + "   LIVES: " + std::to_string(lives) + "   LEVEL: " + std::to_string(currentLevel);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    for (int i = 0; i < 3; i++) {
        float alpha = (i < lives) ? 1.0f : 0.3f;
        float pulse = 0.8f + 0.2f * sin(timeCounter * 6.0f + i);
        drawHeart(720 + i * 25, 570, 8.0f * pulse, 1.0f, 0.3f, 0.3f, alpha);
    }

    if (!ballLaunched) {
        float pulseAlpha = 0.5f + 0.5f * sin(timeCounter * 10.0f);
        drawTransparentRect(300, 200, 200, 60, 0.0f, 0.3f, 0.6f, 0.7f * pulseAlpha);
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(320, 230);
        std::string launchText = "PRESS SPACE";
        for (char c : launchText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        glRasterPos2f(310, 210);
        std::string launchText2 = "TO LAUNCH BALL";
        for (char c : launchText2) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    if (levelComplete) {
        levelCompleteAlpha = std::fmin(levelCompleteAlpha + 0.02f, 1.0f);

        drawTransparentRect(150, 150, 500, 300, 0.0f, 0.2f, 0.4f, levelCompleteAlpha * 0.9f);

        float titleGlow = 0.5f + 0.5f * sin(timeCounter * 6.0f);
        glColor4f(0.2f, 1.0f, 1.0f, levelCompleteAlpha * titleGlow);
        glRasterPos2f(280, 350);
        std::string levelText = "LEVEL " + std::to_string(currentLevel) + " COMPLETED!";
        for (char c : levelText) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        glColor4f(1.0f, 1.0f, 1.0f, levelCompleteAlpha);
        glRasterPos2f(250, 300);
        std::string scoreText = "SCORE: " + std::to_string(score);
        for (char c : scoreText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(250, 270);
        std::string livesText = "LIVES REMAINING: " + std::to_string(lives);
        for (char c : livesText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(270, 220);
        std::string continueText = "PRESS SPACE TO CONTINUE";
        for (char c : continueText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(290, 190);
        std::string restartText = "PRESS 'R' TO RESTART LEVEL";
        for (char c : restartText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }

    if (gameOver) {
        gameOverAlpha += 0.02f * fadeDirection;
        if (gameOverAlpha >= 1.0f || gameOverAlpha <= 0.0f) {
            fadeDirection *= -1.0f;
        }

        drawTransparentRect(150, 150, 500, 300, 0.3f, 0.0f, 0.0f, gameOverAlpha * 0.9f);

        glColor4f(1.0f, 0.2f, 0.2f, gameOverAlpha);
        glRasterPos2f(280, 320);
        std::string gameOverText = "GAME OVER";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        glColor4f(1.0f, 1.0f, 1.0f, gameOverAlpha);
        glRasterPos2f(250, 260);
        std::string finalScore = "FINAL SCORE: " + std::to_string(score);
        for (char c : finalScore) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(270, 220);
        std::string restart = "PRESS 'R' TO RESTART";
        for (char c : restart) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    if (gameWon) {
        gameWonAlpha = std::fmin(gameWonAlpha + 0.01f, 1.0f);

        drawTransparentRect(150, 150, 500, 300, 0.0f, 0.3f, 0.0f, gameWonAlpha * 0.9f);

        float winGlow = 0.5f + 0.5f * sin(timeCounter * 5.0f);
        glColor4f(0.2f, 1.0f, 0.2f, gameWonAlpha * winGlow);
        glRasterPos2f(300, 320);
        std::string winText = "YOU WIN!";
        for (char c : winText) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        glColor4f(1.0f, 1.0f, 1.0f, gameWonAlpha);
        glRasterPos2f(250, 260);
        std::string finalScore = "FINAL SCORE: " + std::to_string(score);
        for (char c : finalScore) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }

        glRasterPos2f(270, 220);
        std::string congrats = "CONGRATULATIONS!";
        for (char c : congrats) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }
}

void Game::update() {
    if (gameOver || gameWon || levelComplete) return;

    timeCounter += 0.016f;
    updateSnowflakes();

    if (ballLaunched) {
        ball.update();
    }
    else {
        ball.setX(paddle.getX() + paddle.getWidth() / 2);
    }

    paddle.update();

    if (ballLaunched) {
        if (ball.getX() - ball.getRadius() < 0 || ball.getX() + ball.getRadius() > 800) {
            ball.reverseX();
        }
        if (ball.getY() + ball.getRadius() > 600) {
            ball.reverseY();
        }

        if (ball.getY() - ball.getRadius() < paddle.getY() + paddle.getHeight() &&
            ball.getY() - ball.getRadius() > paddle.getY() &&
            ball.getX() > paddle.getX() && ball.getX() < paddle.getX() + paddle.getWidth()) {
            ball.reverseY();
            float hitPos = (ball.getX() - paddle.getX()) / paddle.getWidth() - 0.5f;
            ball.setDX(ball.getDX() + hitPos * 2.0f);
        }
    }

    for (auto& block : blocks) {
        if (block.getActive() && block.checkCollision(ball.getX(), ball.getY(), ball.getRadius())) {
            float ballLeft = ball.getX() - ball.getRadius();
            float ballRight = ball.getX() + ball.getRadius();
            float ballTop = ball.getY() + ball.getRadius();
            float ballBottom = ball.getY() - ball.getRadius();

            float blockLeft = block.getX();
            float blockRight = block.getX() + block.getWidth();
            float blockTop = block.getY() + block.getHeight();
            float blockBottom = block.getY();

            bool fromLeft = ballRight > blockLeft && ballLeft < blockLeft;
            bool fromRight = ballLeft < blockRight && ballRight > blockRight;
            bool fromTop = ballBottom < blockTop && ballTop > blockTop;
            bool fromBottom = ballTop > blockBottom && ballBottom < blockBottom;

            if (fromLeft || fromRight) {
                ball.reverseX();
            }
            if (fromTop || fromBottom) {
                ball.reverseY();
            }

            if (!block.getIsIndestructible()) {
                block.reduceHitPoints();
                if (!block.getActive()) {
                    score += 10;
                    blocksRemaining--;
                }
            }
            break;
        }
    }

    for (auto& bomb : bombs) {
        if (bomb.getActive() && bomb.checkCollision(ball.getX(), ball.getY(), ball.getRadius())) {
            explodeBomb(bomb.getX(), bomb.getY());
            bomb.setActive(false);
            lives--;
            score -= 20;
            if (score < 0) score = 0;
            break;
        }
    }

    checkLevelCompletion();

    if (ball.getY() - ball.getRadius() < 0) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }
        else {
            resetBall();
        }
    }
}

void Game::handleInput(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
    case 'A':
        paddle.moveLeft();
        break;
    case 'd':
    case 'D':
        paddle.moveRight();
        break;
    case ' ':
        if (!ballLaunched && !levelComplete && !gameOver && !gameWon) {
            launchBall();
        }
        else if (levelComplete) {
            levelComplete = false;
            levelCompleteAlpha = 0.0f;
            currentLevel++;
            if (currentLevel > 6) {
                gameWon = true;
            }
            else {
                loadLevel(currentLevel);
            }
        }
        break;
    case 'r':
    case 'R':
        if (gameOver || gameWon || levelComplete) {
            gameOver = false;
            gameWon = false;
            levelComplete = false;
            score = 0;
            lives = 3;
            currentLevel = 1;
            loadLevel(currentLevel);
        }
        break;
    case 27:
        exit(0);
        break;
    }
}

void Game::handleSpecialInput(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        paddle.moveLeft();
        break;
    case GLUT_KEY_RIGHT:
        paddle.moveRight();
        break;
    }
}

void Game::handleMouseMove(int x, int y) {
    float gameX = (x / 800.0f) * 800.0f;
    paddle.setX(gameX - paddle.getWidth() / 2);

    if (paddle.getX() < 0) paddle.setX(0);
    if (paddle.getX() + paddle.getWidth() > 800) paddle.setX(800 - paddle.getWidth());
}

void Game::resetBall() {
    ball = Ball(paddle.getX() + paddle.getWidth() / 2, 100, 0.0f, 0.0f, 10.0f);
    ballLaunched = false;
}

void Game::launchBall() {
    ballLaunched = true;
    ballSpeed = 5.0f + currentLevel * 0.5f;
    ball.setDY(ballSpeed);
    float randomDirection = ((rand() % 100) / 50.0f - 1.0f) * 1.5f;
    ball.setDX(randomDirection);
}

void Game::loadLevel(int level) {
    blocks.clear();
    bombs.clear();
    blocksRemaining = 0;
    ballLaunched = false;
    ballSpeed = 5.0f + level * 0.5f;

    float blockWidth = 70.0f;
    float blockHeight = 30.0f;
    float startX = 50.0f;
    float startY = 450.0f;

    switch (level) {
    case 1:
        for (int i = 1; i < 7; i++) {
            float hue = (float)i / 6.0f;
            blocks.emplace_back(
                startX + i * (blockWidth + 10),
                startY,
                blockWidth, blockHeight,
                0.2f + hue * 0.8f, 0.4f, 1.0f,
                0.9f, false, 1, false
            );
            blocksRemaining++;
        }
        break;

    case 2:
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 2; j++) {
                if (!(i == 3 && j == 1) && !(i == 4 && j == 1)) {
                    float hue = (float)i / 8.0f;
                    blocks.emplace_back(
                        startX + i * (blockWidth + 10),
                        startY - j * (blockHeight + 10),
                        blockWidth, blockHeight,
                        0.8f, 0.2f + hue * 0.6f, 0.2f + hue * 0.6f,
                        0.9f, false, 1, false
                    );
                    blocksRemaining++;
                }
            }
        }
        bombs.emplace_back(400, 350, 12.0f);
        break;

    case 3:
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 3; j++) {
                blocks.emplace_back(
                    startX + i * (blockWidth + 10),
                    startY - j * (blockHeight + 10),
                    blockWidth, blockHeight,
                    0.3f, 0.8f, 0.3f,
                    0.9f, false, 1, false
                );
                blocksRemaining++;
            }
        }
        for (int i = 2; i < 6; i++) {
            blocks.emplace_back(
                startX + i * (blockWidth + 10),
                startY + blockHeight + 10,
                blockWidth, blockHeight,
                0.5f, 0.5f, 0.5f,
                0.8f, false, 1, true
            );
        }
        bombs.emplace_back(200, 300, 12.0f);
        bombs.emplace_back(600, 300, 12.0f);
        break;

    case 4:
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 6 - row; col++) {
                blocks.emplace_back(
                    startX + (col + row * 0.5f) * (blockWidth + 10),
                    startY - row * (blockHeight + 10),
                    blockWidth, blockHeight,
                    0.8f, 0.3f, 0.8f,
                    0.9f, false, 1, false
                );
                blocksRemaining++;
            }
        }
        bombs.emplace_back(400, 250, 12.0f);
        break;

    case 5:
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 4; j++) {
                if ((i + j) % 2 == 0) {
                    blocks.emplace_back(
                        startX + i * (blockWidth - 5),
                        startY - j * (blockHeight + 5),
                        blockWidth - 10, blockHeight,
                        1.0f, 0.7f, 0.3f,
                        0.9f, false, 1, false
                    );
                    blocksRemaining++;
                }
            }
        }
        for (int i = 0; i < 5; i++) {
            bombs.emplace_back(150 + i * 120, 320, 12.0f);
        }
        break;

    case 6:
        for (int ring = 0; ring < 3; ring++) {
            for (int i = 0; i < 6 + ring * 2; i++) {
                float angle = (float)i / (6 + ring * 2) * 2 * M_PI;
                float radius = 180.0f - ring * 40.0f;

                blocks.emplace_back(
                    400.0f + cos(angle) * radius - blockWidth / 2,
                    300.0f + sin(angle) * radius - blockHeight / 2,
                    blockWidth, blockHeight,
                    0.9f - ring * 0.2f, 0.3f, 0.3f + ring * 0.2f,
                    0.9f, false, 1, false
                );
                blocksRemaining++;
            }
        }
        bombs.emplace_back(400, 300, 12.0f);
        break;
    }

    resetBall();
}

void Game::explodeBomb(float x, float y) {
    for (int i = 0; i < 60; i++) {
        drawParticles(x, y, 1.0f, 0.1f, 0.1f);
    }

    for (int i = 0; i < 30; i++) {
        drawParticles(x, y, 1.0f, 0.5f, 0.1f);
    }

    float explosionRadius = 100.0f;
    for (auto& block : blocks) {
        if (block.getActive() && !block.getIsIndestructible()) {
            float dx = (block.getX() + block.getWidth() / 2) - x;
            float dy = (block.getY() + block.getHeight() / 2) - y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance < explosionRadius) {
                block.setActive(false);
                blocksRemaining--;
                score += 5;
                for (int i = 0; i < 10; i++) {
                    drawParticles(block.getX() + block.getWidth() / 2,
                        block.getY() + block.getHeight() / 2,
                        0.8f, 0.8f, 0.2f);
                }
            }
        }
    }
}

void Game::drawParticles(float x, float y, float r, float g, float b) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int i = 0; i < 6; i++) {
        float angle = 2.0f * M_PI * (rand() % 100) / 100.0f;
        float speed = 1.5f + (rand() % 100) / 25.0f;
        float px = x + cos(angle) * speed;
        float py = y + sin(angle) * speed;
        float size = 1.2f + (rand() % 100) / 60.0f;
        float alpha = 0.6f + (rand() % 100) / 200.0f;

        glColor4f(r, g, b, alpha);
        glBegin(GL_QUADS);
        glVertex2f(px - size, py - size);
        glVertex2f(px + size, py - size);
        glVertex2f(px + size, py + size);
        glVertex2f(px - size, py + size);
        glEnd();
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::drawTransparentRect(float x, float y, float w, float h, float r, float g, float b, float a) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    glDisable(GL_BLEND);
}

void Game::drawHeart(float x, float y, float size, float r, float g, float b, float alpha) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(r, g, b, alpha);
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y + size * 0.5f);

    for (int i = 0; i <= 20; i++) {
        float t = (float)i / 20.0f * 2.0f * M_PI;
        float heartX = size * 0.5f * sin(t) * sin(t) * sin(t);
        float heartY = size * 0.5f * (0.8f * cos(t) - 0.3f * cos(2 * t) - 0.1f * cos(3 * t) - 0.05f * cos(4 * t));
        glVertex2f(x + heartX, y + heartY);
    }

    glEnd();
    glDisable(GL_BLEND);
}

void Game::updateSnowflakes() {
    for (int i = 0; i < snowflakes.size(); i++) {
        snowflakes[i].second -= snowflakeSpeeds[i];
        snowflakes[i].first += sin(timeCounter + i) * 0.2f;
        if (snowflakes[i].second < 0) {
            snowflakes[i].second = 600;
            snowflakes[i].first = rand() % 800;
        }
        if (snowflakes[i].first < 0) snowflakes[i].first = 800;
        if (snowflakes[i].first > 800) snowflakes[i].first = 0;
    }
}

void Game::drawSnowflakes() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.8f);
    glBegin(GL_POINTS);
    for (int i = 0; i < snowflakes.size(); i++) {
        float brightness = 0.6f + (rand() % 100) / 250.0f;
        glColor3f(brightness, brightness, brightness);
        glVertex2f(snowflakes[i].first, snowflakes[i].second);
    }
    glEnd();
}

void Game::drawGarlands() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < garlands.size(); i++) {
        float flash = (sin(timeCounter * 4.0f + i * 0.5f) + 1.0f) / 2.0f;

        if (i % 3 == 0) {
            glColor4f(1.0f, 0.0f, 0.0f, 0.8f * flash);
        }
        else if (i % 3 == 1) {
            glColor4f(0.0f, 1.0f, 0.0f, 0.8f * flash);
        }
        else {
            glColor4f(1.0f, 1.0f, 0.0f, 0.8f * flash);
        }

        glPointSize(6.0f);
        glBegin(GL_POINTS);
        glVertex2f(garlands[i].first, garlands[i].second);
        glEnd();

        if (i < garlands.size() - 1) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.4f);
            glLineWidth(1.5f);
            glBegin(GL_LINES);
            glVertex2f(garlands[i].first, garlands[i].second);
            glVertex2f(garlands[i + 1].first, garlands[i + 1].second);
            glEnd();
        }
    }

    glDisable(GL_BLEND);
}

void Game::checkLevelCompletion() {
    if (blocksRemaining <= 0 && !levelComplete) {
        levelComplete = true;
        levelCompleteAlpha = 0.0f;
        score += 150 * currentLevel;
    }
}