#pragma once
#include "main.h"
#include "tank.h"

#ifndef BULLET_MOVE_DELAY
	#define BULLET_MOVE_DELAY 50
#endif

struct Bullet {
	int nLife;
	int nDirect;
	int nOldMoveTime;
	COORD nPos;
	COORD nOldPos;
	int state;
};

int hBullet;
struct Bullet g_Tank_Bullet[5];

