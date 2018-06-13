#include "keyhandler.h"


unsigned int WINAPI handleKey(void *arg) {
	static int nKey;
	static int i;
	static clock_t nCurTime;
	static int nOldMoveTime;
	static int nOldFireTime;
	SOCKET hSocket = (SOCKET)arg;
	char msg[4];

	while (1) {
		if (_kbhit()) {
			nKey = _getch();
			nCurTime = clock();
			switch (nKey) {
			case 72: // ����
				if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
					nOldMoveTime = nCurTime;
					msg[0] = TANK_MOVE;
					msg[1] = UP;
					send(hSocket, msg, 2, 0);
				}
				break;
			case 80: // �Ʒ���
				if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
					nOldMoveTime = nCurTime;
					msg[0] = TANK_MOVE;
					msg[1] = DOWN;
					send(hSocket, msg, 2, 0);
				}
				break;
			case 75: // ����
				if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
					nOldMoveTime = nCurTime;
					msg[0] = TANK_MOVE;
					msg[1] = LEFT;
					send(hSocket, msg, 2, 0);
				}
				break;
			case 77: // ������
				if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
					nOldMoveTime = nCurTime;
					msg[0] = TANK_MOVE;
					msg[1] = RIGHT;
					send(hSocket, msg, 2, 0);
				}
				break;
			case ' ': // �����̽���
				if (nCurTime - nOldFireTime >= FIRE_DELAY) {
					nOldMoveTime = nCurTime;
					msg[0] = SHOT;
					send(hSocket, msg, 1, 0);
				}
			}
		}
	}
	return 0;
}