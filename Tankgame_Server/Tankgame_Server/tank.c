#include "tank.h"

struct Tank *g_Tank;

BOOL isBulletAlive(char direct) {
	return direct != DIE;
}

BOOL isPossibleMoveBullet(time_t nTime) {
	return nTime >= BULLET_MOVE_DELAY;
}

void updateBullet() {
	int i, j;
	clock_t nCurTime;

	nCurTime = clock();
	for (i = 0; i < USER_COUNT; i++) { // User Count
		for (j = 0; j < MAX_BULLET_COUNT; j++){ // User's Bullet Count
			if (isBulletAlive(g_Tank[i].bullet[j].nDirect)) {
				if (isPossibleMoveBullet(nCurTime - g_Tank[i].bullet[j].nOldMoveTime)) {
					switch (g_Tank[i].bullet[j].nDirect) { // Moving Calculate by Direct
						case UP:
							g_Tank[i].bullet[j].y -= 1;
							break;
						case DOWN:
							g_Tank[i].bullet[j].y += 1;
							break;
						case LEFT:
							g_Tank[i].bullet[j].x -= 1;
							break;
						case RIGHT:
							g_Tank[i].bullet[j].x += 1;
							break;
					}
					g_Tank[i].bullet[j].nOldMoveTime = nCurTime;
				}
			}
		}
	}
}

void getOldPosition(struct Bullet bullet, char *x, char *y) {
	switch (bullet.nDirect) {
		case UP:
			*y += 1;
			break;
		case DOWN:
			*y -= 1;
			break;
		case LEFT:
			*x += 1;
			break;
		case RIGHT:
			*x -= 1;
			break;
	}
}

BOOL collisionCheck(int ***map, struct Bullet *bullet, SOCKET *hSocket) {
	int k, i;
	int map_value;
	int **rmap = *map;
	char msg[5];

	map_value = rmap[bullet->y][bullet->x];

	switch (map_value) {
		case WALL:
			return 1;
	}

	for (k = 0; k < USER_COUNT; k++) {
		if (g_Tank[k].nDirect == DIE)
			continue;
		//printf("Tank %d Coord(%d, %d)\n", k, g_Tank[k].x, g_Tank[k].y);
		if (g_Tank[k].x == bullet->x && g_Tank[k].y == bullet->y) {
			msg[0] = DESTROY;
			msg[1] = -1;
			msg[2] = bullet->x;
			msg[3] = bullet->y;
			for (i = 0; i < USER_COUNT; i++) {
				send(hSocket[i], msg, 5, 0);
			}
			return 1;
		}
	}

	if (map_value > 0 && map_value < 1001) { // BARRICADE
		rmap[bullet->y][bullet->x] =
			(map_value > 400) ? (map_value - 400) : 0;
		if (rmap[bullet->y][bullet->x] == 0) {
			msg[0] = DESTROY;
			msg[1] = -1;
			msg[2] = bullet->x;
			msg[3] = bullet->y;
			for (i = 0; i < USER_COUNT; i++) {
				send(hSocket[i], msg, 5, 0);
			}
		}

		return 1;
	}

	return 0;
}

void kill(int x, int y) {
	int i;
	for (i = 0; i < USER_COUNT; i++) {
		if (g_Tank[i].nDirect != DIE) {
			if (g_Tank[i].x == x && g_Tank[i].y == y) {
				printf("Tank %d (%d, %d)\n", i, g_Tank[i].x, g_Tank[i].y);
				g_Tank[i].nDirect = DIE;
			}
		}
	}
}

void collision(struct Bullet *bullet, SOCKET *hSocket, char id) {
	char x, y, i;
	char msg[5];
	x = bullet->x;
	y = bullet->y;
	printf("kill (%d, %d) and ", x, y);
	kill(x, y);
	getOldPosition(*bullet, &x, &y);
	bullet->nDirect = DIE;
	printf("kill (%d, %d)\n", x, y);
	kill(x, y);
	msg[0] = EXPLOSION;
	msg[1] = id;
	msg[2] = x;
	msg[3] = y;
	for (i = 0; i < USER_COUNT; i++) {
		printf("Send...");
		send(hSocket[i], msg, 5, 0);
	}

}