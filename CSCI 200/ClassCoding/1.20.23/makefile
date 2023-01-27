TARGET = Random
SRC_FILES = main.cpp

CFLAGS = -Wall -g
OBJECTS = $(SRC_FILES:.cpp=.o)

ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
else
	DEL = rm
endif

debug: CFLAGS += -DDEBUG
debug: CLEAN

$(TARGET): main.o
	g++ -o $@ $^

%.o: %.cpp
	g++ $(CFLAGS) -o $@ -c $<
	
clean:
	$(DEL) $(TARGET) $(OBJECTS)
