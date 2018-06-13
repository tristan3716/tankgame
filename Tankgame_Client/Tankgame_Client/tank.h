#ifndef _TANK_H
#define _TANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcpy
#include <Windows.h> // mbstowcs
#include "keyhandler.h"

struct Tank {
	short CID;
	COORD nCurPosition;
	int nDirect;
};


#endif _TANK_H