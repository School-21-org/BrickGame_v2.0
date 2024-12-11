#include "tetris_contr.h"

#include "tetris_contr_gui.h"

static pthread_t thread;
static _Atomic int run_thread = 0;
static _Atomic int thread_active = 0;

/**
 * @brief Loop backed tetris game in a separate thread
 */
void *tetris_back_thread_loop() {
  while (atomic_load(&run_thread)) {
    updateCurrentState();
    usleep(30000);
  }
  atomic_store(&thread_active, 0);
  return NULL;
}

/**
 * @brief Launches a thread with the tetris backend game
 */
void launch_tetris_game() {
  tet_game_info_init();
  if (!atomic_load(&thread_active)) {
    atomic_store(&thread_active, 1);
    atomic_store(&run_thread, 1);
    pthread_create(&thread, NULL, tetris_back_thread_loop, NULL);
    pthread_detach(thread);
  }
}

/**
 * @brief Detaches a thread with the tetris backend game
 */
void detach_tetris_game() {
  atomic_store(&run_thread, 0);
  while (atomic_load(&thread_active)) {
    usleep(10000);
  }
  mem_free();
}
