#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ncurses.h>
#include <ctime>
#include <list>

// LINUX/OS X SPECIFIC
// WILL NOT WORK ON WINDOWS
// REQUIRES NCURSES LIBRARY

// Split off of snake base game

// Player controls
int CONTROLS[2][4] = {{'w', 'a', 's', 'd'},                      // player 1
                      {KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT}};  // player 2

static const int MAX_BOARD_X = 50;
static const int MAX_BOARD_Y = 50;
static const double FRAMES_PER_SEC = 15;
static const int STARTING_LENGTH = 1;  // must be < MAX_BOARD_X - 1

struct Link {
  int x;
  int y;
  char symbol;
  int player;
};

// FORWARD DECLARATIONS - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Runs game
void run_game();

// Asks player whether they want to play again or exits game
bool play_again(int loser);

// Prints the board borders
void print_board();

// Checks whether this collides with anything
int check_collisions(const std::list<Link>& this_snake,
                      const std::list<Link>& other);

// Executes one game frame
void game_frame(std::list<Link>& player1, int& p1direction,
                std::list<Link>& player2, int& p2direction);

// Change direction of snake, can't change to opposite
bool get_dir(int& direction, int input, int player);

// Set initial direction at start of game
bool get_init_dir(int& direction, int input, int player);

// Increments snake position according to dircetion
void move_snek(std::list<Link>& snake_list, int direction);

// Prints current snake position
void print_snake(const std::list<Link>& snake_list);

// Prints an empty box to write prompts on
void print_prompt();

// Checks for user keyboard hit (req if getch is nonblocking)
bool kbhit();

// Init all the ncurses specific stuff
void init_ncurses();

// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main() {
  init_ncurses();

  run_game();

  endwin();
}

// FUNCTION IMPLEMENTATIONS - - - - - - - - - - - - - - - - - - - - - - - - - -

void run_game() {

  // Create player 1
  std::list<Link> player1;
  Link p1start = {5, MAX_BOARD_Y / 2, 'X', 1};
  player1.push_back(p1start);

  // Create player 2
  std::list<Link> player2;
  Link p2start = {MAX_BOARD_X - 5, MAX_BOARD_Y / 2, 'O', 2};
  player2.push_back(p2start);

  clear();
  print_board();
  print_snake(player1);
  print_snake(player2);

  // Wait for initial direction input
  int p1direction;
  int p2direction;
  bool p1sel = false;
  bool p2sel = false;
  while(!(p1sel && p2sel)) {
    if (kbhit()) {
      int input = getch();
      if (!p1sel) {
        p1sel = get_init_dir(p1direction, input, 1);
      }
      if (!p2sel) {
        p2sel = get_init_dir(p2direction, input, 2);
      }
    }
  }

  int loser = 0;
  while(loser == 0) {

    game_frame(player1, p1direction, player2, p2direction);
  }
  if (!play_again(loser)) {
    return;
  }
  else {
    run_game();
  }

  loser = check_collisions(player1, player2);
  if (loser == 0) {
    loser = check_collisions(player2, player1);
  }
}

bool play_again(int loser) {
  print_prompt();

  int halfx = MAX_BOARD_X;
  int halfy = MAX_BOARD_Y / 2;

  mvprintw(halfy - 1, halfx - 7, "Player %d Wins!", (loser % 2) + 1);
  mvprintw(halfy, halfx - 5, "Play again?");
  mvprintw(halfy + 1, halfx - 2, "(y/n)");

  while (1) {
    char input = 0;
    if (kbhit()) {
      input = getch();
    }

    if (input == 'y' || input == 'Y' || input == 'f') {
      return true;
    }
    if (input == 'n' || input == 'N' || input == 'b') {
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

int check_collisions(const std::list<Link>& this_snake,
                      const std::list<Link>& other) {
  // Only need to check head
  const Link head = this_snake.back();

  // Check self collisions
  std::list<Link>::const_iterator it = this_snake.begin();
  while (it != --this_snake.end()) {
    if (head.x == it->x && head.y == it->y) {
      return head.player;
    }
    ++it;
  }

  // Check other collisions
  std::list<Link>::const_iterator othit = other.begin();
  while (othit != other.end()) {
    if (head.x == othit->x && head.y == othit->y) {
      return head.player;
    }
    ++othit;
  }

  // Check wall check_collisions
  if (head.x == 0 || head.x == MAX_BOARD_X ||
      head.y == 0 || head.y == MAX_BOARD_Y) {
    return head.player;
  }
  return 0;
}

void game_frame(std::list<Link>& player1, int& p1direction,
                std::list<Link>& player2, int& p2direction) {
  // Wait and accept up to 1 direction change
  clock_t start = clock();
  bool p1changed = false;
  bool p2changed = false;
  while( ((clock()-start) / (double) CLOCKS_PER_SEC) < (1 / FRAMES_PER_SEC)) {
    if (kbhit()) {
      int input = getch();
      if (!p1changed) {
        p1changed = get_dir(p1direction, input, 1);
      }
      if (!p2changed) {
        p2changed = get_dir(p2direction, input, 2);
      }
    }
  }
  move_snek(player1, p1direction);
  move_snek(player2, p2direction);
  print_snake(player1);
  print_snake(player2);
}

bool get_dir(int& direction, int input, int player) {
  if (input == CONTROLS[player-1][0] && direction != 2) {
    direction = 0;
    return true;
  }
  if (input == CONTROLS[player-1][1] && direction != 1) {
    direction = 3;
    return true;
  }
  if (input == CONTROLS[player-1][2] && direction != 0) {
    direction = 2;
    return true;
  }
  if (input == CONTROLS[player-1][3] && direction != 3) {
    direction = 1;
    return true;
  }
  return false;
}

bool get_init_dir(int& direction, int input, int player) {
  if (input == CONTROLS[player-1][0]) {
    direction = 0;
    return true;
  }
  if (input == CONTROLS[player-1][1]) {
    direction = 3;
    return true;
  }
  if (input == CONTROLS[player-1][2]) {
    direction = 2;
    return true;
  }
  if (input == CONTROLS[player-1][3]) {
    direction = 1;
    return true;
  }
  return false;
}

void move_snek(std::list<Link>& snake_list, int direction) {
  Link Link = {snake_list.back().x, snake_list.back().y,
               snake_list.back().symbol, snake_list.back().player};

  if (direction == 3) {
    if (Link.x > 0) {
      --Link.x;
      snake_list.push_back(Link);
    }
    return;
  }
  else if (direction == 1) {
    if (Link.x < MAX_BOARD_X) {
      ++Link.x;
      snake_list.push_back(Link);
    }
    return;
  }
  else if (direction == 2) {
    if (Link.y < MAX_BOARD_Y) {
      ++Link.y;
      snake_list.push_back(Link);
    }
    return;
  }
  else if (direction == 0) {
    if (Link.y > 0) {
      --Link.y;
      snake_list.push_back(Link);
    }
    return;
  }
}

void print_snake(const std::list<Link>& snake_list) {
  for (std::list<Link>::const_iterator it = snake_list.begin();
       it != snake_list.end();
       ++it) {
    mvprintw(it->y, 2 * it->x, "%c", it->symbol);
  }

  // Link
  // mvprintw(MAX_BOARD_Y + 1, 0, "%d,%d\r",snake_list.back().x, snake_list.back().y);
  refresh();
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
