#include "stdafx.h"
#include "main.h"


BOOL isMatched(int signal) {
	return signal == MATCHED;
}

BOOL isGameProgressing(int signal) {
	return signal == GAME_END;
}

/* =======================================================================================

Initiate Client
Ip Address	: 127.0.0.1
Port		: DEFAULT_PORT(46464, main.h)

--------------------------------------------------------------------------------------- */
char servAddr[64];
void startClient(SOCKET *hSocket, SOCKADDR_IN *servAdr) {
	WSADATA	wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		handleError("WSAStartup() error!");

	*hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (*hSocket == INVALID_SOCKET)
		handleError("socket() error");

	memset(servAdr, 0, sizeof(*servAdr));
	servAdr->sin_family = AF_INET;
	servAdr->sin_addr.s_addr = inet_addr(servAddr);
	servAdr->sin_port = htons(DEFAULT_PORT);
}

int nMaxUserCount;
int nCurUserCount;
int nCID;
int nMapNumber;

CRITICAL_SECTION g_cs;


int main(int argc, char *argv[]) {
	SOCKET hSocket;
	int i;
	SOCKADDR_IN	servAdr;
	char msg[BUF_SIZE];

	HANDLE hThread[3];
	DWORD dwThreadID = (DWORD)NULL;

	int **map;
	system("mode con cols=81 lines=37");

	if (argc != 2) {
		printf("usage : Client Server_IP\n");
		exit(1);
	}
	strcpy(servAddr, argv[1]);
	g_nScreenFlag = LOADING;
	initQueue(&queue);
	initQueueR(&expQueue);
	InitializeCriticalSection(&g_cs);

	startClient(&hSocket, &servAdr);
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, handleScreen, &map, 0, (unsigned*)&dwThreadID);

	// Try Connect to Server
	while (TRUE) {
		if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
			g_nScreenFlag_Sub = CONNECTION_FAIL;
			continue;
		}
		else {
			ping(hSocket);
			g_nScreenFlag_Sub = CONNECTION_SUCCESS;
			break;
		}
	}
	Sleep(500);
	// Initialize Default Information
	recv(hSocket, msg, 4, 0);
	nMaxUserCount = (int)msg[1];
	nCID = (int)msg[2];
	nCurUserCount = nCID + 1;
	nMapNumber = (int)msg[3];
	g_nScreenFlag_Sub = CONNECTION_FINISH;
	while (g_nScreenFlag != MATCHING) {
		Sleep(333);
	}

	// Wait for Match Making
	while (TRUE) {
		recv(hSocket, msg, 1, 0);
		if (msg[0] == JOIN_USER) {
			nCurUserCount++;
		}
		else if (msg[0] == MATCHED) {
			Sleep(2000);
			g_nScreenFlag_Sub = MATCHING_FINISH;
			break;
		}
	}
	initializedBullet(&g_bullet);
	g_Tank = (struct Tank *)malloc(sizeof(struct Tank) * nMaxUserCount);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, handleKey, (void *)hSocket, 0, (unsigned*)&dwThreadID);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, receiver, (void *)hSocket, 0, (unsigned*)&dwThreadID);

	while (TRUE) {
		Packet tempPacket;
		static BOOL oldPacket;
		ExplosionPos exPos;
		int nCountDefaultSet = 0;

		if (!isEmpty(&queue)) {
			deQueue(&queue, &tempPacket);

			switch (tempPacket.signal) {
				case SET_DEFAULT_POSITION:
					i = tempPacket.id;
					g_Tank[i].id = i;
					g_Tank[i].x = tempPacket.x;
					g_Tank[i].y = tempPacket.y;
					g_Tank[i].oldX[0] = 0;
					g_Tank[i].oldX[1] = 0;
					g_Tank[i].oldY[0] = -1;
					g_Tank[i].oldY[1] = -1;
					g_Tank[i].nDirect = tempPacket.nDirect;

					nCountDefaultSet++;
					break;
				case TANK_MOVE:
					i = tempPacket.id;

					EnterCriticalSection(&g_cs);
					g_Tank[i].x = tempPacket.x;
					g_Tank[i].y = tempPacket.y;
					g_Tank[i].nDirect = tempPacket.nDirect;
					LeaveCriticalSection(&g_cs);
					break;
				case MOVE_BULLET:
					i = tempPacket.id;

					EnterCriticalSection(&g_cs);
					g_bullet[i].x = tempPacket.x;
					g_bullet[i].y = tempPacket.y;
					g_bullet[i].nDirect = tempPacket.nDirect;
					LeaveCriticalSection(&g_cs);
					break;
				case EXPLOSION:
					i = tempPacket.id;

					g_bullet[i].flag[0] = 1;
					g_bullet[i].flag[1] = 1;
					exPos.x = tempPacket.x;
					exPos.y = tempPacket.y;
					exPos.id = i;

					enQueueR(&expQueue, exPos);
					break;
				case DESTROY:
					i = tempPacket.id;

					exPos.x = tempPacket.x;
					exPos.y = tempPacket.y;
					exPos.id = i;

					enQueueR(&expQueue, exPos);
					break;
				case WINNER:
					i = tempPacket.id;

					g_nScreenFlag = WINNER;
					WaitForSingleObject(hThread[0], INFINITE);
					s_print(g_Tank[i].x*2, g_Tank[i].y+1, "@");
					s_print(g_Tank[i].x*2 -2, g_Tank[i].y+2, "WINNER");
					flipScreen();
					_getch();
					DeleteCriticalSection(&g_cs);
					closesocket(hSocket);
					WSACleanup();
					return 0;
				default:
					break;
			}
		}
	}

	return 0;
}

void handleError(char *message) {
	s_print(0, 0, message);
}