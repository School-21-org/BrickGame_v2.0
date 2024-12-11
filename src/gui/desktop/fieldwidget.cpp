#include "fieldwidget.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

GameInfo_t game_info = {nullptr, nullptr, 0, 0, 0, 0, 0};
UserAction_t user_action = Start;
char PATH[500] = {'\0'};

FieldWidget::FieldWidget(QWidget *parent) : QWidget{parent} {
  get_cwd();
  initializeGame();

  font.setPointSize(14);
  setFixedSize(600, 600);
  setFocusPolicy(Qt::StrongFocus);

  // timer to periodically update the matrix display
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &FieldWidget::updateFieldDisplay);
  timer->start(30);  // ms
}

FieldWidget::~FieldWidget() { delete timer; }

void FieldWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  painter.begin(this);
  painter.setFont(font);

  handle_pause(&painter);
  handle_gameover(&painter);

  if (game_selector == Snake) handle_win(&painter);

  for (int row = 0; row < 20 && game_info.field; ++row) {
    for (int col = 0; col < 10 && game_info.field; ++col) {
      if (game_info.field[row][col] == 1) {
        painter.setBrush(Qt::black);
      } else if (game_info.field[row][col] == 2) {
        painter.setBrush(Qt::green);
      } else if (game_info.field[row][col] == 3) {
        painter.setBrush(Qt::red);
      } else {
        painter.setBrush(Qt::white);
      }
      painter.drawRect(col * cellSize, row * cellSize, cellSize, cellSize);
    }
  }

  if (game_selector == Tetris) {
    for (int row = 0; row < 4 && game_info.next; ++row) {
      for (int col = 3; col < 7 && game_info.next; ++col) {
        if (game_info.next[row][col]) {
          painter.setBrush(Qt::black);
        } else {
          painter.setBrush(Qt::white);
        }
        painter.drawRect(col * cellSize + 300, row * cellSize + 40, cellSize,
                         cellSize);
      }
    }
  }

  draw_information(&painter);

  painter.end();
}

void FieldWidget::draw_information(QPainter *painter) {
  if (game_selector == Tetris) painter->drawText(390, 25, "NEXT BLOCK");
  std::string score = "SCORE: " + std::to_string(game_info.score);
  painter->drawText(390, 200, score.data());
  std::string h_score = "MAX SCORE: " + std::to_string(game_info.high_score);
  painter->drawText(390, 240, h_score.data());
  std::string level = "LEVEL: " + std::to_string(game_info.level);
  painter->drawText(390, 280, level.data());
}

void FieldWidget::handle_pause(QPainter *painter) {
  if (game_info.pause == (int)PAUSE) {
    painter->drawText(390, 320, "PAUSED");
    painter->drawText(390, 360, "Press ENTER to continue");
    if (user_action == Start) {
      game_info.pause = (int)GAME;
    }
  }
}

void FieldWidget::handle_gameover(QPainter *painter) {
  if (game_info.pause == (int)GAME_OVER) {
    painter->drawText(390, 320, "GAME OVER");
    painter->drawText(390, 360, "Press ENTER to try again");
    painter->drawText(390, 400, "Press ESCAPE to exit");
    if (user_action == Start) {
      game_info.pause = (int)GAME;
    }
  }
}

void FieldWidget::handle_win(QPainter *painter) {
  if (game_info.pause == (int)WIN) {
    painter->drawText(390, 320, "WIN");
    painter->drawText(390, 360, "Press ENTER to try again");
    painter->drawText(390, 400, "Press ESCAPE to exit");
    if (user_action == Start) {
      game_info.pause = (int)GAME;
    }
  }
}

void FieldWidget::keyPressEvent(QKeyEvent *event) {
  get_user_action(event->key());
  if (user_action == Terminate) {
    if (!game_info.pause) game_info.pause = (int)PAUSE;
    handleTerminateGame();
  } else {
    QWidget::keyPressEvent(event);
  }
}

void FieldWidget::updateFieldDisplay() { update(); }

/**
 * @brief Transforms raw user input into static user action
 */
void get_user_action(int user_input) {
  if (user_input == Qt::Key_Down)
    user_action = Down;
  else if (user_input == Qt::Key_Up)
    user_action = Up;
  else if (user_input == Qt::Key_Left)
    user_action = Left;
  else if (user_input == Qt::Key_Right)
    user_action = Right;
  else if (user_input == Qt::Key_Escape)
    user_action = Terminate;
  else if (user_input == Qt::Key_Return)
    user_action = Start;
  else if (user_input == Qt::Key_Space)
    user_action = Action;
  else if (user_input == Qt::Key_P)
    user_action = Pause;
}

void FieldWidget::handleTerminateGame() { QCoreApplication::quit(); }

void FieldWidget::initializeGame() {
  qt_game_info_init();
  if (game_selector == Tetris) {
    launch_tetris_game();
  } else if (game_selector == Snake) {
    launch_snake_game();
  }
}

void FieldWidget::get_cwd() {
  QString appDir = QCoreApplication::applicationDirPath();
  std::filesystem::path currentDir =
      std::filesystem::path(appDir.toStdString());
  std::filesystem::path subpathToRemove = "brickgame.app/Contents/MacOS";
  while (!subpathToRemove.empty() &&
         currentDir.filename() == subpathToRemove.filename()) {
    currentDir = currentDir.parent_path();
    subpathToRemove = subpathToRemove.parent_path();
  }
  std::strcpy(PATH, currentDir.string().c_str());
  std::strcat(PATH, "/");
}

void qt_game_info_init() {
  size_t height = 20;
  size_t width = 10;
  game_info.field = new int *[height];
  for (size_t i = 0; i < height; ++i) game_info.field[i] = new int[width]{0};

  if (game_selector == Tetris) {
    game_info.next = new int *[4];
    for (size_t i = 0; i < 4; ++i) game_info.next[i] = new int[width]{0};
  }
}