#ifndef _BULLET_H
#define _BULLET_H

#include "main.h"

#define MAX_BULLET_COUNT 5

struct Bullet {
	char x;
	char y;
	char oldX[2];
	char oldY[2];
	char nDirect;
	char flag[2];
};


void initializedBullet(struct Bullet **bullet);

void updateBullet();

#endif