#include "tic.h"

clock_t frame_timer;

void init_ncurses() {
  srand(time(NULL));
  initscr();
  curs_set(0);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);  // getch can't block clock
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
}

void print_board_frame(int yoffset, int xoffset) {
  mvprintw(0 + yoffset, 3 + xoffset, "|");
  mvprintw(0 + yoffset, 7 + xoffset, "|");
  mvprintw(1 + yoffset, 0 + xoffset, "---|---|---");
  mvprintw(2 + yoffset, 3 + xoffset, "|");
  mvprintw(2 + yoffset, 7 + xoffset, "|");
  mvprintw(3 + yoffset, 0 + xoffset, "---|---|---");
  mvprintw(4 + yoffset, 3 + xoffset, "|");
  mvprintw(4 + yoffset, 7 + xoffset, "|");
  refresh();
}

void print_board_frame() {
  print_board_frame(0, 0);
}

void print_board(const Board& board, int yoffset, int xoffset) {
  // Print each spot in correct position
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      char toprint = get_board_char(board, row, col);
      mvprintw(2 * row + yoffset, 4 * col + 1 + xoffset, "%c", toprint);
    }
  }
  refresh();
}

void print_board(const Board& board) {
  print_board(board, 0, 0);
}

ButtonPress poll_for_btn(CursorPos& cursor) {
  int input = 0;
  if (kbhit()) {
    input = getch();
  }

  switch (input) {

    case KEY_LEFT:
      if (cursor.col > 0) {
        --cursor.col;
      }
      return ARROW_PRESS;

    case KEY_RIGHT:
      if (cursor.col < NUM_COLS - 1) {
        ++cursor.col;
      }
      return ARROW_PRESS;

    case KEY_UP:
      if (cursor.row > 0) {
        --cursor.row;
      }
      return ARROW_PRESS;

    case KEY_DOWN:
      if (cursor.row < NUM_ROWS - 1) {
        ++cursor.row;
      }
      return ARROW_PRESS;

    case '\n':
      return ENTER_PRESS;

    case 'N':
    case 'n':
      return N_PRESS;

    case 'Y':
    case 'y':
      return Y_PRESS;

    case 'q':
      return Q_PRESS;

    default:
      return NO_PRESS;
  }
}

ButtonPress poll_for_btn() {
  int input = 0;
  if (kbhit()) {
    input = getch();
  }

  switch (input) {
    case '\n':
      return ENTER_PRESS;

    case 'N':
    case 'n':
      return N_PRESS;

    case 'Y':
    case 'y':
      return Y_PRESS;

    case 'q':
      return Q_PRESS;

    default:
      return NO_PRESS;
  }
}

GameState check_for_game_done(const Board& board, int row, int col) {
  // Check if last player won
  bool won = false;
  Piece placed = board.boardstate[row][col];

  // Check vertical win
  if (board.boardstate[(row+1) % NUM_ROWS][col] == placed &&
      board.boardstate[(row+2) % NUM_ROWS][col] == placed) {
    won = true;
  }
  // Check horiz win
  if (board.boardstate[row][(col+1) % NUM_COLS] == placed &&
      board.boardstate[row][(col+2) % NUM_COLS] == placed) {
    won = true;
  }

  // Check diags if row+col is even
  if ((row + col) % 2 == 0) {
    // Check diag
    if (row == col) {
      if (board.boardstate[(row+1) % NUM_ROWS][(col+1) % NUM_COLS] == placed &&
          board.boardstate[(row+2) % NUM_ROWS][(col+2) % NUM_COLS] == placed) {
        won = true;
      }
    }

    // Check antidiag
    if (row != col || (row == 1 && col == 1)) {
      if (board.boardstate[(row+1) % NUM_ROWS][(col+2) % NUM_COLS] == placed &&
          board.boardstate[(row+2) % NUM_ROWS][(col+1) % NUM_COLS] == placed) {
        won = true;
      }
    }

  }

  if (won) {
    return (board.current_player == PLAYER_ONE) ? P2WIN : P1WIN;
  }

  // Check if board is full
  bool open_spaces_exist = false;
  for (int row = 0; row < NUM_ROWS; ++row) {
    for (int col = 0; col < NUM_COLS; ++col) {
      if (board.boardstate[row][col] == NONE) {
        open_spaces_exist = true;
        break;
      }
    }
  }
  if (!open_spaces_exist) {
    return DRAW;
  }

  return NO_END;
}

