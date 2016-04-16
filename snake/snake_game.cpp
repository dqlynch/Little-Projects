#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ncurses.h>
#include <ctime>
#include <list>

// LINUX/OS X SPECIFIC
// WILL NOT WORK ON WINDOWS
// REQUIRES NCURSES LIBRARY

static const int MAX_BOARD_X = 50;
static const int MAX_BOARD_Y = 30;
static const double FRAMES_PER_SEC = 20;
static const int STARTING_LENGTH = 20;  // must be < MAX_BOARD_X - 1

struct Coords {
  int x;
  int y;
  char symbol;
};

// Runs game
void run_game();

// Asks player whether they want to play again or exits game
bool play_again();

// Prints the board borders
void print_board();

// Checks whether snek has collided into anything
bool check_collisions(const std::list<Coords>& snake_list);

// Executes one game frame
void game_frame(std::list<Coords>& snake_list, int& direction);

// Change direction of snake, can't change to opposite
bool get_dir(int& direction);

// Increments snake position according to dircetion
void move_snek(std::list<Coords>& snake_list, int direction);

// Prints current snake position
void print_snake(const std::list<Coords>& snake_list);

// Print prompt
void print_prompt();

// Sets last 5 characters of snake + eraser on the tail
void set_name(std::list<Coords>& snake_list);

// Checks for user keyboard hit (req if getch is nonblocking)
bool kbhit();

// Init all the ncurses specific stuff
void init_ncurses();

int main() {
  init_ncurses();

  run_game();

  // End game here TODO
  endwin();
}

// FUNCTION IMPLEMENTATIONS - - - - - - - - - - - - - - - - - - - -

void run_game() {


  std::list<Coords> snake_list;
  for (int i = 0; i < STARTING_LENGTH + 1; ++i) {
    Coords link = {1 + i, MAX_BOARD_Y / 2, '#'};
    snake_list.push_back(link);
  }

  clear();
  print_board();
  set_name(snake_list);
  print_snake(snake_list);

  // Wait for initial direction input
  int direction = 3;
  while(!get_dir(direction));

  while(!check_collisions(snake_list)) {
    game_frame(snake_list, direction);
  }
  if (!play_again()) {
    return;
  }
  else {
    run_game();
  }
}

bool play_again() {
  print_prompt();
  mvprintw(MAX_BOARD_Y / 2, MAX_BOARD_X - 5, "Play again?");
  mvprintw(MAX_BOARD_Y / 2 + 1, MAX_BOARD_X - 2, "(y/n)");

  while (1) {
    char input = 0;
    if (kbhit()) {
      input = getch();
    }

    if (input == 'y' || input == 'Y') {
      return true;
    }
    if (input == 'n' || input == 'N') {
      return false;
    }
  }
}

void print_board() {
  for (int col = 1; col < 2 * MAX_BOARD_X; ++col) {
    mvprintw(0, col, "-");
    mvprintw(MAX_BOARD_Y, col, "-");
  }

  for (int row = 1; row < MAX_BOARD_Y; ++row) {
    mvprintw(row, 0, "|");
    mvprintw(row, 2 * MAX_BOARD_X, "|");
  }
}

void print_prompt() {
  int halfx = MAX_BOARD_X;
  int halfy = MAX_BOARD_Y / 2;
  for (int i = halfx - 19; i <= halfx + 19; ++i) {
    for (int j = halfy - 4; j <= halfy + 4; ++j) {
      mvprintw(j, i, " ");
    }
  }
  for (int i = halfx - 19; i <= halfx + 19; ++i) {
    mvprintw(halfy - 5, i, "_");
    mvprintw(halfy + 5, i, "-");
  }
  for (int i = halfy - 4; i <= halfy + 4; ++i) {
    mvprintw(i, halfx - 20, "|");
    mvprintw(i, halfx + 20, "|");
  }
}

bool check_collisions(const std::list<Coords>& snake_list) {
  // Only need to check head
  const Coords head = snake_list.back();

  // Check self collisions
  std::list<Coords>::const_iterator it = snake_list.begin();
  while (++it != --snake_list.end()) {
    if (head.x == it->x && head.y == it->y) {
      return true;
    }
  }
  // Check wall check_collisions
  if (head.x == 0 || head.x == MAX_BOARD_X ||
      head.y == 0 || head.y == MAX_BOARD_Y) {
    return true;
  }
  return false;
}

void game_frame(std::list<Coords>& snake_list, int& direction) {
  // Wait and accept up to 1 direction change
  clock_t start = clock();
  bool changed = false;
  while( ((clock()-start) / (double) CLOCKS_PER_SEC) < (1 / FRAMES_PER_SEC)) {
    if (!changed) {
      if (get_dir(direction)) {
        changed = true;
      }
    }
  }
  move_snek(snake_list, direction);
  set_name(snake_list);
  print_snake(snake_list);
}

bool get_dir(int& direction) {
  int input = 0;
  // Only get char if keyboard is hit
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

void move_snek(std::list<Coords>& snake_list, int direction) {
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
    if (coords.x < MAX_BOARD_X) {
      ++coords.x;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
  else if (direction == 2) {
    if (coords.y < MAX_BOARD_Y) {
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

void print_snake(const std::list<Coords>& snake_list) {
  for (std::list<Coords>::const_iterator it = snake_list.begin();
       it != snake_list.end();
       ++it) {
    mvprintw(it->y, 2 * it->x, "%c", it->symbol);
  }

  // Coords
  mvprintw(MAX_BOARD_Y + 1, 0, "%d,%d\r",snake_list.back().x, snake_list.back().y);
  refresh();
}

void set_name(std::list<Coords>& snake_list) {
  // Last character always a space to erase over snake tail
  std::list<Coords>::iterator it = snake_list.begin();
  it++->symbol = ' ';  // tail
  while (it != --snake_list.end()) {
    it++->symbol = '#';
  }
  it->symbol = 'X';  // head
}

bool kbhit() {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return true;
    }
    else {
        return false;
    }
}

void init_ncurses() {
  initscr();
  curs_set(0);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);  // getch can't block clock
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  resizeterm(MAX_BOARD_Y + 10, 2 * MAX_BOARD_X + 10);
}
