#ifndef _TANK_H
#define _TANK_H

#include "keyhandler.h"

struct Tank {
	char id;
	char x;
	char y;
	char oldX[2];
	char oldY[2];
	int nDirect;
};


#endif _TANK_H