# A line starting with a has is a comment, we'll build up
# the below example

TARGET = A2
SRC_FILES = main.cpp wumpus_functions.cpp

# DO NOT EDIT BELOW HERE
CXX = g++
CFLAGS = -Wall -g -std=c++11
OBJECTS = $(SRC_FILES:.cpp=.o)
ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
else
	DEL = rm
endif

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	$(DEL) $(TARGET) $(OBJECTS)

# DEPENDENCIES
main.o: main.cpp wumpus_functions.h
wumpus_functions.o: wumpus_functions.h wumpus_functions.cpp