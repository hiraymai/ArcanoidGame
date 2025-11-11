CXX = g++
CXXFLAGS = -std=c++11 -I./include -L./lib
LIBS = -lfreeglut -lopengl32 -lglu32
TARGET = arcanoid.exe
SOURCES = src/main.cpp src/Game.cpp src/Ball.cpp src/Paddle.cpp src/Block.cpp src/Bomb.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS) $(LIBS)

clean:
	del $(TARGET)

run: $(TARGET)
	.\$(TARGET)

.PHONY: clean run