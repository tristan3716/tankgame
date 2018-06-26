#include "stdafx.h"
#include "bullet.h"


struct Bullet *g_bullet;

void initializedBullet(struct Bullet **arg_bullet) {
	int i;
	int nBulletCount = nMaxUserCount * MAX_BULLET_COUNT;
	*arg_bullet = (struct Bullet *)malloc(sizeof(struct Bullet) * nBulletCount);
	for (i = 0; i < nBulletCount; i++) {
		(*arg_bullet)[i].nDirect = DIE;
		(*arg_bullet)[i].oldX[0] = 0;
		(*arg_bullet)[i].oldX[1] = 0;
		(*arg_bullet)[i].oldY[0] = -1;
		(*arg_bullet)[i].oldY[1] = -1;
		(*arg_bullet)[i].flag[0] = 0;
		(*arg_bullet)[i].flag[1] = 0;
	}
}
