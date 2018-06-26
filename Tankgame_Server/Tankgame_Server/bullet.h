#ifndef _BULLET_H
#define _BULLET_H

#include "../../signal.h"
#include <time.h>

#ifndef BULLET_MOVE_DELAY
	#define BULLET_MOVE_DELAY 33
#endif

#define MAX_BULLET_COUNT 5

typedef struct Bullet {
	char nDirect;
	int nOldMoveTime;
	char x;
	char y;
} Bullet;

void correctionBullet(Bullet *bullet);

#endif _BULLET_H