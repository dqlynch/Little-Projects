// Header for snake specific functions
#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <iostream>
#include <list>
#include <ncurses.h>
#include <ctime>
#include "Screen.h"

class GameVars {
public:
  static double FRAMES_PER_SEC;
  static int STARTING_LENGTH;
};

// Checks whether snek has collided into anything
bool check_collisions(const std::list<Coords>& snake_list);

// Executes one game frame
void game_frame(std::list<Coords>& snake_list, int& direction);

// Asks player whether they want to play again or exits game
bool play_again();

// Change direction of snake, can't change to opposite
bool get_dir(int& direction);

// Increments snake position according to dircetion
void move_snek(std::list<Coords>& snake_list, int direction);

// Prints current snake position
void print_snake(const std::list<Coords>& snake_list);

// Sets last 5 characters of snake + eraser on the tail
void set_name(std::list<Coords>& snake_list);

#endif
