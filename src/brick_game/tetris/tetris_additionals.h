#ifndef _TETRIS_ADDITIONALS_H__
#define _TETRIS_ADDITIONALS_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "../common/common_objects.h"
#include "../common/defines.h"
#include "tetris_objects.h"

void tet_game_info_init();
int field_memory_allocation();
void mem_free();

type_tetromino select_figure();
int block_selector_check(type_tetromino block);
void check_showed_blocks_limit();

long long current_timestamp();

int get_bit(int num, int position);
void draw_block(fsm_action_t fsm_action);
void erase_block();
void fill_block(int check, fsm_action_t fsm_action);
void fill_next();

void add_score(int win_lines);
void check_win_line();
void up_level();

void move_field_down(int row);

int read_max_score();
void write_max_score();

void clear_field();
void reset_game_info();
void fill_go_field();

#endif