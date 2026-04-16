CXX = g++
CXXFLAGS = -Wall -std=c++17 -MMD -MP $(shell pkg-config --cflags sdl2 SDL2_image)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image)

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
D = $(SRC:.cpp=.d)
TARGET = GameEngine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(TARGET) $(D)

-include $(OBJ:.o=.d)
