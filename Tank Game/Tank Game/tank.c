#include "tank.h"

struct Tank g_Tank;

void initiateTank(struct Tank *this) {
	this->nFlag = DEFAULT;
	this->nPos.X = 10;
	this->nPos.Y = 10;
	this->nOldPos.X = 0;
	this->nOldPos.Y = 0;

	this->isMoved = isMoved;
}

int isMoved(int flag) {
	if (flag == MOVED) {
		g_Tank.nFlag = NONE;
		return g_Tank.nOldPos.X != g_Tank.nPos.X || g_Tank.nOldPos.Y != g_Tank.nPos.Y;
	}
	else return FALSE;
}