// Header for snake specific functions
#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <iostream>
#include <list>
#include <ncurses.h>
#include <ctime>
#include <string>
#include <stdlib.h>
#include "Screen.h"

class GameVars {
public:
  static double FRAMES_PER_SEC;
  static int STARTING_LENGTH;
};

struct SnakeInfo {
  std::list<Coords> list;
  int direction;
  Coords fruit;
  std::string fruit_word;
  int fruit_word_index;
};

// Checks whether snek has collided into anything
bool check_collisions(const SnakeInfo& snake);

// Checks whether object collides with any of snake EXCEPT HEAD
bool check_collision_with_snake(const SnakeInfo& snake, const Coords& object);

// Executes one game frame
void game_frame(SnakeInfo& snake);

// Asks player whether they want to play again or exits game
bool play_again(SnakeInfo snake);

// Prints the a prompt asking if they want to play again
void print_play_again_prompt();

// Change direction of snake, can't change to opposite
bool get_dir(SnakeInfo& snake);

// Creates a snake based on screen size
std::list<Coords> create_snake();

// Sets last 5 characters of snake + eraser on the tail
void set_name(SnakeInfo& snake);

// Increments snake position according to dircetion
void move_snek(SnakeInfo& snake);

// Prints current snake position
void print_snake(const SnakeInfo& snake);

// Generates a fruit at random location that snake doesn't currently occupy
void generate_fruit(SnakeInfo& snake);

// Cycles back to index 0 if end of word
void increment_fruit_word(SnakeInfo& snake);

// Checks whether or not snake found a fruit
bool check_fruit_get(SnakeInfo& snake);

// Reprints board with any changed size or positions
void update_board(const SnakeInfo& snake);

#endif
