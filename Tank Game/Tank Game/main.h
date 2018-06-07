#ifndef MAIN_H
#define MAIN_H

#pragma comment(lib, "WS2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // MMSystem.h
#include <MMSystem.h> // PlaySound function
#include <process.h> // _beginthreadex
//#include <winsock2.h>
#include "sound.h" 
#include "console.h"
#include "screen.h"

#include "map.h"
#include "key.h"

#ifndef MAX_BUF_SIZE
	#define MAX_BUF_SIZE 1024
#endif

#ifndef FALSE
	#define FALSE 0
#endif
#ifndef TRUE
	#define TRUE !FALSE
#endif

CRITICAL_SECTION g_cs;

struct Params {
	struct SoundList *sHd;
	int id;
};

extern struct Tank g_Tank;

#endif // MAIN_H