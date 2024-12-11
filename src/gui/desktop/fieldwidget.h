#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QCoreApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/common/common_objects.h"
#include "../../brick_game/snake/controller_gui.hpp"
#include "../../brick_game/tetris/tetris_contr_gui.h"

void get_user_action(int user_input);

typedef enum {
  Tetris = 1,
  Snake = 2,
} t_selector;

extern t_selector game_selector;

class FieldWidget : public QWidget {
  Q_OBJECT
 public:
  explicit FieldWidget(QWidget *parent = nullptr);
  ~FieldWidget();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void updateFieldDisplay();

 private slots:
  void handleTerminateGame();

 private:
  void initializeGame();
  void get_cwd();
  void draw_information(QPainter *painter);
  void handle_pause(QPainter *painter);
  void handle_gameover(QPainter *painter);
  void handle_win(QPainter *painter);
  const int cellSize = 30;
  QTimer *timer;
  QPainter painter;
  QFont font;
};

void qt_game_info_init();
extern GameInfo_t game_info;
extern UserAction_t user_action;

#endif  // FIELDWIDGET_H
