#ifndef _MAIN_H
#define _MAIN_H

#pragma comment(lib, "WS2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // MMSystem.h
#include <MMSystem.h> // PlaySound function
#include <process.h> // _beginthreadex
#include <time.h> // clock
//#include <winsock2.h>
#include "sound.h" 
//#include "console.h"
//#include "screen.h"

#include "map.h"
//#include "key.h"

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

#define DEFAULT_PORT 46464
#define USER_COUNT 1
#define MAP_NUMBER 1

CRITICAL_SECTION g_cs;

typedef enum _SIGNAL {
	DEFAULT_INFORMATION = 101, PING = 102, MATCHED = 103, JOIN_USER = 104
} SIGNAL;

void handleError(const char *message);

#endif _MAIN_H