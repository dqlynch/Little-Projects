#ifndef TIC_H
#define TIC_H

#include <ncurses.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cassert>

// Global constants
enum Piece {NONE, X_S, O_S};
enum Player {PLAYER_ONE, PLAYER_TWO};
enum ButtonPress {NO_PRESS, ARROW_PRESS, ENTER_PRESS, Y_PRESS, N_PRESS, Q_PRESS};
enum GameState {NO_END, DRAW, P1WIN, P2WIN};

const int NUM_ROWS = 3;
const int NUM_COLS = 3;

const double FRAMES_PER_SEC = 4;
const double SECS_PER_FRAME = 1 / FRAMES_PER_SEC;

// Global frame timer
extern clock_t frame_timer;

// Board struct
struct Board {
  Piece boardstate[NUM_ROWS][NUM_COLS] = {{NONE}};

  int current_player = PLAYER_ONE;
};

struct CursorPos {
  char ch = '_';
  int row = 0;
  int col = 0;
};

// Initializes ncurses
void init_ncurses();

// Prints the board frame with nothing in it, starting from offset (default 0)
void print_board_frame(int yoffset, int xoffset);
void print_board_frame();

// Prints board contents with offset (default 0)
void print_board(const Board& board, int yoffset, int xoffset);
void print_board(const Board& board);

// Moves cursor according to arrow keys, also listens for enter.
// Returns true if a key entry is made, 1 is a directional entry, 2 is an enter
ButtonPress poll_for_btn(CursorPos& cursor);
ButtonPress poll_for_btn();

// Checks if either player has won or if the board is full if the last move
// is row, col
// REQUIRES: row, col is set, and player has been switched since last move
//           (checks after the move is made)
GameState check_for_game_done(const Board& board, int row, int col);

enum EndingState {CONTINUE, REPLAY, END};
EndingState handle_end(GameState gamestate);

// Return type for solve best move algorithm
struct Move {
  // The game result if other player plays perfectly and this move is made
  int result;

  // Number of mistakes that could be made INSTEAD of this move (same level)
  // Used as a tiebreaker for moves w/ same result when played optimally
  int num_mistakes;

  // Coords
  int row;
  int col;

  // bool operator<(const Move& other) const {
  //   return result < other.result;
  // }
};

// // Helper for solve_best_move
// static Move solve_best_move_helper(const Board& board);

// Solves for the best ai cpu move
// Returns a move and modifies the board for the move made
Move solve_best_move(const Board& board, bool show_thinking);


// WRAPPERS AND HELPERS - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Wrapper for kbhit
bool kbhit();

// Waits until the timer has been running for secs (not a sleep)
void wait_until_s(clock_t timer, double secs);

// Returns the time in seconds since the last time frame_timer was reset
double get_frame_time_s();

// Resets the frame timer
void reset_frame_timer();

// Returns the correct character for the (row,col) position on board
char get_board_char(const Board& board, int row, int col);

void print_player_turn(int current_player);

#endif
