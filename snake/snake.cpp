#include <iostream>
#include <ncurses.h>
#include <list>
#include "Screen.h"
#include "SnakeGame.h"

// LINUX/OS X SPECIFIC
// WILL NOT WORK ON WINDOWS
// REQUIRES NCURSES LIBRARY

int ScreenVars::MAX_BOARD_Y;  // values assigned in set_game_size
int ScreenVars::MAX_BOARD_X;
double GameVars::FRAMES_PER_SEC = 20;
int GameVars::STARTING_LENGTH = 20;  // must be < MAX_BOARD_X - 1

// FORWARD DECLARATIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Runs game
void run_game();

// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main() {
  init_ncurses();
  run_game();
  endwin();
}

// FUNCTION IMPLEMENTATIONS - - - - - - - - - - - - - - - - - - - - - - - - - -

void run_game() {

  // Set up initial gamestate
  set_game_size();
  std::list<Coords> snake_list = create_snake();
  update_board(snake_list);

  // Wait for initial direction input
  int direction = 1;
  while(!get_dir(direction)) {
    if (set_game_size()) {
      snake_list = create_snake();
      update_board(snake_list);
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
