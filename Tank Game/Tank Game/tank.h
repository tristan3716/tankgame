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
	int nMoveTime; // �̵� ������ // �� ������ �ִ°� ���� ������ �ֳ� (�޸�)
	int nOldMoveTime;
	int nFireTime;
	int nOldFireTime;
	int flag;
	COORD nPos;
	COORD nOldPos;
}; 

void initiateTank(void);
int isMoved(int flag);