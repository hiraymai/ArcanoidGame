#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game(int w, int h) : width(w), height(h), gameOver(false), score(0), lives(3), gameWon(false) {
    std::srand(static_cast<unsigned>(std::time(0)));
    Initialize();
}

void Game::Initialize() {
    // Initialize paddle
    paddleWidth = 10;
    paddleX = width / 2 - paddleWidth / 2;
    paddleY = height - 3;
    
    // Initialize ball
    ballX = width / 2;
    ballY = height - 4;
    ballSpeedX = 0.0f;
    ballSpeedY = 0.0f;
    ballStuck = true;
    
    // Initialize bricks
    bricks.clear();
    for (int y = 3; y < 8; y++) {
        for (int x = 3; x < width - 3; x += 4) {
            Brick brick;
            brick.x = x;
            brick.y = y;
            brick.destroyed = false;
            bricks.push_back(brick);
        }
    }
}

void Game::DrawHearts() {
    std::cout << "Lives: ";
    for (int i = 0; i < 3; i++) {
        if (i < lives) {
            std::cout << "❤️ ";
        } else {
            std::cout << "♡ ";
        }
    }
    std::cout << " ";
}

void Game::Draw() {
    // Clear screen
    std::cout << "\033[2J\033[1;1H";
    
    // Draw header with hearts
    std::cout << "ARCADNOID - Score: " << score << " ";
    DrawHearts();
    std::cout << "\n";
    
    // Draw top border
    for (int x = 0; x < width + 2; x++)
        std::cout << "█";
    std::cout << "\n";
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Draw left border
            if (x == 0)
                std::cout << "█";
            
            // Draw ball
            if (x == static_cast<int>(ballX) && y == static_cast<int>(ballY))
                std::cout << "●";
            // Draw paddle
            else if (y == paddleY && x >= paddleX && x < paddleX + paddleWidth)
                std::cout << "═";
            // Draw bricks
            else {
                bool brickDrawn = false;
                for (const auto& brick : bricks) {
                    if (!brick.destroyed && brick.x == x && brick.y == y) {
                        std::cout << "■";
                        brickDrawn = true;
                        break;
                    }
                }
                if (!brickDrawn)
                    std::cout << " ";
            }
            
            // Draw right border
            if (x == width - 1)
                std::cout << "█";
        }
        std::cout << "\n";
    }
    
    // Draw bottom border
    for (int x = 0; x < width + 2; x++)
        std::cout << "█";
    std::cout << "\n";
    
    // Draw controls
    if (ballStuck) {
        std::cout << ">>> Press SPACE to launch ball! <<<\n";
    }
    std::cout << "Controls: A-left, D-right, SPACE-launch, Q-quit\n";
}

void Game::Update() {
    if (ballStuck) {
        // Ball follows paddle when stuck
        ballX = paddleX + paddleWidth / 2;
        ballY = height - 4;
        return;
    }
    
    // Update ball position
    ballX += ballSpeedX;
    ballY += ballSpeedY;
    
    // Ball collision with walls
    if (ballX <= 1) {
        ballX = 1;
        ballSpeedX = -ballSpeedX;
    }
    if (ballX >= width - 2) {
        ballX = width - 2;
        ballSpeedX = -ballSpeedX;
    }
    if (ballY <= 1) {
        ballY = 1;
        ballSpeedY = -ballSpeedY;
    }
    
    // Ball collision with paddle
    if (ballY >= paddleY - 1 && ballY <= paddleY &&
        ballX >= paddleX && ballX < paddleX + paddleWidth) {
        
        // Calculate hit position on paddle (-1 to 1)
        float hitPos = (ballX - paddleX) / static_cast<float>(paddleWidth) - 0.5f;
        hitPos *= 2.0f;
        
        // Adjust ball speed based on hit position
        ballSpeedX = hitPos * 0.8f;
        ballSpeedY = -fabs(ballSpeedY); // Always bounce up
        
        // Ensure minimum speed
        if (fabs(ballSpeedX) < 0.3f) ballSpeedX = (ballSpeedX > 0) ? 0.3f : -0.3f;
        if (fabs(ballSpeedY) < 0.3f) ballSpeedY = -0.3f;
        
        ballY = paddleY - 1; // Prevent sticking
    }
    
    // Ball collision with bricks
    for (auto& brick : bricks) {
        if (!brick.destroyed) {
            // Simple collision detection
            if (static_cast<int>(ballX) == brick.x && static_cast<int>(ballY) == brick.y) {
                brick.destroyed = true;
                ballSpeedY = -ballSpeedY; // Simple bounce
                score += 10;
                break;
            }
        }
    }
    
    // Ball lost - only lose life when ball goes below paddle
    if (ballY >= height) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
            gameWon = false;
        } else {
            // Reset ball
            ballStuck = true;
            ballX = paddleX + paddleWidth / 2;
            ballY = height - 4;
            ballSpeedX = 0;
            ballSpeedY = 0;
        }
    }
    
    // Check win condition
    bool allBricksDestroyed = true;
    for (const auto& brick : bricks) {
        if (!brick.destroyed) {
            allBricksDestroyed = false;
            break;
        }
    }
    if (allBricksDestroyed) {
        gameOver = true;
        gameWon = true;
    }
}

void Game::MovePaddle(int direction) {
    paddleX += direction;
    
    // Keep paddle within bounds
    if (paddleX < 1)
        paddleX = 1;
    if (paddleX > width - paddleWidth - 1)
        paddleX = width - paddleWidth - 1;
    
    // If ball is stuck, move it with paddle
    if (ballStuck) {
        ballX = paddleX + paddleWidth / 2;
    }
}

void Game::LaunchBall() {
    if (ballStuck) {
        ballStuck = false;
        ballSpeedX = (std::rand() % 2 == 0) ? 0.5f : -0.5f;
        ballSpeedY = -0.7f;
    }
}

void Game::ProcessInput(char input) {
    switch (input) {
        case 'a':
        case 'A':
            MovePaddle(-3);
            break;
        case 'd':
        case 'D':
            MovePaddle(3);
            break;
        case ' ':
            LaunchBall();
            break;
    }
}

bool Game::IsGameOver() const {
    return gameOver;
}

bool Game::IsGameWon() const {
    return gameWon;
}
