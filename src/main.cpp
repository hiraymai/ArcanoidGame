#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

// Improved non-blocking input functions
struct TermiosSettings {
    struct termios original;
    
    TermiosSettings() {
        tcgetattr(STDIN_FILENO, &original);
    }
    
    ~TermiosSettings() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original);
    }
};

void SetNonBlockingInput(bool enable) {
    static TermiosSettings settings;
    static bool isSet = false;
    
    if (enable && !isSet) {
        struct termios newt = settings.original;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        isSet = true;
    } else if (!enable && isSet) {
        tcsetattr(STDIN_FILENO, TCSANOW, &settings.original);
        isSet = false;
    }
}

bool KeyPressed() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char GetKey() {
    char ch = 0;
    if (read(STDIN_FILENO, &ch, 1) != 1) {
        return 0;
    }
    return ch;
}

void ShowWelcomeScreen() {
    std::cout << "\033[2J\033[1;1H";
    std::cout << "========================================\n";
    std::cout << "           WELCOME TO ARCANOID!        \n";
    std::cout << "========================================\n";
    std::cout << "                                        \n";
    std::cout << "    Destroy all bricks with the ball!   \n";
    std::cout << "                                        \n";
    std::cout << "Controls:                               \n";
    std::cout << "  A - Move paddle left                  \n";
    std::cout << "  D - Move paddle right                 \n";
    std::cout << "  SPACE - Launch ball                   \n";
    std::cout << "  Q - Quit game                         \n";
    std::cout << "                                        \n";
    std::cout << "You have 3 lives ❤️ ❤️ ❤️              \n";
    std::cout << "Press ENTER to start...                 \n";
    std::cout << "========================================\n";
    
    // Wait for Enter key
    SetNonBlockingInput(false);
    std::cin.get();
    SetNonBlockingInput(true);
}

void ShowGameOver(int score, bool won) {
    SetNonBlockingInput(false);
    std::cout << "\033[2J\033[1;1H";
    std::cout << "========================================\n";
    if (won) {
        std::cout << "            YOU WIN! 🎉              \n";
    } else {
        std::cout << "            GAME OVER! 💀            \n";
    }
    std::cout << "========================================\n";
    std::cout << "                                        \n";
    std::cout << "        Final Score: " << score << "           \n";
    std::cout << "                                        \n";
    if (won) {
        std::cout << "   You destroyed all bricks!         \n";
    } else {
        std::cout << "   Better luck next time!           \n";
    }
    std::cout << "                                        \n";
    std::cout << "   Press ENTER to close...              \n";
    std::cout << "========================================\n";
    std::cin.get();
}

int main() {
    const int WIDTH = 50;
    const int HEIGHT = 25;
    
    ShowWelcomeScreen();
    
    Game game(WIDTH, HEIGHT);
    SetNonBlockingInput(true);
    
    bool quit = false;
    while (!game.IsGameOver() && !quit) {
        game.Draw();
        game.Update();
        
        // Check for input with better handling
        while (KeyPressed()) {
            char input = GetKey();
            if (input == 'q' || input == 'Q') {
                quit = true;
                break;
            }
            game.ProcessInput(input);
        }
        
        // Control game speed
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    SetNonBlockingInput(false);
    
    // Show appropriate game over screen
    ShowGameOver(game.GetScore(), game.IsGameWon());
    
    return 0;
}
