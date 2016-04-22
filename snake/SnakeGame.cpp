// Implementation of Snake_Game.h snake-specific functions
#include "SnakeGame.h"

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
  // Check wall collisions
  if (head.x == 0 || head.x == ScreenVars::MAX_BOARD_X ||
      head.y == 0 || head.y == ScreenVars::MAX_BOARD_Y) {
    return true;
  }
  return false;
}

void game_frame(std::list<Coords>& snake_list, int& direction) {
  // Wait and accept up to 1 direction change
  clock_t start = clock();
  bool changed = false;
  while( ((clock()-start) /
          (double) CLOCKS_PER_SEC) < (1 / GameVars::FRAMES_PER_SEC)) {
    if (!changed) {
      changed = get_dir(direction);
    }
  }
  move_snek(snake_list, direction);
  print_snake(snake_list);
}

bool play_again() {
  print_prompt();
  mvprintw(ScreenVars::MAX_BOARD_Y / 2,
           ScreenVars::MAX_BOARD_X - 5,
           "Play again?");
  mvprintw(ScreenVars::MAX_BOARD_Y / 2 + 1,
           ScreenVars::MAX_BOARD_X - 2,
           "(y/n)");

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

std::list<Coords> create_snake() {
  // Determine how long to make snake
  GameVars::STARTING_LENGTH = ScreenVars::MAX_BOARD_X / 3 + 1;

  // Make length of STARTING_LENGTH + 1 because last link is invisible
  std::list<Coords> snake_list;
  for (int i = 0; i < GameVars::STARTING_LENGTH + 1; ++i) {
    Coords link = {1 + i, ScreenVars::MAX_BOARD_Y / 2, '!'};
    snake_list.push_back(link);
  }
  set_name(snake_list);
  return snake_list;
}

void move_snek(std::list<Coords>& snake_list, int direction) {
  Coords new_coord = {snake_list.back().x,
                      snake_list.back().y,
                      snake_list.back().symbol};

  if (direction == 3) {
    --new_coord.x;
  }
  else if (direction == 1) {
    ++new_coord.x;
  }
  else if (direction == 2) {
    ++new_coord.y;
  }
  else if (direction == 0) {
    --new_coord.y;
  }

  // Shift symbols back by one position
  char symbol_pass = ' ';
  for (auto&& coord : snake_list) {
    char symbol_receive = symbol_pass;  // Receive symbol from prev
    symbol_pass = coord.symbol;         // Give symbol to next
    coord.symbol = symbol_receive;
  }

  // Must set new tail as an eraser
  snake_list.pop_front();
  snake_list.push_back(new_coord);
  snake_list.front().symbol = ' ';
}

void print_snake(const std::list<Coords>& snake_list) {
  for (std::list<Coords>::const_iterator it = snake_list.begin();
       it != snake_list.end();
       ++it) {
    mvprintw(it->y, 2 * it->x, "%c", it->symbol);
  }

  // Print coords
  // mvprintw(ScreenVars::MAX_BOARD_Y + 1, 0,
  //          "%d,%d\r",snake_list.back().x, snake_list.back().y);
  refresh();
}

void set_name(std::list<Coords>& snake_list) {
  // Last character always a space to erase over snake tail
  std::list<Coords>::iterator it = snake_list.begin();
  it++->symbol = ' ';  // tail
  while (it != --snake_list.end()) {
    it++->symbol = '@';
  }
  it->symbol = 'X';  // head
}

void update_board(std::list<Coords> snake_list) {
  clear();
  print_board();
  print_snake(snake_list);
}
