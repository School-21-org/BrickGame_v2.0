#ifndef __FSM_HPP__
#define __FSM_HPP__

#include "../common/common_objects.h"
#include "../common/defines.h"
#include "objects.hpp"

typedef void (*action)();

typedef enum {
  START = 0,
  SPAWN,
  MOVE,
  BUMP,
  GAMEOVER,
  WIN_STATE,
  PAUSE_STATE
} GameState_s;

void updateCurrentState();

void spawn();
void turn();
void check_length();
void fill_game_info();
void game_over();
void timer_move();
void auto_move();
void shift_move();
void move_helper(bool lucky_move);
void set_pause();

#endif