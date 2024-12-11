#include "gui.h"

/**
 * @brief Prints window (calls overlay, playground, stats)
 * @param start 1/0 to print additional info
 */
void print_window(int start) {
  erase();
  print_overlay(start);
  if (!start) print_playground();
  if (!start && game_selector == Tetris) print_next_figure();
  print_stats();
}

/**
 * @brief Prints game overlay
 * @param start 1/0 to print additional info
 */
void print_overlay(int start) {
  print_rectangle(1, 3, WIDTH * 4 + 3, WIDTH * 4 + POINTS_WIDTH + 9);
  print_rectangle(4, 6, WIDTH * 4 + 3, WIDTH * 4 + POINTS_WIDTH + 9);
  print_rectangle(7, 9, WIDTH * 4 + 3, WIDTH * 4 + POINTS_WIDTH + 9);
  if (game_selector == Tetris)
    print_rectangle(10, 16, WIDTH * 4 + 3, WIDTH * 4 + POINTS_WIDTH + 9);
  print_rectangle(0, HEIGHT * 2, 3, (WIDTH + 1) * 3);

  MVPRINTW(2, WIDTH * 4 + 5, "LEVEL");
  MVPRINTW(5, WIDTH * 4 + 5, "SCORE");
  MVPRINTW(8, WIDTH * 4 + 5, "MAX SCORE");
  if (game_selector == Tetris) MVPRINTW(11, WIDTH * 4 + 7, "NEXT BLOCK");
  if (!start) {
    MVPRINTW(18, WIDTH * 4 + 2, "PRESS <P> TO PAUSE");
    if (game_selector == Tetris) {
      MVPRINTW(20, WIDTH * 4 + 2, "<SPACE> ROTATE BLOCK");
      MVPRINTW(22, WIDTH * 4 + 2, "<ARROWS> MOVE BLOCK");
    } else {
      MVPRINTW(20, WIDTH * 4 + 2, "SPACE - MOVE FASTER");
      MVPRINTW(22, WIDTH * 4 + 2, "ARROWS - TURN SNAKE");
    }
  }
  if (start) {
    MVPRINTW(22, WIDTH * 4 + 2, "PRESS <ENTER> TO START");
    MVPRINTW(24, WIDTH * 4 + 2, "PRESS <ESCAPE> TO EXIT");
  }

  if (game_selector == Tetris) {
    draw_tetris(0);
  } else if (game_selector == Snake) {
    draw_snake(0);
  }
}

/**
 * @brief Prints tetris line
 */
void draw_tetris(int start) {
  if (start) {
    MVPRINTW(1, 3, " _____ _____ ___________ _____ _____ ");
    MVPRINTW(2, 3, "|_   _|  ___|_   _| ___ \\_   _/  ___|");
    MVPRINTW(3, 3, "  | | | |__   | | | |_/ / | | \\ `--. ");
    MVPRINTW(4, 3, "  | | |  __|  | | |    /  | |  `--. \\");
    MVPRINTW(5, 3, "  | | | |___  | | | |\\ \\ _| |_/\\__/ /");
    MVPRINTW(6, 3, "  \\_/ \\____/  \\_/ \\_| \\_|\\___/\\____/ ");
  } else {
    MVPRINTW(1, WIDTH * 7 - 5, " _____ _____ ___________ _____ _____ ");
    MVPRINTW(2, WIDTH * 7 - 5, "|_   _|  ___|_   _| ___ \\_   _/  ___|");
    MVPRINTW(3, WIDTH * 7 - 5, "  | | | |__   | | | |_/ / | | \\ `--. ");
    MVPRINTW(4, WIDTH * 7 - 5, "  | | |  __|  | | |    /  | |  `--. \\");
    MVPRINTW(5, WIDTH * 7 - 5, "  | | | |___  | | | |\\ \\ _| |_/\\__/ /");
    MVPRINTW(6, WIDTH * 7 - 5, "  \\_/ \\____/  \\_/ \\_| \\_|\\___/\\____/ ");
  }
}

/**
 * @brief Prints snake line
 */
void draw_snake(int start) {
  if (start) {
    MVPRINTW(8, 3, " _____ _   _   ___   _   __ _____ ");
    MVPRINTW(9, 3, "/  ___| \\ | | / _ \\ | | / /|  ___|");
    MVPRINTW(10, 3, "\\ `--.|  \\| |/ /_\\ \\| |/ / | |__  ");
    MVPRINTW(11, 3, " `--. \\ . ` ||  _  ||    \\ |  __| ");
    MVPRINTW(12, 3, "/\\__/ / |\\  || | | || |\\  \\| |___ ");
    MVPRINTW(13, 3, "\\____/\\_| \\_/\\_| |_/\\_| \\_/\\____/ ");
  } else {
    MVPRINTW(1, WIDTH * 7 - 5, " _____ _   _   ___   _   __ _____ ");
    MVPRINTW(2, WIDTH * 7 - 5, "/  ___| \\ | | / _ \\ | | / /|  ___|");
    MVPRINTW(3, WIDTH * 7 - 5, "\\ `--.|  \\| |/ /_\\ \\| |/ / | |__  ");
    MVPRINTW(4, WIDTH * 7 - 5, " `--. \\ . ` ||  _  ||    \\ |  __| ");
    MVPRINTW(5, WIDTH * 7 - 5, "/\\__/ / |\\  || | | || |\\  \\| |___ ");
    MVPRINTW(6, WIDTH * 7 - 5, "\\____/\\_| \\_/\\_| |_/\\_| \\_/\\____/ ");
  }
}

