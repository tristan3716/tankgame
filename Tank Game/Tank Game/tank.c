#include "tank.h"

struct Tank g_Tank;

void initiateTank(void) {
	g_Tank.flag = FLAG_DEFAULT;
	g_Tank.nPos.X = 10;
	g_Tank.nPos.Y = 10;
	g_Tank.nMoveTime = 33;
	g_Tank.nFireTime = 33;
}

int isMoved(int flag) {
	if (flag == FLAG_MOVED) {
		g_Tank.flag = FLAG_NONE;
		return g_Tank.nOldPos.X != g_Tank.nPos.X || g_Tank.nOldPos.Y != g_Tank.nPos.Y;
	}
	else return false;
}