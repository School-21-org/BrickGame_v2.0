#ifndef __COMMON_OBJECTS_H__
#define __COMMON_OBJECTS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Start = 0,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

extern GameInfo_t game_info;
extern UserAction_t user_action;
extern char PATH[500];

typedef enum { GAME = 0, PAUSE, GAME_OVER, WIN } t_state;

#ifdef __cplusplus
}
#endif

#endif