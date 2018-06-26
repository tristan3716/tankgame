#ifndef _MAIN_H
#define _MAIN_H

#pragma comment(lib, "WS2_32.lib")
#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h> // need to locate above windows.h
#include <Windows.h> // MMSystem.h
#include <MMSystem.h> // PlaySound function
#include <process.h> // _beginthreadex
#include <time.h> // clock
#include "map.h"
//#include "console.h"
//#include "screen.h"

//#include "key.h"
#include "pong.h"
#include "tank.h"
#include "signal.h"

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef FALSE
	#define FALSE (0)
#endif
#ifndef TRUE
	#define TRUE !FALSE
#endif

#define DEBUG

#define DEFAULT_PORT 46464
#define USER_COUNT 2
#define MAP_NUMBER 1
#define TICK_DELAY 120

extern struct Tank *g_Tank;


void handleError(const char *message);
BOOL isPossibleMove(struct Tank tank, int **map, int nDirect);
int getIndexfromID(int id, int *indexlist);
int getIDfromIndex(int index, int *indexlist);

#endif _MAIN_H