EndingState handle_end(GameState gamestate) {
  mvprintw(0, 15, "                        ");
  if (gamestate == DRAW) {
    mvprintw(0, 15, "DRAW");
  }
  if (gamestate == P1WIN) {
    mvprintw(0, 15, "P1 (X) WINS!");
  }
  if (gamestate == P2WIN) {
    mvprintw(0, 15, "P2 (O) WINS!");
  }
  if (gamestate == DRAW || gamestate == P1WIN || gamestate == P2WIN) {
    mvprintw(1, 15, "Play Again? (y/n)");
    refresh();
    while (1) {
      ButtonPress ans = poll_for_btn();
      if (ans == N_PRESS) {
        return END;
      }
      else if (ans == Y_PRESS) {
        return REPLAY;
      }
      mvprintw(2, 15, "%c", ans);
    }
  }
  return CONTINUE;
}

// Helper for solve_best_move
static Move solve_best_move_helper(const Board& board, bool print_thoughts) {
  // Initialize best result with a loss
  Move best_move = {(board.current_player == PLAYER_ONE) ? -1 : 1, 0, -1, -1};

  int mistake_count = 0;


  int yoff = 8;
  int xoff = 0;

  int yoff2 = 16;
  int xoff2 = 0;
  if (print_thoughts) {
    // For printing primary eval
    mvprintw(yoff - 2, xoff, "Primary eval:\n1=XWIN 0=DRAW -1=OWIN");
    print_board_frame(yoff, xoff);

    // For printing secondary eval
    mvprintw(yoff2 - 2, xoff, "Secondary eval:\n#=wrong moves possible");
    print_board_frame(yoff2, xoff2);
  }

  // Iterate through all possible moves
  for (int row = 0; row < NUM_ROWS; ++row) {
    for (int col = 0; col < NUM_COLS; ++col) {
      if (board.boardstate[row][col] == NONE) {
        // Can move here, create a copy of board after this move is made
        Board boardcpy = board;
        boardcpy.boardstate[row][col] =
            (board.current_player == PLAYER_ONE) ? X_S : O_S;
        boardcpy.current_player ^= 1;

        // Check if this move would end the game !! BASE CASE !!
        Move move = {0, 10, row, col};
        GameState gs = check_for_game_done(boardcpy, row, col);
        if (gs == DRAW) {
          move.result = 0;
        }
        else if (gs == P1WIN) {
          move.result = 1;
        }
        else if (gs == P2WIN) {
          move.result = -1;
        }

        // Recursive case
        else {
          move = solve_best_move_helper(boardcpy, false);
          move.row = row;
          move.col = col;
        }

        // Mark as best move if better than prev best move (tiebreak by mistakes)
        // If move is wrong, increment num mistakes
        if (board.current_player == PLAYER_ONE) {
          if (move.result == -1) {
            ++mistake_count;
          }
          best_move = std::max(move, best_move,
            [](const Move& l, const Move& r) {
              if (l.result != r.result) {
                return l.result < r.result;
              }
              return l.num_mistakes < r.num_mistakes;
            });
        }
        else {
          if (move.result == 1) {
            ++mistake_count;
          }
          best_move = std::min(move, best_move,
            [](const Move& l, const Move& r) {
              if (l.result != r.result) {
                return l.result < r.result;
              }
              return r.num_mistakes < l.num_mistakes;
            });
        }


        if (print_thoughts) {
          // Print primary evaluation
          if (move.result > -1) {
            mvprintw(2 * row + yoff, 4 * col + 1 + xoff, "%d ", move.result);
          }
          else {
            mvprintw(2 * row + yoff, 4 * col + 1 + xoff, "%d", move.result);
          }

          // Print secondary eval
          mvprintw(2 * row + yoff2, 4 * col + 1 + xoff2, "%d", move.num_mistakes);
        }

      }
      else {
        if (print_thoughts) {
          // Print space
          mvprintw(2 * row + yoff, 4 * col + 1 + xoff, "  ");
          mvprintw(2 * row + yoff2, 4 * col + 1 + xoff2, "  ");
        }
      }

      if (print_thoughts) {
        refresh();
      }

    } // for
  } // for

  best_move.num_mistakes = mistake_count;
  return best_move;
}


Move solve_best_move(const Board& board, bool show_thinking) {
  Move move = solve_best_move_helper(board, show_thinking);
  if (show_thinking) {
    mvprintw(10, 20, "Selected move: (%d,%d)", move.row, move.col);
    refresh();
  }
  return move;
}



// HELPERS - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

void wait_until_s(clock_t timer, double secs) {
  while (((clock() - timer) / (double) CLOCKS_PER_SEC) < secs);
}

double get_frame_time_s() {
  return ((clock() - frame_timer) / (double) CLOCKS_PER_SEC);
}

void reset_frame_timer() {
  frame_timer = clock();
}

char get_board_char(const Board& board, int row, int col) {
  int spot = board.boardstate[row][col];
  return spot == NONE ? ' ' : (spot == X_S) ? 'X' : 'O';
}

void print_player_turn(int current_player) {
  mvprintw(0, 15, (current_player == PLAYER_ONE) ?
                  "Player 1's turn (X)" : "Player 2's turn (O)");
  refresh();
}
