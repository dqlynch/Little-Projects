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
double GameVars::FRAMES_PER_SEC = 10;
int GameVars::STARTING_LENGTH = 20;  // must be < MAX_BOARD_X - 1

// FORWARD DECLARATIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Runs entire game from start
void start_new_game();

// Runs one instance of the game
void run_game(SnakeInfo& snake);

// Set up initial gamestate
SnakeInfo set_up_new_game();

// Gets initial direction of mov't
// Blocking fn, breaks on user input
// Can resize during blocked state
void get_init_direction(SnakeInfo& snake);

// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main() {
  init_ncurses();
  start_new_game();
  endwin();
}

// FUNCTION IMPLEMENTATIONS - - - - - - - - - - - - - - - - - - - - - - - - - -

void start_new_game() {
  SnakeInfo snake = set_up_new_game();

  // Wait for user to start game
  get_init_direction(snake);
  generate_fruit(snake);
  move_snek(snake);
  print_snake(snake);

  run_game(snake);

  if (play_again(snake)) {
    start_new_game();
  }
}

void run_game(SnakeInfo& snake) {
  while(!check_collisions(snake)) {
    game_frame(snake);
  }
}

SnakeInfo set_up_new_game() {
  set_game_size();
  Coords temp_fruit = {0, 0, ' '};
  std::string fruit_message = pick_fruit_message();

  //                list, direction, fruit, fruit_word, fruit_word_index
  // index starts at -1 because it must be incremented before printing
  SnakeInfo snake = {create_snake(), 1, temp_fruit, fruit_message, -1};
  update_board(snake);
  return snake;
}

void get_init_direction(SnakeInfo& snake) {
  while(!get_dir(snake)) {
    if (set_game_size()) {
      snake.list = create_snake();
      update_board(snake);
    }
  }
}
