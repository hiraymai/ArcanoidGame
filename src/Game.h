#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Paddle.h"
#include "Block.h"
#include "Bomb.h"
#include <vector>

class Game {
private:
    Ball ball;
    Paddle paddle;
    std::vector<Block> blocks;
    std::vector<Bomb> bombs;
    std::vector<std::pair<float, float>> snowflakes;
    std::vector<float> snowflakeSpeeds;
    std::vector<std::pair<float, float>> garlands;
    int score;
    int lives;
    int currentLevel;
    bool gameOver;
    bool gameWon;
    bool levelComplete;
    float gameOverAlpha;
    float gameWonAlpha;
    float levelCompleteAlpha;
    float fadeDirection;
    int blocksRemaining;
    bool ballLaunched;
    float ballSpeed;
    float timeCounter;

public:
    Game();
    void init();
    void draw();
    void update();
    void handleInput(unsigned char key, int x, int y);
    void handleSpecialInput(int key, int x, int y);
    void handleMouseMove(int x, int y);
    void resetBall();
    void launchBall();
    void loadLevel(int level);
    void checkLevelCompletion();
    void explodeBomb(float x, float y);
    void drawParticles(float x, float y, float r, float g, float b);
    void drawTransparentRect(float x, float y, float w, float h, float r, float g, float b, float a);
    void drawHeart(float x, float y, float size, float r, float g, float b, float alpha);
    void updateSnowflakes();
    void drawSnowflakes();
    void drawGarlands();
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }
    int getScore() const { return score; }
    int getLives() const { return lives; }
    int getCurrentLevel() const { return currentLevel; }
};

#endif