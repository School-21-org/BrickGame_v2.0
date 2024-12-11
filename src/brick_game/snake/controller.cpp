#include "controller.hpp"

#include "controller_gui.hpp"

static pthread_t snake_thread;
static std::atomic<int> run_snake_thread(0);
static std::atomic<int> snake_thread_active(0);

/**
 * @brief Runs backend in separate thread untill it is active
 */
void* back_thread_loop(void*) {
  while (run_snake_thread.load()) {
    updateCurrentState();
    usleep(50000);
  }
  snake_thread_active.store(0);
  return nullptr;
}

/**
 * @brief Launches the snake game
 */
void launch_snake_game() {
  game_info_init();
  if (!snake_thread_active.load()) {
    snake_thread_active.store(1);
    run_snake_thread.store(1);
    pthread_create(&snake_thread, nullptr, back_thread_loop, nullptr);
    pthread_detach(snake_thread);
  }
}

/**
 * @brief Detaches a thread with the snake backend game
 */
void detach_snake_game() {
  run_snake_thread.store(0);
  while (snake_thread_active.load()) {
    usleep(10000);
  }
  game_info_free();
}

/**
 * @brief Initializes PATH and allocates memory for field
 */
void game_info_init() {
  strcat(PATH, "snake_max_score.txt");
  if (game_info.field == nullptr) {
    game_info.field = new int*[HEIGHT];
    for (size_t i = 0; i < HEIGHT; ++i) game_info.field[i] = new int[WIDTH]{0};
  }
}

/**
 * @brief Deallocates memory for field
 */
void game_info_free() {
  if (game_info.field != nullptr) {
    for (size_t i = 0; i < HEIGHT; ++i) delete[] game_info.field[i];
    delete[] game_info.field;
    game_info.field = nullptr;
  }
}