CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
SRCDIR = src
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/Game.cpp
TARGET = arcanoid

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
