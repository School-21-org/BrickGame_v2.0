#ifndef _TETRIS_OBJECTS_H__
#define _TETRIS_OBJECTS_H__

#define CHECK_MODE 1
#define FILL_MODE 0
#define BREAK -1
#define MAX_BLOCK_REPEATS 3
#define COUNT_BLOCK_TYPES 7

typedef enum { MV_down = 0, MV_aside, MV_rotaite } fsm_action_t;

typedef enum { I_ = 0, J_, L_, O_, S_, T_, Z_ } type_tetromino;

typedef struct count_blocks {
  int i_, j_, l_, o_, s_, t_, z_;
  int total;
} t_count_blocks;

typedef enum { pos_1 = 0, pos_2, pos_3, pos_4 } t_pos;

typedef struct g_tetromino {
  type_tetromino current, next;
  t_pos position;
  int initial_x;
  int initial_y;
} g_tetromino_t;

extern g_tetromino_t block;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  EXIT_STATE,
  PAUSE_STATE
} GameState_t;

extern GameState_t game_state;

#endif