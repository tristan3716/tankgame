#include "bullet.h"

void correctionBullet(Bullet *bullet) { // Correction Bullet by Bullet's nDirect
	switch (bullet->nDirect) {
		case UP:
			bullet->y = bullet->y - 1;
			break;
		case DOWN:
			bullet->y = bullet->y + 1;
			break;
		case LEFT:
			bullet->x = bullet->x - 1;
			break;
		case RIGHT:
			bullet->x = bullet->x + 1;
			break;
	}
}
