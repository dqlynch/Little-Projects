#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>   // sleep
#include <ctime>
#include <list>

// LINUX/OS X SPECIFIC
// WILL NOT WORK ON WINDOWS
// REQUIRES NCURSES LIBRARY

static const int MAX_BOARD_COORD = 30;
static const double FRAMES_PER_SEC = 20;
static const int STARTING_LENGTH = 15;  // must be >= 5

struct Coords {
  int x;
  int y;
  char symbol;
};

// Waits for one frame cycle, accepts up to one direction change throughout
void gameFrame(int& direction);

// Change direction of snake, can't change to opposite
bool getDir(int& direction);

// Increments snake position according to dircetion
void moveSnek(std::list<Coords>& snake_list, int direction);

// Prints current snake pos
void printBoard(std::list<Coords>& snake_list);

// Sets name of snake
void setName(std::list<Coords>& snake_list);

// Checks for user keyboard hit (req if getch is nonblocking)
bool kbhit();

int main() {

  initscr();
  curs_set(0);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);  // getch can't block clock
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);

  // 0 = up, clockwise to 3 = left
  int direction = 1;

  Coords link0 = {0, 0, 'S'};
  Coords link1 = {0, 0, 'S'};
  Coords link2 = {1, 0, 'N'};
  Coords link3 = {2, 0, 'A'};
  Coords link4 = {3, 0, 'K'};
  Coords link5 = {4, 0, 'E'};

  std::list<Coords> snake_list = std::list<Coords>();
  snake_list.push_back(link5);
  snake_list.push_back(link4);
  snake_list.push_back(link3);
  snake_list.push_back(link2);
  snake_list.push_back(link1);
  for (int i = 0; i < STARTING_LENGTH; ++i) {
    snake_list.push_back(link0);
  }

  printBoard(snake_list);

  while(1) {
    moveSnek(snake_list, direction);
    setName(snake_list);
    printBoard(snake_list);
    gameFrame(direction);
  }
}

void gameFrame(int& direction) {
  clock_t start = clock();
  bool changed = false;
  while( ((clock()-start) / (double) CLOCKS_PER_SEC) < (1 / FRAMES_PER_SEC)) {
    if (!changed) {
      if (getDir(direction)) {
        changed = true;
      }
    }
  }
}

bool getDir(int& direction) {
  int input = 0;
  if (kbhit()) {
    input = getch();
  }
  if (input == KEY_UP && direction != 2) {
    direction = 0;
    return true;
  }
  if (input == KEY_RIGHT && direction != 3) {
    direction = 1;
    return true;
  }
  if (input == KEY_DOWN && direction != 0) {
    direction = 2;
    return true;
  }
  if (input == KEY_LEFT && direction != 1) {
    direction = 3;
    return true;
  }
  return false;
}

void moveSnek(std::list<Coords>& snake_list, int direction) {
  Coords coords = {snake_list.back().x, snake_list.back().y, 'S'};

  if (direction == 3) {
    if (coords.x > 0) {
      --coords.x;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
  else if (direction == 1) {
    if (coords.x < MAX_BOARD_COORD) {
      ++coords.x;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
  else if (direction == 2) {
    if (coords.y < MAX_BOARD_COORD) {
      ++coords.y;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
  else if (direction == 0) {
    if (coords.y > 0) {
      --coords.y;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
}

void printBoard(std::list<Coords>& snake_list) {
  system("clear");
  for (int row = 0; row <= MAX_BOARD_COORD; ++row) {
    for (int col = 0; col <= MAX_BOARD_COORD; ++col) {

      bool snek_found = false;
      std::list<Coords>::iterator itend = snake_list.end();
      for (std::list<Coords>::iterator it = snake_list.begin(); it != itend; ++it) {
        if ((*it).x == col && (*it).y == row) {
          printf("%c  ", (*it).symbol);
          snek_found = true;
          break;
        }
      }
      if (!snek_found) {
        printf("   ");
      }
    }
    printf("\r\n");
  }
  printf("%d,%d\r",snake_list.back().x, snake_list.back().y);
  refresh();
}

void setName(std::list<Coords>& snake_list) {
  std::list<Coords>::iterator it = snake_list.begin();
  (*it).symbol = 'E';
  (*++it).symbol = 'K';
  (*++it).symbol = 'A';
  (*++it).symbol = 'N';
  (*++it).symbol = 'S';
}

bool kbhit() {
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return true;
    } else {
        return false;
    }
}
