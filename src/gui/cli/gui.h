#ifndef GUI_H
#define GUI_H
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "../../brick_game/common/common_objects.h"
#include "../../brick_game/common/defines.h"
#include "../../brick_game/snake/controller_gui.hpp"
#include "../../brick_game/tetris/tetris_contr_gui.h"

typedef enum {
  Tetris = 1,
  Snake = 2,
} t_selector;

extern t_selector game_selector;

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
    start_color();        \
    init_pair(1, 0, 0);   \
    init_pair(2, 7, 7);   \
    init_pair(3, 0, 7);   \
    init_pair(4, 0, 2);   \
    init_pair(5, 0, 1);   \
    init_pair(5, 0, 1);   \
    init_pair(6, 1, 7);   \
    init_pair(7, 2, 7);   \
    bkgd(COLOR_PAIR(3));  \
  }

#define GET_USER_INPUT getch()

#define BOARDS_BEGIN 2

#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c);
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__);

void gui_loop();
void start();
void pause_state();
void gameover_state();
void get_user_action(int user_input);
void win_state();
void pause_loop();

void print_window(int start);
void print_overlay(int start);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_playground();
void print_stats();
void print_gameover();
void print_pause();
void print_win();
void print_next_figure();

void draw_tetris(int start);
void draw_snake(int start);

int get_cwd();

#endif