# Спецификация для библиотеки игры из коллекции игр BrickGame

Функция `userInput` принимает на вход пользовательское действие `action` и дополнительный параметр `hold`, который отвечает за зажатие клавиши.

Функция `updateCurrentState` предназначена для получения данных для отрисовки в интерфейсе. Она возвращает структуру, содержащую информацию о текущем состоянии игры. Например, для тетриса истечение таймера приводит к смещению фигуры вниз на один ряд. Эта функция должна вызываться из интерфейса с некоторой периодичностью для поддержания интерфейса в актуальном состоянии.

```c
typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t;

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();
```
