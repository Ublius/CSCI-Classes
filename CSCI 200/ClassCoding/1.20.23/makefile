TARGET = Random
SRC_FILES = main.cpp

OBJECTS = $(SRC_FILES:.cpp=.o)

ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
else
	DEL = rm
endif

$(TARGET): main.o
	g++ -o $@ $^

%.o: %.cpp
	g++ $@ -c $<

clean:
	$(DEL) $(TARGET) $(OBJECTS)
