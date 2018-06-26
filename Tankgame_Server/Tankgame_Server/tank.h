#ifndef _TANK_H
#define _TANK_H

#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>

#include "bullet.h"
#include "main.h"

struct Tank {
	int CID;
	char x;
	char y;
	char nDirect;
	Bullet bullet[MAX_BULLET_COUNT];
	char nCurBulletCount;
};

void updateBullet();

BOOL collisionCheck(int ***map, struct Bullet *bullet, SOCKET *hSocket);
void collision(struct Bullet *bullet, SOCKET *hSocket, char id);

void kill(int x, int y);

#endif _TANK_H