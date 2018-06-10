#pragma once
#include "main.h"

//#define FLAG_NONE 600
//#define FLAG_DEFAULT 601
//#define FLAG_MOVED 602
//#define FLAG_TURN 603
//#define FLAG_HOLD 604

#ifndef MOVE_DELAY
	#define MOVE_DELAY 33
#endif // (33 ms)
#ifndef FIRE_DELAY
	#define FIRE_DELAY 33
#endif // (33 ms)

typedef enum _DIRECT { UP, DOWN, LEFT, RIGHT } DIRECT;
typedef enum _FLAG {NONE=600, DEFAULT, MOVED, TURN, HOLD} FLAG;

struct Tank {
	int clientId;
	int nLife;
	int nDirect;
	int nFlag;
	int nLeftBulletCount;
	int nOldMoveTime;
	int nOldFireTime;
	COORD nPos;
	COORD nOldPos;

	BOOL (*isMoved)(int flag);
}; 

void initiateTank(struct Tank *tank);
int isMoved(int flag);