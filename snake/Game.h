// Contains general game functions and constants for terminal games
// like snake and bmtron
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/ioctl.h>  // for get window size

class GameVars {
public:
  static int MAX_BOARD_Y;
  static int MAX_BOARD_X;
  static double FRAMES_PER_SEC;
  static int STARTING_LENGTH;

private:
  // Private default constructor to prevent any creation of instances
  GameVars() {}
};

struct Coords {
  int x;
  int y;
  char symbol;
};

// Init all the ncurses specific stuff
void init_ncurses();

// Sets the window size and board size, return true if changed
bool set_game_size();

// Checks for user keyboard hit (req if getch is nonblocking)
bool kbhit();

// Print prompt
void print_prompt();

#endif
