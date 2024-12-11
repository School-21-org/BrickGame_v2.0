extern "C" {
#include "gui.h"
}

#include <iostream>
using namespace std;

int main() {
  WIN_INIT(0);
  gui_loop();
  endwin();
  return 0;
}
