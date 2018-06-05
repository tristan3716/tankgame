#pragma once
#include "main.h"
#include "tank.h"

#define BULLET_MOVE_DELAY 50

struct Bullet {
	int nLife;
	int nDirect;
	int nOldMoveTime;
	COORD nPos;
};

int hBullet;
struct Bullet g_Tank_Bullet[5];

void handleBullet(void);