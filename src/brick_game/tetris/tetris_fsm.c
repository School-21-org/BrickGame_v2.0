#include "tetris_fsm.h"

GameState_t game_state = START;
g_tetromino_t block = {0};
static long long prev_time = 0;

typedef void (*action)();

action tetris_fsm_table[8][8] = {
    {spawn, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {NULL, pause_game, NULL, NULL, NULL, NULL, NULL, NULL},
    {NULL, pause_game, NULL, moveleft, moveright, NULL, movedown, rotaite},
    {NULL, pause_game, NULL, moveleft, moveright, NULL, movedown, rotaite},
    {spawn, pause_game, spawn, spawn, spawn, spawn, spawn, spawn},
    {spawn, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    {terminate, terminate, terminate, terminate, terminate, terminate,
     terminate, terminate},
    {timer_move, NULL, NULL, NULL, NULL, NULL, NULL, NULL}};

/**
 * @brief Calls function from the condition matrix, based on user input and
 * game state
 */
void updateCurrentState() {
  if (!game_info.pause) {
    if (game_state == GAMEOVER) {
      reset_game_info();
      game_state = START;
    } else if (game_state != ATTACHING)
      timer_move();

    action act = tetris_fsm_table[game_state][user_action];
    if (act) act();

    if (game_state == GAMEOVER) gameover();
    if (!game_info.pause) user_action = Start;
    if (game_info.pause && user_action == Start) user_action = Action;
  }
}

/**
 * @brief Spawn figure
 */
void spawn() {
  if (game_state == GAMEOVER) {
    reset_game_info();
    game_state = START;
  }
  block.current = game_state == START ? I_ : block.next;
  game_state = SPAWN;
  block.next = select_figure();
  fill_next();
  block.position = pos_1;
  block.initial_y = 0;
  block.initial_x = 3;
  draw_block(MV_down);
}

/**
 * @brief Auto move in case time condition satisfies
 */
void timer_move() {
  if (!prev_time) prev_time = current_timestamp();
  long long current_time = current_timestamp();
  if (current_time - prev_time > game_info.speed) {
    movedown();
    prev_time = current_time;
  }
}

/**
 * @brief Moves a block down
 */
void movedown() {
  erase_block();
  block.initial_y += 1;
  draw_block(MV_down);
  if (game_state != MOVING) {
    block.initial_y -= 1;
    fill_block(FILL_MODE, MV_down);
    if (game_state == ATTACHING) check_win_line();
    if (game_state == SHIFTING) game_state = MOVING;
  }
}

/**
 * @brief Moves a block to the right
 */
void moveright() {
  erase_block();
  block.initial_x += 1;
  draw_block(MV_aside);
  if (game_state != MOVING) {
    block.initial_x -= 1;
    fill_block(FILL_MODE, MV_down);
    if (game_state == SHIFTING) game_state = MOVING;
  }
}

/**
 * @brief Moves a block to the left
 */
void moveleft() {
  erase_block();
  block.initial_x -= 1;
  draw_block(MV_aside);
  if (game_state != MOVING) {
    block.initial_x += 1;
    fill_block(FILL_MODE, MV_down);
    if (game_state == SHIFTING) game_state = MOVING;
  }
}

/**
 * @brief Rotaites figure
 */
void rotaite() {
  erase_block();
  if (block.position == pos_4)
    block.position = pos_1;
  else
    block.position += 1;
  draw_block(MV_rotaite);
  if (game_state != MOVING) {
    if (block.position == pos_1)
      block.position = pos_4;
    else
      block.position -= 1;
    fill_block(FILL_MODE, MV_down);
    if (game_state == SHIFTING) game_state = MOVING;
  }
}

/**
 * @brief Set game over state
 */
void gameover() {
  fill_go_field();
  game_info.pause = (int)GAME_OVER;
}

/**
 * @brief Sets pause state
 */
void pause_game() {
  game_info.pause = (int)PAUSE;
  game_state = PAUSE_STATE;
}

/**
 * @brief Emergency exit in case of any errors
 */
void terminate() { user_action = Terminate; }
