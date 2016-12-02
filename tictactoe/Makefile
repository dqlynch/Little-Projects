CXX = g++
FLAGS = -std=c++14 -Wall -g3 -lncurses

SOURCES = tic.cpp
HEADERS = tic.h
OBJECTS = $(SOURCES:.cpp=.o)


all: driver

driver: $(OBJECTS)
	$(CXX) $(FLAGS) $(OBJECTS) driver.cpp -o tictactoe

clean:
	rm tictactoe *.o

.cpp.o: $<.h
	$(CXX) $(FLAGS) $< -c -o $@
