#include "gui.h"

char PATH[500] = {'\0'};

t_selector game_selector = Tetris;
GameInfo_t game_info = {0, 0, 0, 0, 0, 0, 0};
UserAction_t user_action = Action;

typedef void (*selection)(int start);

selection menu_items[] = {draw_tetris, draw_snake};

/**
 * @brief Starts main gui loop, repeatedly scans for user input and refreshes
 * the terminal
 */
void gui_loop() {
  start();

  if (user_action == Start)
    user_action = get_cwd() == SUCCESS ? Start : Terminate;

  if (user_action == Start) {
    if (game_selector == Snake)
      launch_snake_game();
    else if (game_selector == Tetris)
      launch_tetris_game();
  }

  while (user_action != Terminate) {
    print_window(0);
    get_user_action(GET_USER_INPUT);
    if (game_info.pause == (int)PAUSE) pause_state();
    if (game_info.pause == (int)GAME_OVER) gameover_state();
    if (game_info.pause == (int)WIN) win_state();
    if (user_action == Terminate) {
      if (game_selector == Snake)
        detach_snake_game();
      else if (game_selector == Tetris)
        detach_tetris_game();
    }
  }
}

/**
 * @brief Gets current working directory
 * @return error code (-1 in case of error)
 */
int get_cwd() {
  int error = SUCCESS;
  if (getcwd(PATH, sizeof(PATH)) == NULL) error = ERROR;
  if (error == SUCCESS) strcat(PATH, "/");
  return error;
}

/**
 * @brief Transforms raw user input into static user action
 */
void get_user_action(int user_input) {
  if (user_input == KEY_DOWN)
    user_action = Down;
  else if (user_input == KEY_UP)
    user_action = Up;
  else if (user_input == KEY_LEFT)
    user_action = Left;
  else if (user_input == KEY_RIGHT)
    user_action = Right;
  else if (user_input == ESCAPE)
    user_action = Terminate;
  else if (user_input == ENTER_KEY)
    user_action = Start;
  else if (user_input == SPACE_KEY)
    user_action = Action;
  else if (user_input == PAUSE_KEY_L || user_input == PAUSE_KEY_U)
    user_action = Pause;
}

/**
 * @brief Game selector, loop until user choses a game
 */
void select_game() {
  int num_items = sizeof(menu_items) / sizeof(menu_items[0]);
  int current_item = 0, previous_item = -1;
  while (user_action != Start && user_action != Terminate) {
    if (current_item != previous_item) {
      clear();
      for (int i = 0; i < num_items; ++i) {
        if (i == current_item) attron(COLOR_PAIR(4));
        selection select = menu_items[i];
        select(1);
        if (i == current_item) attroff(COLOR_PAIR(4));
      }
      refresh();
      previous_item = current_item;
    }

    int ch = getch();
    if (ch == KEY_UP)
      current_item = (current_item == 0) ? num_items - 1 : current_item - 1;
    else if (ch == KEY_DOWN)
      current_item = (current_item == num_items - 1) ? 0 : current_item + 1;
    else if (ch == ENTER_KEY || ch == ESCAPE) {
      if (current_item == 0)
        game_selector = Tetris;
      else if (current_item == 1)
        game_selector = Snake;
      get_user_action(ch);
    }
  }
  usleep(10000);
}

/**
 * @brief Start menu loop, waits until user input
 */
void start() {
  select_game();
  if (user_action == Start) {
    print_window(1);
    int signal = 0;
    while (signal != ENTER_KEY && signal != ESCAPE) {
      signal = GET_USER_INPUT;
    }
    get_user_action(signal);
  }
}

/**
 * @brief Pause menu loop, waits until user input
 */
void pause_state() {
  print_window(0);
  print_pause();
  pause_loop();
}

/**
 * @brief Game over menu loop, waits until user input
 */
void gameover_state() {
  print_window(0);
  print_gameover();
  pause_loop();
}

/**
 * @brief Win menu loop, waits until user input
 */
void win_state() {
  print_window(0);
  print_win();
  pause_loop();
}

/**
 * @brief Pause menu loop, waits until user input
 */
void pause_loop() {
  while (game_info.pause) {
    int signal = GET_USER_INPUT;
    if (signal == ENTER_KEY || signal == ESCAPE) {
      game_info.pause = (int)GAME;
      get_user_action(signal);
    }
  }
}
