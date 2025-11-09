#!/bin/bash
echo "Installing dependencies for Arcanoid Game on macOS..."

# Проверяем, установлен ли Homebrew
if ! command -v brew &> /dev/null; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Проверяем, установлен ли clang++
if ! command -v clang++ &> /dev/null; then
    echo "Xcode command line tools are required..."
    xcode-select --install
    echo "Please complete Xcode installation and run this script again."
    exit 1
fi

# Устанавливаем SFML
echo "Installing SFML..."
brew install sfml

echo "Dependencies installed successfully!"
echo "You can now build the game with: make"
