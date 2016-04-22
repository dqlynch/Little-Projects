// Implementation of Snake_Game.h snake-specific functions
#include "Snake_Game.h"

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
  while( ((clock()-start) / (double) CLOCKS_PER_SEC) < (1 / GameVars::FRAMES_PER_SEC)) {
    if (!changed) {
      changed = get_dir(direction);
    }
  }
  move_snek(snake_list, direction);
  set_name(snake_list);
  print_snake(snake_list);
}

bool play_again() {
  print_prompt();
  mvprintw(ScreenVars::MAX_BOARD_Y / 2, ScreenVars::MAX_BOARD_X - 5, "Play again?");
  mvprintw(ScreenVars::MAX_BOARD_Y / 2 + 1, ScreenVars::MAX_BOARD_X - 2, "(y/n)");

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
    if (coords.x < ScreenVars::MAX_BOARD_X) {
      ++coords.x;
      snake_list.push_back(coords);
      snake_list.pop_front();
    }
    return;
  }
  else if (direction == 2) {
    if (coords.y < ScreenVars::MAX_BOARD_Y) {
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
  mvprintw(ScreenVars::MAX_BOARD_Y + 1, 0, "%d,%d\r",snake_list.back().x, snake_list.back().y);
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
