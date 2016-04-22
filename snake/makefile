play: snake
	./snake

snake: snake.cpp SnakeGame.cpp Screen.cpp
	g++ -std=c++11 snake.cpp Screen.cpp SnakeGame.cpp -lncurses -O1 -o snake

clean:
	\rm snake
