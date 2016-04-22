#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ncurses.h>
#include <ctime>
#include <list>
#include <sys/ioctl.h>

#include "Game.h"

// LINUX/OS X SPECIFIC
// WILL NOT WORK ON WINDOWS
// REQUIRES NCURSES LIBRARY

int GameVars::MAX_BOARD_Y;  // values assigned in set_game_size
int GameVars::MAX_BOARD_X;
double GameVars::FRAMES_PER_SEC = 20;
int GameVars::STARTING_LENGTH = 20;  // must be < MAX_BOARD_X - 1

// FORWARD DECLARATIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

// Sets last 5 characters of snake + eraser on the tail
void set_name(std::list<Coords>& snake_list);


// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main() {
  init_ncurses();

  run_game();

  endwin();
}

// FUNCTION IMPLEMENTATIONS - - - - - - - - - - - - - - - - - - - - - - - - - -

void run_game() {

  set_game_size();

  std::list<Coords> snake_list;
  for (int i = 0; i < GameVars::STARTING_LENGTH + 1; ++i) {
    Coords link = {1 + i, GameVars::MAX_BOARD_Y / 2, '#'};
    snake_list.push_back(link);
  }
  set_name(snake_list);
  clear();
  print_board();
  print_snake(snake_list);

  // Wait for initial direction input
  int direction = 1;
  while(!get_dir(direction)) {
    if (set_game_size()) {
      clear();
      print_board();
      print_snake(snake_list);
    }
  }

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
  mvprintw(GameVars::MAX_BOARD_Y / 2, GameVars::MAX_BOARD_X - 5, "Play again?");
  mvprintw(GameVars::MAX_BOARD_Y / 2 + 1, GameVars::MAX_BOARD_X - 2, "(y/n)");

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
  for (int col = 1; col < 2 * GameVars::MAX_BOARD_X; ++col) {
    mvprintw(0, col, "-");
    mvprintw(GameVars::MAX_BOARD_Y, col, "-");
  }

  for (int row = 1; row < GameVars::MAX_BOARD_Y; ++row) {
    mvprintw(row, 0, "|");
    mvprintw(row, 2 * GameVars::MAX_BOARD_X, "|");
  }
}

void print_prompt() {
  int halfx = GameVars::MAX_BOARD_X;
  int halfy = GameVars::MAX_BOARD_Y / 2;
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
  if (head.x == 0 || head.x == GameVars::MAX_BOARD_X ||
      head.y == 0 || head.y == GameVars::MAX_BOARD_Y) {
    return true;
  }
  return false;
}

void game_frame(std::list<Coords>& snake_list, int& direction) {
  // Wait and accept up to 1 direction change
  clock_t start = clock();
  bool changed = false;
  while( ((clock()-start) / (double) CLOCKS_PER_SEC) < (1 / GameVars::FRAMES_PER_SEC)) {
    if (!changed) {
      changed = get_dir(direction);
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
    if (coords.x < GameVars::MAX_BOARD_X) {
      ++coords.x;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
  else if (direction == 2) {
    if (coords.y < GameVars::MAX_BOARD_Y) {
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
  mvprintw(GameVars::MAX_BOARD_Y + 1, 0, "%d,%d\r",snake_list.back().x, snake_list.back().y);
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
}

bool set_game_size() {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);

  int y_save = GameVars::MAX_BOARD_Y;
  int x_save = GameVars::MAX_BOARD_X;

  GameVars::MAX_BOARD_Y = w.ws_row - 1;
  GameVars::MAX_BOARD_X = (w.ws_col / 2) - 1;

  if (y_save != GameVars::MAX_BOARD_Y ||
      x_save != GameVars::MAX_BOARD_X) {
    // Only resize term if window size changed
    resizeterm(w.ws_row, w.ws_col);
    return true;
  }
  return false;
}
