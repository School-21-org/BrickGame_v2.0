#ifndef __TETRIS_FSM_H__
#define __TETRIS_FSM_H__

#include "tetris_additionals.h"

void updateCurrentState();
void timer_move();
void spawn();
void movedown();
void moveright();
void moveleft();
void rotaite();
void gameover();
void terminate();
void pause_game();

#endif