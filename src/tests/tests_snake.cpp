#include "tests_run.hpp"

GameInfo_t game_info = {nullptr, nullptr, 0, 0, 0, 0, 0};
UserAction_t user_action = Action;
char PATH[500] = "snake_max_score.txt";

TEST(Suite_user_class, add_score) {
  User::add_score(15U);
  EXPECT_EQ(User::get_score(), 15U);
}

TEST(Suite_user_class, get_score) {
  size_t score = User::get_score();
  EXPECT_EQ(User::get_score(), score);
}

TEST(Suite_user_class, set_score_to_zero) {
  User::set_score_to_zero();
  EXPECT_EQ(User::get_score(), 0U);
}

TEST(Suite_user_class, set_max_score) {
  User::add_score(15U);
  User::set_max_score();
  EXPECT_EQ(User::get_max_score(), 15U);
}

TEST(Suite_user_class, get_level) { EXPECT_EQ(User::get_level(), 1U); }

TEST(Suite_user_class, up_level) {
  User::up_level();
  EXPECT_EQ(User::get_level(), 2U);
}

TEST(Suite_user_class, set_level_to_zero) {
  User::set_level_to_zero();
  EXPECT_EQ(User::get_level(), 1U);
}

TEST(Suite_snake_class, get_length) { EXPECT_EQ(Snake::get_lenght(), 0U); }

TEST(Suite_snake_class, create_snake) {
  Snake::create_snake();
  EXPECT_EQ(Snake::get_lenght(), 4U);
}

TEST(Suite_fsm_logic, start_state) {
  if (game_info.field == nullptr) {
    game_info.field = new int*[HEIGHT];
    for (size_t i = 0; i < HEIGHT; ++i) game_info.field[i] = new int[WIDTH]{0};
  }
  user_action = Start;
  updateCurrentState();
  EXPECT_EQ(Snake::get_lenght(), 4U);
  EXPECT_EQ(User::get_score(), 0U);
  EXPECT_EQ(User::get_level(), 1U);
  pair<size_t, size_t> apple = Field::get_apple_coordinates();
  EXPECT_EQ(game_info.field[apple.first][apple.second], 2);
}

TEST(Suite_fsm_logic, spawn_state) {
  user_action = Start;
  usleep(1000000);
  updateCurrentState();
  EXPECT_EQ(Snake::get_lenght(), 4U);
  EXPECT_EQ(User::get_score(), 0U);
  EXPECT_EQ(User::get_level(), 1U);
  pair<size_t, size_t> apple = Field::get_apple_coordinates();
  EXPECT_EQ(game_info.field[apple.first][apple.second], 2);
  size_t sum = 0;
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (game_info.field[i][j] == 1) sum += 1;
    }
  }
  EXPECT_EQ(sum, 4U);
}

TEST(Suite_fsm_logic, move_state_1) {
  set<pair<size_t, size_t>> snake_cor = Snake::get_snake_coordinates();
  user_action = Left;
  for (int i = 0; i < HEIGHT; ++i) {
    if (Snake::contains(make_pair(i, 0))) user_action = Right;
  }

  updateCurrentState();

  EXPECT_EQ(User::get_score(), 0U);
  EXPECT_EQ(User::get_level(), 1U);
  pair<size_t, size_t> apple = Field::get_apple_coordinates();
  EXPECT_EQ(game_info.field[apple.first][apple.second], 2);
  size_t sum = 0;
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (game_info.field[i][j] == 1) sum += 1;
    }
  }
  EXPECT_EQ(sum != 0, true);
}

TEST(Suite_fsm_logic, move_state_2) {
  set<pair<size_t, size_t>> snake_cor = Snake::get_snake_coordinates();
  user_action = Action;
  for (int i = 0; i < HEIGHT; ++i) {
    if (Snake::contains(make_pair(i, 0)) || Snake::contains(make_pair(i, 9)))
      user_action = Up;
  }

  updateCurrentState();

  EXPECT_EQ(User::get_score(), 0U);
  EXPECT_EQ(User::get_level(), 1U);
  pair<size_t, size_t> apple = Field::get_apple_coordinates();
  EXPECT_EQ(game_info.field[apple.first][apple.second], 2);
  size_t sum = 0;
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (game_info.field[i][j] == 1) sum += 1;
    }
  }
  EXPECT_EQ(sum != 0, true);
}

