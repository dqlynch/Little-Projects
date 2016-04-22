// Implementation of Game.h for terminal game-general functions

#include "Game.h"

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
