#ifndef __CONTROLLER_HPP__
#define __CONTROLLER_HPP__

#include <pthread.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>

#include "../common/common_objects.h"
#include "../common/defines.h"
#include "fsm.hpp"

using namespace std;

void* back_thread_loop(void*);
void game_info_init();
void game_info_free();

#endif