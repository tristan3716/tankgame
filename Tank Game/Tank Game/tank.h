#pragma once
#include "main.h"

#define FLAG_DEFAULT 999
#define FLAG_NONE 1000
#define FLAG_MOVED 1001
#define FLAG_TURN 1002
#define FLAG_HOLD 1003

typedef enum _DIRECT { UP, DOWN, LEFT, RIGHT } DIRECT;

struct Tank {
	int clientId;
	int nLife;
	int nDirect;
	int nLeftBulletCount;
	int nMoveTime;
	int nOldMoveTime;
	int nFireTime;
	int nOldFireTime;
	int flag;
	COORD nPos;
	COORD nOldPos;
}; 

void initiateTank(void);
int isMoved(int flag);