// Implementation of Snake_Game.h snake-specific functions
#include "SnakeGame.h"

bool check_collisions(const SnakeInfo& snake) {
  // Only need to check if head collides
  const Coords head = snake.list.back();

  // Check self collisions
  if (check_collision_with_snake(snake, head)) {
    return true;
  }

  // Check wall collisions
  if (head.x == 0 || head.x == ScreenVars::MAX_BOARD_X ||
      head.y == 0 || head.y == ScreenVars::MAX_BOARD_Y) {
    return true;
  }
  return false;
}

bool check_collision_with_snake(const SnakeInfo& snake, const Coords& obj) {
  std::list<Coords>::const_iterator it = snake.list.begin();
  while (++it != --snake.list.end()) {
    if (obj.x == it->x && obj.y == it->y) {
      return true;
    }
  }
  return false;
}

void game_frame(SnakeInfo& snake) {
  // Wait and accept up to 1 direction change
  clock_t start = clock();
  bool changed = false;
  while(((clock()-start) / (double) CLOCKS_PER_SEC) <
         (1 / GameVars::FRAMES_PER_SEC)) {
    if (!changed) {
      changed = get_dir(snake);
    }
  }
  move_snek(snake);
  print_snake(snake);
}

bool play_again(SnakeInfo snake) {
  print_play_again_prompt();

  while (1) {

    char input = 0;
    if (kbhit()) {
      input = getch();
      if (input == 'y' || input == 'Y') {
        return true;
      }
      if (input == 'n' || input == 'N') {
        return false;
      }
    }

    if (set_game_size()) {
      update_board(snake);
      print_play_again_prompt();
    }
  }
}

void print_play_again_prompt() {
  print_prompt();
  mvprintw(ScreenVars::MAX_BOARD_Y / 2,
           ScreenVars::MAX_BOARD_X - 5,
           "Play again?");
  mvprintw(ScreenVars::MAX_BOARD_Y / 2 + 1,
           ScreenVars::MAX_BOARD_X - 2,
           "(y/n)");
}

bool get_dir(SnakeInfo& snake) {
  int input = 0;
  // Only get char if keyboard is hit
  if (kbhit()) {
    input = getch();
  }
  if (input == KEY_UP && snake.direction != 2) {
    snake.direction = 0;
    return true;
  }
  if (input == KEY_RIGHT && snake.direction != 3) {
    snake.direction = 1;
    return true;
  }
  if (input == KEY_DOWN && snake.direction != 0) {
    snake.direction = 2;
    return true;
  }
  if (input == KEY_LEFT && snake.direction != 1) {
    snake.direction = 3;
    return true;
  }
  return false;
}

std::list<Coords> create_snake() {
  // Determine how long to make snake
  GameVars::STARTING_LENGTH = ScreenVars::MAX_BOARD_X / 3 + 1;
  // GameVars::STARTING_LENGTH = 1;   // optional mode

  // Make length of STARTING_LENGTH + 1 because last link is invisible
  SnakeInfo snake;
  for (int i = 0; i < GameVars::STARTING_LENGTH + 1; ++i) {
    Coords link = {1 + i, ScreenVars::MAX_BOARD_Y / 2, '!'};
    snake.list.push_back(link);
  }
  set_name(snake);
  return snake.list;
}

void set_name(SnakeInfo& snake) {
  // Last character always a space to erase over snake tail
  std::list<Coords>::iterator it = snake.list.begin();
  it++->symbol = ' ';    // tail
  while (it != --snake.list.end()) {
    it++->symbol = '#';  // body
  }
  it->symbol = '@';      // head
}

void move_snek(SnakeInfo& snake) {
  Coords new_coord = {snake.list.back().x,
                      snake.list.back().y,
                      snake.list.back().symbol};
  if (snake.direction == 3) {
    --new_coord.x;
  }
  else if (snake.direction == 1) {
    ++new_coord.x;
  }
  else if (snake.direction == 2) {
    ++new_coord.y;
  }
  else if (snake.direction == 0) {
    --new_coord.y;
  }

  // Shift symbols up by one position (back of list is front of snake)
  char symbol_pass = ' ';
  for (auto&& coord : snake.list) {
    char symbol_receive = symbol_pass;  // Receive symbol from prev
    symbol_pass = coord.symbol;         // Give symbol to next
    coord.symbol = symbol_receive;
  }

  // Must set new tail as an eraser
  Coords save_tail = snake.list.front();
  snake.list.pop_front();
  snake.list.push_back(new_coord);
  snake.list.front().symbol = ' ';

  // Check if fruit get
  if (check_fruit_get(snake)) {
    snake.list.front().symbol = snake.fruit.symbol;
    snake.list.push_front(save_tail);
    snake.list.front().symbol = ' ';
    generate_fruit(snake);
  }
}

void print_snake(const SnakeInfo& snake) {
  for (std::list<Coords>::const_iterator it = snake.list.begin();
       it != snake.list.end();
       ++it) {
    mvprintw(it->y, 2 * it->x, "%c", it->symbol);
  }

  mvprintw(snake.fruit.y, 2 * snake.fruit.x, "%c", snake.fruit.symbol);

  // // Fruit coords
  // mvprintw(ScreenVars::MAX_BOARD_Y - 2, 0,
  //          "%d,%d\r",snake.fruit.y, snake.fruit.x);
  //
  // // Snake coords
  // mvprintw(ScreenVars::MAX_BOARD_Y - 1, 0,
  //          "%d,%d\r",snake.list.back().y, snake.list.back().x);
  refresh();
}

void generate_fruit(SnakeInfo& snake) {
  // Generate new fruit coordinates that are not on snake body
  Coords head = snake.list.back();
  do {
    int rand_x = rand() % (ScreenVars::MAX_BOARD_X - 1) + 1;
    int rand_y = rand() % (ScreenVars::MAX_BOARD_Y - 1) + 1;
    snake.fruit = {rand_x, rand_y, '!'};
  } while (check_collision_with_snake(snake, snake.fruit) ||
           (snake.fruit.x == head.x &&
            snake.fruit.y == head.y));

  increment_fruit_word(snake);
}

void increment_fruit_word(SnakeInfo& snake) {
  // Cycle symbol through word of choice
  if (snake.fruit_word_index == snake.fruit_word.size() - 1) {
    snake.fruit_word_index = 0;
  }
  else {
    ++snake.fruit_word_index;
  }
  snake.fruit.symbol = snake.fruit_word[snake.fruit_word_index];
}

bool check_fruit_get(SnakeInfo& snake) {
  Coords head = snake.list.back();
  if (head.x == snake.fruit.x && head.y == snake.fruit.y) {
    // mvprintw(1, 1, "FRUIT GOT");
    refresh();
    return true;
  }
  return false;
}

void update_board(const SnakeInfo& snake) {
  clear();
  print_board();
  print_snake(snake);
}
