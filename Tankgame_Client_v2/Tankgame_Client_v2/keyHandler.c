#include "stdafx.h"
#include "keyhandler.h"

unsigned int WINAPI handleKey(void *arg) {
	static int nKey;
	static int i;
	static clock_t nCurTime;
	static int nOldMoveTime;
	static int nOldFireTime;
	SOCKET hSocket = (SOCKET)arg;
	char msg[5];

	while (1) {
		nKey = _getch();
		nCurTime = clock();
		switch (nKey) {
			case 32: // SPACEBAR
				if (nCurTime - nOldFireTime >= FIRE_DELAY) {
					nOldFireTime = nCurTime;
					msg[0] = SHOT;
					send(hSocket, msg, 5, 0);
				}
				break;
			case 27: // ESC
				msg[0] = CLIENT_ACTIVE_CLOSE;
				send(hSocket, msg, 5, 0);

				DeleteCriticalSection(&g_cs);
				closesocket(hSocket);
				WSACleanup();
				exit(0);
				break;
		}
		if (nKey == 224) {
			nKey = _getch();
			switch (nKey) {
				case 72: // 위쪽
					if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
						nOldMoveTime = nCurTime;
						msg[0] = TANK_MOVE;
						msg[4] = UP;
						send(hSocket, msg, 5, 0);
					}
					break;
				case 80: // 아래쪽
					if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
						nOldMoveTime = nCurTime;
						msg[0] = TANK_MOVE;
						msg[4] = DOWN;
						send(hSocket, msg, 5, 0);
					}
					break;
				case 75: // 왼쪽
					if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
						nOldMoveTime = nCurTime;
						msg[0] = TANK_MOVE;
						msg[4] = LEFT;
						send(hSocket, msg, 5, 0);
					}
					break;
				case 77: // 오른쪽
					if (nCurTime - nOldMoveTime >= MOVE_DELAY) {
						nOldMoveTime = nCurTime;
						msg[0] = TANK_MOVE;
						msg[4] = RIGHT;
						send(hSocket, msg, 5, 0);
					}
					break;
			}
		}
	}

	return 0;
}