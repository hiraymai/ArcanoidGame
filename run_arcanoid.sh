#!/bin/bash

# Компилируем игру если нужно
make

# Открываем новое окно терминала и запускаем игру
osascript -e 'tell application "Terminal" to do script "cd \"'$(pwd)'\" && ./arcanoid"'
