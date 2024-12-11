#include "tetris_additionals.h"

static t_count_blocks showed_blocks = {0};

int bit_figure[][4] = {
    {0b0000111100000000, 0b0010001000100010, 0b0000000011110000,
     0b0100010001000100},  // I
    {0b1000111000000000, 0b0110010001000000, 0b0000111000100000,
     0b0100010011000000},  // J
    {0b0010111000000000, 0b0100010001100000, 0b0000111010000000,
     0b1100010001000000},  // L
    {0b0110011000000000, 0b0110011000000000, 0b0110011000000000,
     0b0110011000000000},  // O
    {0b0110110000000000, 0b0100011000100000, 0b0000011011000000,
     0b1000110001000000},  // S
    {0b0100111000000000, 0b0100011001000000, 0b0000111001000000,
     0b0100110001000000},  // T
    {0b1100011000000000, 0b0010011001000000, 0b0000110001100000,
     0b0100110010000000}  // Z
};

/**
 * @brief Initialize game_info structure
 */
void tet_game_info_init() {
  strcat(PATH, "tetris_max_score.txt");
  int error = SUCCESS;
  game_info.level = 1;
  game_info.score = 0;
  game_info.high_score = read_max_score();
  if (game_info.high_score == ERROR) error = ERROR;
  game_info.speed = DEFAULT_SPEED;
  game_info.pause = (int)GAME;
  if (!error && !game_info.field) error = field_memory_allocation();
  if (error == ERROR) game_state = EXIT_STATE;
}

/**
 * @brief Frees memory for matrix fields
 */
void mem_free() {
  if (game_info.field) {
    for (int i = 0; i < HEIGHT; ++i) {
      free(game_info.field[i]);
    }
    free(game_info.field);
  }
  if (game_info.next) {
    for (int i = 0; i < 4; ++i) {
      free(game_info.next[i]);
    }
    free(game_info.next);
  }
}

/**
 * @brief Allocates memory for matrix fields
 * @returns -1 if allocation failed, 0 in case of success
 */
int field_memory_allocation() {
  int mem_error = SUCCESS;
  game_info.field = calloc(HEIGHT, sizeof(int *));
  if (game_info.field) {
    for (int i = 0; i < HEIGHT && mem_error == SUCCESS; ++i) {
      game_info.field[i] = calloc(WIDTH, sizeof(int));
      if (!game_info.field[i]) mem_error = ERROR;
    }
  } else {
    mem_error = ERROR;
  }
  if (!mem_error) {
    game_info.next = calloc(4, sizeof(int *));
    if (game_info.next) {
      for (int i = 0; i < 4 && mem_error == SUCCESS; ++i) {
        game_info.next[i] = calloc(WIDTH, sizeof(int));
        if (!game_info.next[i]) mem_error = ERROR;
      }
    } else {
      mem_error = ERROR;
    }
  }
  return mem_error;
}

/**
 * @brief Calculates time in miliseconds
 * @returns current miliseconds of the day
 */
long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL);
  long long miliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  return miliseconds;
}

/**
 * @brief Randomly selects the next figure
 * @returns next figure type
 */
type_tetromino select_figure() {
  srand(time(NULL));
  int n = 0;
  n = rand() % 7;
  type_tetromino new_block = n;
  while (block_selector_check(new_block)) {
    n = rand() % 7;
    new_block = n;
  }
  return new_block;
}

/**
 * @brief Randomly selects the next figure
 * @param new_block a block that was randomly selected
 * @returns 0 in case it is possible to use block, else 1
 */
