#include <iostream>

#include "tic.h"

using namespace std;

bool run_game();

int main(int argc, char* argv[]) {
  init_ncurses();

  while (run_game());

  endwin();
}

bool run_game() {
  clear();
  frame_timer = clock();

  bool ai_p1 = true;
  bool ai_p2 = true;
  bool show_thinking = false;

  ButtonPress ans;
  mvprintw(0, 0, "Player 1 human? (y/n)");
  refresh();
  while (1) {
    ans = poll_for_btn();
    if (ans == Y_PRESS) {
      ai_p1 = false;
      break;
    }
    else if (ans == N_PRESS) {
      ai_p1 = true;
      break;
    }
  }

  mvprintw(0, 0, "Player 2 human? (y/n)");
  refresh();
  while(1) {
    ans = poll_for_btn();
    if (ans == Y_PRESS) {
      ai_p2 = false;
      break;
    }
    else if (ans == N_PRESS) {
      ai_p2 = true;
      break;
    }
  }

  if (ai_p1 || ai_p2) {
    mvprintw(0, 0, "Show computer thinking? (y/n)");
    refresh();
    while(1) {
      ans = poll_for_btn();
      if (ans == Y_PRESS) {
        show_thinking = true;
        break;
      }
      else if (ans == N_PRESS) {
        show_thinking = false;
        break;
        break;
      }
    }
  }
  clear();


  Board board;
  CursorPos cursor;
  print_board_frame();
  print_board(board);

  bool cursor_shown = false;

  print_player_turn(board.current_player);
  refresh();

  // Main loop
  while(1) {

    // Poll for keyboard entries, main game logic
    while (get_frame_time_s() < SECS_PER_FRAME) {

      if ((ai_p2 && board.current_player == PLAYER_TWO) ||
          (ai_p1 && board.current_player == PLAYER_ONE)) {

        clock_t wait_time = clock();      // time cpu move

        Move move = solve_best_move(board, show_thinking);

        board.boardstate[move.row][move.col] =
            (board.current_player == PLAYER_ONE) ? X_S : O_S;
        board.current_player ^= 1;

        // Give user time to see
        wait_until_s(wait_time, 0.5);

        print_player_turn(board.current_player);
        print_board(board);

        GameState gs = check_for_game_done(board, move.row, move.col);
        EndingState es = handle_end(gs);
        if (es == REPLAY) {
          return true;
        }
        if (es == END) {
          return false;
        }
      }
      else {
        if (ButtonPress press = poll_for_btn(cursor)) {
          // Cursor shown if directional input
          cursor_shown = false;

          if (press == Q_PRESS) {
            return false;
          }

          if (press == ENTER_PRESS) {
            // Place piece if unoccupied
            if (board.boardstate[cursor.row][cursor.col] == NONE) {
              board.boardstate[cursor.row][cursor.col] =
                (board.current_player == PLAYER_ONE) ? X_S : O_S;
              board.current_player ^= 1;

              GameState gs = check_for_game_done(board, cursor.row, cursor.col);
              print_board(board);
              EndingState es = handle_end(gs);
              if (es == REPLAY) {
                return true;
              }
              if (es == END) {
                return false;
              }

              print_player_turn(board.current_player);
              cursor_shown = true;  // Cursor hidden if placing piece
            }
          }

          print_board(board);
          break;                    // Reset frame timer
        }
      }


    }


    // BLINK CURSOR
    cursor_shown ^= 1;
    char ch = cursor_shown ?
              cursor.ch :
              get_board_char(board, cursor.row, cursor.col);
    mvprintw(2 * cursor.row, 4 * cursor.col + 1, "%c", ch);


    reset_frame_timer();
    refresh();
  }

}
