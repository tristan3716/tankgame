#ifndef _KEY_HANDLER_H
#define _KEY_HANDLER_H

#include "stdafx.h"
#include "main.h"

#define MOVE_DELAY 33
#define FIRE_DELAY 33

typedef enum _DIRECT { UP, DOWN, LEFT, RIGHT } DIRECT;

unsigned int WINAPI handleKey(void *arg);

#endif _KEY_HANDLER_H