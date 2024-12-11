#ifndef __TETRIS_CONTR_H__
#define __TETRIS_CONTR_H__

#include <pthread.h>
#include <stdatomic.h>

#include "tetris_fsm.h"

void *tetris_back_thread_loop();

#endif