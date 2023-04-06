TARGET = A4
SRC_FILES = main.cpp Node.hpp LinkedList.hpp Game.cpp Player.cpp

CXX = g++
CFLAGS = -Wall
OBJECTS = $(SRC_FILES:.cpp=.o)
ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
else
	DEL = rm
endif

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^

debug: CFLAGS += -DDEBUG
debug: clean $(TARGET)

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	$(DEL) $(TARGET) $(OBJECTS)
