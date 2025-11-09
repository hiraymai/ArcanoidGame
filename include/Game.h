#ifndef GAME_H
#define GAME_H

#include <vector>

class Game {
private:
    int width, height;
    bool gameOver;
    int score;
    int lives;
    bool gameWon;
    
    // Player paddle
    int paddleX, paddleY;
    int paddleWidth;
    
    // Ball
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    bool ballStuck;
    
    // Bricks
    struct Brick {
        int x, y;
        bool destroyed;
    };
    std::vector<Brick> bricks;
    
public:
    Game(int w, int h);
    void Initialize();
    void Draw();
    void Update();
    void ProcessInput(char input);
    bool IsGameOver() const;
    bool IsGameWon() const;
    void MovePaddle(int direction);
    void LaunchBall();
    int GetScore() const { return score; }
    int GetLives() const { return lives; }
    void DrawHearts();
};

#endif
