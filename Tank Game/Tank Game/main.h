#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // MMSystem.h
#include <MMSystem.h> // PlaySound function
#include <process.h> // _beginthreadex
#include "sound.h" 
#include "console.h"
#include "screen.h"

#include "map.h"
#include "key.h"

#define MAX_BUF_SIZE 1024

#define true 1
#define false 0

CRITICAL_SECTION g_cs;

struct Params {
	struct SoundList *sHd;
	int id;
};

extern struct Tank g_Tank;

#endif // MAIN_H