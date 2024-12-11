#include "fsm.hpp"

using namespace s21;

t_dir Snake::move_dir = UP;
size_t Snake::length = 0U;
set<pair<size_t, size_t>> Snake::s_coordinates;
queue<pair<size_t, size_t>> Snake::q_coordinates;

size_t Field::width = WIDTH;
size_t Field::height = HEIGHT;
pair<size_t, size_t> Field::apple_coordinates;

size_t User::score = 0U;
size_t User::max_score = 0U;
size_t User::level = 1U;
size_t User::speed = DEFAULT_SPEED;

static GameState_s game_state = START;
static auto prev_time = chrono::system_clock::now();

action fsm_table[7][8] = {
    {spawn, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, set_pause, nullptr, turn, turn, turn, turn, shift_move},
    {game_over, game_over, game_over, game_over, game_over, game_over,
     game_over, game_over},
    {spawn, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {spawn, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {shift_move, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
};

/**
 * @brief Updates the game_info based on user input or timer count
 */
void updateCurrentState() {
  if (!game_info.pause) {
    timer_move();
    action act = fsm_table[game_state][user_action];
    if (act) act();
    fill_game_info();
  }
}

/**
 * @brief Spawn snake
 */
void spawn() {
  User::set_score_to_zero();
  User::set_level_to_zero();
  Snake::create_snake();
  Field::set_random_apple_coordinates(Snake::get_snake_coordinates());
  game_state = MOVE;
}

/**
 * @brief Turn snake to left or right (based on user input)
 */
void turn() {
  auto mv_dir = (UserAction_t)Snake::get_move_dir();
  if (((user_action == Up || user_action == Down) &&
       (mv_dir == Left || mv_dir == Right)) ||
      ((user_action == Left || user_action == Right) &&
       (mv_dir == Up || mv_dir == Down))) {
    bool lucky_move = Snake::move_snake((t_dir)user_action);
    move_helper(lucky_move);
    if (game_state != WIN_STATE) user_action = Start;
  }
}

/**
 * @brief Checks if it is time to auto move snake
 */
void timer_move() {
  if (game_state == MOVE) {
    auto current_time = chrono::system_clock::now();
    if (current_time > prev_time + chrono::milliseconds(User::get_speed())) {
      auto_move();
      prev_time = current_time;
    }
  }
}

/**
 * @brief Auto moves snake
 */
void auto_move() {
  bool lucky_move = Snake::move_snake(Snake::get_move_dir());
  move_helper(lucky_move);
}

/**
 * @brief Moves snake based on user Action
 */
void shift_move() {
  bool lucky_move = Snake::move_snake(Snake::get_move_dir());
  move_helper(lucky_move);
  if (game_state != WIN_STATE) user_action = Start;
}

/**
 * @brief Checks the possibility to move the snake, updates snake, scores,
 * length
 */
void move_helper(bool lucky_move) {
  if (!lucky_move)
    game_state = BUMP;
  else {
    if (Snake::contains(Field::get_apple_coordinates())) {
      Field::set_random_apple_coordinates(Snake::get_snake_coordinates());
      User::add_score(1);
      Snake::add_length();
      check_length();
    } else
      Snake::pop_tail();
    if (game_state != WIN_STATE) game_state = MOVE;
    prev_time = chrono::system_clock::now();
  }
}

/**
 * @brief Checks high_score and lenght, ups level and sets WIN_STATE
 */
void check_length() {
  if (User::get_score() > User::get_max_score()) User::set_max_score();
  if (User::get_score() % 5 == 0) User::up_level();
  if (Snake::get_lenght() >= MAX_POINTS) {
    game_info.pause = (int)WIN;
    game_state = WIN_STATE;
    user_action = Action;
  }
}

/**
 * @brief Updates game_info according to snake and apple positions
 */
void fill_game_info() {
  game_info.level = User::get_level();
  game_info.high_score = User::get_max_score();
  game_info.score = User::get_score();
  game_info.speed = User::get_speed();
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (game_state == BUMP || game_state == GAMEOVER)
        game_info.field[i][j] = Snake::contains(make_pair(i, j)) ? 3 : 0;
      else if (game_state == WIN_STATE)
        game_info.field[i][j] = Snake::contains(make_pair(i, j)) ? 2 : 0;
      else
        game_info.field[i][j] = Snake::contains(make_pair(i, j)) ? 1 : 0;
    }
  }
  if (game_state != BUMP && game_state != GAMEOVER)
    game_info.field[Field::get_apple_coordinates().first]
                   [Field::get_apple_coordinates().second] = 2;
}

/**
 * @brief Turns on game_over state
 */
void game_over() {
  game_state = GAMEOVER;
  game_info.pause = (int)GAME_OVER;
  user_action = Action;
}

/**
 * @brief Turns on pause state
 */
void set_pause() {
  game_state = PAUSE_STATE;
  game_info.pause = (int)PAUSE;
}
