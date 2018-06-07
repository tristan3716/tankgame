#pragma once
#include "main.h"

#define FLAG_NONE 600
#define FLAG_DEFAULT 601
#define FLAG_MOVED 602
#define FLAG_TURN 603
#define FLAG_HOLD 604

typedef enum _DIRECT { UP, DOWN, LEFT, RIGHT } DIRECT;

struct Tank {
	int clientId;
	int nLife;
	int nDirect;
	int nLeftBulletCount;
	int nMoveTime; // 이동 딜레이 // 를 가지고 있는게 성능 영향이 있나 (메모리)
	int nOldMoveTime;
	int nFireTime;
	int nOldFireTime;
	int flag;
	COORD nPos;
	COORD nOldPos;
}; 

void initiateTank(void);
int isMoved(int flag);