#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "key.h"

extern int hBullet;

unsigned int WINAPI handleKey(void){
	int nKey;
	int i;
	clock_t nCurTime;
	char *pState[4] = { "����", "�Ʒ���", "����", "������" };
	while (1){
		if (_kbhit()){
			nKey = _getch();
			nCurTime = clock();
			switch (nKey){
				case 72: // ����
					if (nCurTime - g_Tank.nOldMoveTime >= MOVE_DELAY){
						if (g_Tank.nDirect != UP) { // ������ȯ (���� ����� �ٸ� ������ �Էµ�)
							g_Tank.nDirect = UP;
							g_Tank.nOldMoveTime = nCurTime; 
							g_Tank.nFlag = TURN; break;
						}
						// �̵� (���� ����� ���� ������ �Էµ�)
						g_Tank.nOldPos = g_Tank.nPos;
						g_Tank.nDirect = UP;
						g_Tank.nPos.Y-=1;
						g_Tank.nOldMoveTime = nCurTime;
						g_Tank.nFlag = MOVED;
					}
					break;
				case 80: // �Ʒ���
					if (nCurTime - g_Tank.nOldMoveTime >= MOVE_DELAY){
						if (g_Tank.nDirect != DOWN) {
							g_Tank.nDirect = DOWN;
							g_Tank.nOldMoveTime = nCurTime;
							g_Tank.nFlag = TURN; break;
						}
						g_Tank.nOldPos = g_Tank.nPos;
						g_Tank.nDirect = DOWN;
						g_Tank.nPos.Y+=1;
						g_Tank.nOldMoveTime = nCurTime;
						g_Tank.nFlag = MOVED;
					}
					break;
				case 75: // ����
					if (nCurTime - g_Tank.nOldMoveTime >= MOVE_DELAY){
						if (g_Tank.nDirect != LEFT) {
							g_Tank.nDirect = LEFT;
							g_Tank.nOldMoveTime = nCurTime;
							g_Tank.nFlag = TURN; break;
						}
						g_Tank.nOldPos = g_Tank.nPos;
						g_Tank.nDirect = LEFT;
						g_Tank.nPos.X-=2;
						g_Tank.nOldMoveTime = nCurTime;
						g_Tank.nFlag = MOVED;
					}
					break;
				case 77: // ������
					if (nCurTime - g_Tank.nOldMoveTime >= MOVE_DELAY){
						if (g_Tank.nDirect != RIGHT) {
							g_Tank.nDirect = RIGHT;
							g_Tank.nOldMoveTime = nCurTime;
							g_Tank.nFlag = TURN; break;
						}
						g_Tank.nOldPos = g_Tank.nPos;
						g_Tank.nDirect = RIGHT;
						g_Tank.nPos.X+=2;
						g_Tank.nOldMoveTime = nCurTime;
						g_Tank.nFlag = MOVED;
					}
					break;
				case ' ': // �����̽���
					hBullet = 1; // �����̽��� ���ȴ��� Ȯ�� (DEBUG)
					if (nCurTime - g_Tank.nOldFireTime >= FIRE_DELAY) {
						for (i = 0; i < 5; i++) {
							if (g_Tank_Bullet[i].nLife == 0) {
								g_Tank_Bullet[i].nLife = 1;
								g_Tank_Bullet[i].nDirect = g_Tank.nDirect;
								g_Tank_Bullet[i].nPos = g_Tank.nPos;
								switch (g_Tank_Bullet[i].nDirect) {
								case UP:
									g_Tank_Bullet[i].nPos.Y -= 1; break;
								case DOWN:
									g_Tank_Bullet[i].nPos.Y += 1; break;
								case LEFT:
									g_Tank_Bullet[i].nPos.X -= 2; break;
								case RIGHT:
									g_Tank_Bullet[i].nPos.X += 2; break;
								}
								g_Tank.nOldFireTime = g_Tank_Bullet[i].nOldMoveTime = nCurTime;
								break;
							}
						}
					}
			}
		}
	}
	return 0;
}