TEST(Suite_fsm_logic, move_state_3) {
  set<pair<size_t, size_t>> snake_cor = Snake::get_snake_coordinates();

  if (Snake::get_move_dir() == UP) {
    user_action = Left;
    for (int i = 0; i < HEIGHT; ++i) {
      if (Snake::contains(make_pair(i, 0))) user_action = Right;
    }
  } else if (Snake::get_move_dir() == RIGHT || Snake::get_move_dir() == LEFT) {
    user_action = Up;
  }

  updateCurrentState();

  EXPECT_EQ(User::get_score(), 0U);
  EXPECT_EQ(User::get_level(), 1U);
  pair<size_t, size_t> apple = Field::get_apple_coordinates();
  EXPECT_EQ(game_info.field[apple.first][apple.second], 2);
  size_t sum = 0;
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (game_info.field[i][j] == 1) sum += 1;
    }
  }
  EXPECT_EQ(sum != 0, true);
}
TEST(Suite_fsm_logic, move_state_5) {
  set<pair<size_t, size_t>> snake_cor = Snake::get_snake_coordinates();

  bool bump = false;
  while (bump != true) {
    user_action = Action;
    for (int i = 0; i < HEIGHT; ++i) {
      for (int j = 0; j < WIDTH; ++j) {
        if (i == 0 && Snake::get_move_dir() == UP &&
            Snake::contains(make_pair(i, j)))
          bump = true;
        if (i == 19 && Snake::get_move_dir() == DOWN &&
            Snake::contains(make_pair(i, j)))
          bump = true;
        if (j == 0 && Snake::get_move_dir() == LEFT &&
            Snake::contains(make_pair(i, j)))
          bump = true;
        if (j == 9 && Snake::get_move_dir() == RIGHT &&
            Snake::contains(make_pair(i, j)))
          bump = true;
      }
    }
    updateCurrentState();
  }

  size_t sum = 0;
  for (size_t i = 0; i < HEIGHT; ++i) {
    for (size_t j = 0; j < WIDTH; ++j) {
      if (game_info.field[i][j] == 3) sum += 1;
    }
  }
  EXPECT_EQ(sum != 0, true);
  updateCurrentState();
  EXPECT_EQ(game_info.pause, (int)GAME_OVER);
}

TEST(Suite_fsm_logic, restart) {
  user_action = Start;
  game_info.pause = (int)GAME;
  updateCurrentState();
  EXPECT_EQ(Snake::get_lenght(), 4U);
  EXPECT_EQ(User::get_score(), 0U);
  EXPECT_EQ(User::get_level(), 1U);
  pair<size_t, size_t> apple = Field::get_apple_coordinates();
  EXPECT_EQ(game_info.field[apple.first][apple.second], 2);
}

TEST(Suite_fsm_logic, pause) {
  user_action = Pause;
  updateCurrentState();
  EXPECT_EQ(game_info.pause, (int)PAUSE);
  game_info.pause = (int)GAME;
  user_action = Start;
  updateCurrentState();
}

TEST(Suite_additional_functions, check_length) {
  User::add_score(100U);
  check_length();
  EXPECT_EQ(User::get_score(), 100U);
  EXPECT_EQ(User::get_max_score(), 100U);
  EXPECT_EQ(User::get_level(), 2U);
}

TEST(Suite_additional_functions, check_length_win) {
  User::add_score(100U);
  for (int i = 0; i < 200; ++i) Snake::add_length();
  check_length();
  EXPECT_EQ(User::get_score(), 200U);
  EXPECT_EQ(User::get_max_score(), 200U);
  EXPECT_EQ(User::get_level(), 3U);

  EXPECT_EQ(game_info.pause, (int)WIN);

  if (game_info.field != nullptr) {
    for (size_t i = 0; i < HEIGHT; ++i) delete[] game_info.field[i];
    delete[] game_info.field;
    game_info.field = nullptr;
  }
}

TEST(Suite_additional_functions, read_max_score) {
  User::read_max_score();
  EXPECT_EQ(User::get_max_score(), 200U);
}