int block_selector_check(type_tetromino new_block) {
  int code = SUCCESS;
  if (new_block == block.current)
    code = CHECK_MODE;
  else if (new_block == I_) {
    if (showed_blocks.i_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.i_;
    else
      code = CHECK_MODE;
  } else if (new_block == J_) {
    if (showed_blocks.j_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.j_;
    else
      code = CHECK_MODE;
  } else if (new_block == L_) {
    if (showed_blocks.l_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.l_;
    else
      code = CHECK_MODE;
  } else if (new_block == O_) {
    if (showed_blocks.o_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.o_;
    else
      code = CHECK_MODE;
  } else if (new_block == S_) {
    if (showed_blocks.s_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.s_;
    else
      code = CHECK_MODE;
  } else if (new_block == T_) {
    if (showed_blocks.t_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.t_;
    else
      code = CHECK_MODE;
  } else if (new_block == Z_) {
    if (showed_blocks.z_ < MAX_BLOCK_REPEATS)
      ++showed_blocks.z_;
    else
      code = CHECK_MODE;
  }
  if (code == SUCCESS) ++showed_blocks.total;
  check_showed_blocks_limit();
  return code;
}

/**
 * @brief Resets showed_block structure in case it reached a MAX_BLOCK_REPEATS
 * limit
 */
void check_showed_blocks_limit() {
  if (showed_blocks.total == COUNT_BLOCK_TYPES * MAX_BLOCK_REPEATS) {
    showed_blocks.i_ = 0;
    showed_blocks.j_ = 0;
    showed_blocks.l_ = 0;
    showed_blocks.o_ = 0;
    showed_blocks.s_ = 0;
    showed_blocks.t_ = 0;
    showed_blocks.z_ = 0;
    showed_blocks.total = 0;
  }
}

/**
 * @brief Draws block to the field if possible, else changes game_state
 * @param fsm_action the action that called this function
 */
void draw_block(fsm_action_t fsm_action) {
  fill_block(CHECK_MODE, fsm_action);
  if (game_state != GAMEOVER && game_state != ATTACHING &&
      game_state != SHIFTING) {
    fill_block(FILL_MODE, fsm_action);
    game_state = MOVING;
  }
}

/**
 * @brief Checks if it possible to draw block if param check = 1,
 * else fills field with new block
 * @param check triggers check mode if 1, else fill the field
 * @param fsm_action the action that called this function
 */
void fill_block(int check, fsm_action_t fsm_action) {
  int pos = 15;
  for (int i = block.initial_y; i < (block.initial_y + 4) && check != BREAK;
       ++i) {
    for (int j = block.initial_x; j < (block.initial_x + 4) && check != BREAK;
         ++j) {
      int bit = get_bit(bit_figure[block.current][block.position], pos);
      if (check == CHECK_MODE && bit) {
        if (fsm_action == MV_down && (i > 19 || game_info.field[i][j])) {
          game_state = game_state == SPAWN ? GAMEOVER : ATTACHING;
          check = BREAK;
        } else if (fsm_action == MV_aside &&
                   (j > 9 || j < 0 || game_info.field[i][j])) {
          game_state = SHIFTING;
          check = BREAK;
        } else if (fsm_action == MV_rotaite &&
                   (i > 19 || j > 9 || j < 0 || game_info.field[i][j])) {
          game_state = SHIFTING;
          check = BREAK;
        }
      }
      if (check == FILL_MODE && bit) game_info.field[i][j] = bit;
      --pos;
    }
  }
}

/**
 * @brief Erases a current block from the field
 */
void erase_block() {
  int pos = 15;
  for (int i = block.initial_y; i < block.initial_y + 4; ++i) {
    for (int j = block.initial_x; j < block.initial_x + 4; ++j) {
      int bit = get_bit(bit_figure[block.current][block.position], pos);
      if (bit) {
        game_info.field[i][j] = 0;
      }
      --pos;
    }
  }
}

/**
 * @brief Get bit from figure mask
 * @param num
 * @param position(15-0)
 */
int get_bit(int num, int position) {
  int mask = 1 << position;
  return !!(num & mask);
}

/**
 * @brief Checks win lines in field, adds score and uplevel in case of win
 * lines
 */
void check_win_line() {
  int win_line_count = 0;
  for (int i = 0; i < HEIGHT; ++i) {
    int win_line = 1;
    for (int j = 0; j < WIDTH && win_line == 1; ++j) {
      if (!game_info.field[i][j]) win_line = 0;
    }
    if (win_line) {
      for (int j = 0; j < WIDTH; ++j) {
        if (game_info.field[i][j]) game_info.field[i][j] = 2;
        usleep(50000);
      }
      move_field_down(i);
      ++win_line_count;
    }
  }
  if (win_line_count) {
    add_score(win_line_count);
    up_level();
  }
}

/**
 * @brief adds score according to win lines quantity
 * @param win_lines quantity of win lines
 */
void add_score(int win_lines) {
  if (win_lines == 1) {
    game_info.score += 100;
  } else if (win_lines == 2) {
    game_info.score += 300;
  } else if (win_lines == 3) {
    game_info.score += 700;
  } else if (win_lines >= 4) {
    game_info.score += 1500;
  }
  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
    write_max_score();
  }
}

/**
 * @brief Removes win line from field and moves field accordingly
 * @param row win line row
 */
void move_field_down(int row) {
  for (int i = row; i >= 0; i--) {
    for (int j = 0; j < WIDTH; j++) {
      if (i != 0)
        game_info.field[i][j] = game_info.field[i - 1][j];
      else
        game_info.field[i][j] = 0;
    }
  }
}

/**
 * @brief Up level and increase speed
 */
void up_level() {
  if (game_info.level < 10 && game_info.score >= 600) {
    game_info.speed = DEFAULT_SPEED - SPEED_STEP * (game_info.score / 600);
    game_info.level = 1 + game_info.score / 600;
  }
}

/**
 * @brief Clears the field
 */
void clear_field() {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      game_info.field[i][j] = 0;
    }
  }
}

/**
 * @brief Reads maximum score from file
 * @returns maximum score
 */
int read_max_score() {
  int max_score = 0;
  FILE *file = NULL;

  file = fopen(PATH, "r");
  if (!file) {
    write_max_score();
    file = fopen(PATH, "r");
  }

  if (file) {
    if (fscanf(file, "%d", &max_score) != 1) max_score = ERROR;
    fclose(file);
  } else {
    max_score = ERROR;
  }
  return max_score;
}

/**
 * @brief Writes maximum score to file
 */
void write_max_score() {
  FILE *file = NULL;
  int error = SUCCESS;
  file = fopen(PATH, "w");
  if (file) {
    fprintf(file, "%d", game_info.score);
    fclose(file);
  } else {
    error = ERROR;
  }
  if (error == ERROR) game_state = EXIT_STATE;
}

/**
 * @brief Reset score, level, speed and clear field
 */
void reset_game_info() {
  game_info.score = 0;
  game_info.level = 1;
  game_info.speed = DEFAULT_SPEED;
  clear_field();
}

/**
 * @brief Fills a next block to **next
 */
void fill_next() {
  int pos = 15;
  for (int i = 0; i < 2; ++i) {
    for (int j = 3; j < 7; ++j) {
      int bit = get_bit(bit_figure[block.next][pos_1], pos);
      game_info.next[i][j] = bit;
      --pos;
    }
  }
}

/**
 * @brief Fiils the field with different values to set proper color in GUI
 */
void fill_go_field() {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      if (game_info.field[i][j]) game_info.field[i][j] = 3;
      usleep(5000);
    }
  }
}