/**
 * @brief Prints rectangle boarders
 * @param top_y
 * @param bottom_y
 * @param left_x
 * @param right_x
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);
  int i = left_x + 1;
  for (; i < right_x; i++) {
    MVADDCH(top_y, i, ACS_HLINE);
  }
  MVADDCH(top_y, i, ACS_URCORNER);
  for (int j = top_y + 1; j < bottom_y; j++) {
    MVADDCH(j, left_x, ACS_VLINE);
    MVADDCH(j, right_x, ACS_VLINE);
  }
  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

/**
 * @brief Prints game statistics
 */
void print_stats() {
  MVPRINTW(2, WIDTH * 4 + 16, "%d", game_info.level);
  MVPRINTW(5, WIDTH * 4 + 16, "%d", game_info.score);
  MVPRINTW(8, WIDTH * 4 + 16, "%d", game_info.high_score);
}

/**
 * @brief Prints game field
 */
void print_playground() {
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      int y = 3 + i;
      int x = 6 + j;
      if (j != 0) x += 2 * (x - 6);
      if (i != 0) y += (y - 3);
      if (game_info.field[i][j] == 1) {
        attron(COLOR_PAIR(1));
        mvprintw(y, x, "  ");
        attroff(COLOR_PAIR(1));
      } else if (game_info.field[i][j] == 2) {
        attron(COLOR_PAIR(4));
        mvprintw(y, x, "  ");
        attroff(COLOR_PAIR(4));
      } else if (game_info.field[i][j] == 3) {
        attron(COLOR_PAIR(5));
        mvprintw(y, x, "  ");
        attroff(COLOR_PAIR(5));
      } else {
        attron(COLOR_PAIR(2));
        mvprintw(y, x, "  ");
        attroff(COLOR_PAIR(2));
      }
    }
  }
}

/**
 * @brief Prints game over info
 */
void print_gameover() {
  attron(COLOR_PAIR(6));
  MVPRINTW(26, WIDTH * 4 + 2, "GAME OVER");
  MVPRINTW(28, WIDTH * 4 + 2, "YOUR LEVEL: %d", game_info.level);
  MVPRINTW(30, WIDTH * 4 + 2, "YOUR SCORE: %d", game_info.score);
  MVPRINTW(32, WIDTH * 4 + 2, "PRESS ENTER TO TRY AGAIN");
  MVPRINTW(34, WIDTH * 4 + 2, "PRESS ESCAPE TO EXIT");
  attroff(COLOR_PAIR(6));
}

/**
 * @brief Prints win info
 */
void print_win() {
  attron(COLOR_PAIR(7));
  MVPRINTW(26, WIDTH * 4 + 2, "WIN");
  MVPRINTW(28, WIDTH * 4 + 2, "YOUR LEVEL: %d", game_info.level);
  MVPRINTW(30, WIDTH * 4 + 2, "YOUR SCORE: %d", game_info.score);
  MVPRINTW(32, WIDTH * 4 + 2, "PRESS ENTER TO TRY AGAIN");
  MVPRINTW(34, WIDTH * 4 + 2, "PRESS ESCAPE TO EXIT");
  attroff(COLOR_PAIR(7));
}

/**
 * @brief Prints next figure
 */
void print_next_figure() {
  attron(COLOR_PAIR(1));
  for (int i = 0; i < 2; ++i) {
    for (int j = 3; j < 7; ++j) {
      if (game_info.next[i][j]) {
        int y = 13 + i * 2;
        int x = WIDTH * 4 - 2 + j + j * 2;
        MVPRINTW(y, x, "  ");
      }
    }
  }
  attroff(COLOR_PAIR(1));
}

/**
 * @brief Prints pause info
 */
void print_pause() {
  MVPRINTW(32, WIDTH * 4 + 1, "PRESS <ENTER> TO CONTINUE");
  attron(COLOR_PAIR(1));
  mvprintw(26, WIDTH * 4 + 11, "  ");
  mvprintw(27, WIDTH * 4 + 11, "  ");
  mvprintw(28, WIDTH * 4 + 11, "  ");
  mvprintw(29, WIDTH * 4 + 11, "  ");
  mvprintw(30, WIDTH * 4 + 11, "  ");

  mvprintw(26, WIDTH * 4 + 16, "  ");
  mvprintw(27, WIDTH * 4 + 16, "  ");
  mvprintw(28, WIDTH * 4 + 16, "  ");
  mvprintw(29, WIDTH * 4 + 16, "  ");
  mvprintw(30, WIDTH * 4 + 16, "  ");
  attroff(COLOR_PAIR(1));
}