#include "main.h"

BOOL isMatched(int signal) {
	return signal == MATCHED;
}

BOOL isGameProgressing(int signal) {
	return signal == GAME_END;
}

void GAME(void) {
	//printf("Game is Progressing...\n");
	return;
}

/* =======================================================================================

	Initiate Client
		Ip Address	: 127.0.0.1
		Port		: DEFAULT_PORT(46464, main.h)

--------------------------------------------------------------------------------------- */
void startClient(SOCKET *hSocket, SOCKADDR_IN *servAdr) {
	WSADATA	wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		handleError("WSAStartup() error!");

	*hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (*hSocket == INVALID_SOCKET)
		handleError("socket() error");

	memset(servAdr, 0, sizeof(*servAdr));
	servAdr->sin_family = AF_INET;
	servAdr->sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr->sin_port = htons(DEFAULT_PORT);
}

int nMaxUserCount;
int nCurUserCount;
int nCID;
int nMapNumber;

int main(void) {
	SOCKET hSocket;
	int nSignal,i;
	SOCKADDR_IN	servAdr;
	char msg[BUF_SIZE];
	char cSignal[2];

	HANDLE hThread[3];
	DWORD dwThreadID = (DWORD)NULL;

	int **map;
	system("mode con cols=61 lines=32");
	g_nScreenFlag = LOADING;

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
	recv(hSocket, msg, 5, 0);
	nCID = (int)msg[1];
	nMaxUserCount = (int)msg[2];
	nCurUserCount = (int)msg[3];
	nMapNumber = (int)msg[4];
	g_nScreenFlag_Sub = CONNECTION_FINISH;
	g_nScreenFlag = MATCHING;

	// Wait for Match Making
	while (TRUE) {
		recv(hSocket, cSignal, sizeof(cSignal), 0);
		if (*cSignal == JOIN_USER) {
			nCurUserCount++;
		}
		else if (*cSignal == MATCHED){
			g_nScreenFlag_Sub = MATCHING_FINISH;
			break;
		}
	}
	nCurTankPositions = (COORD *)malloc(sizeof(COORD) * nMaxUserCount);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, handleKey, &hSocket, 0, (unsigned*)&dwThreadID);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, checkPing, &hSocket, 0, (unsigned*)&dwThreadID);

	while (TRUE) {
		recv(hSocket, cSignal, sizeof(cSignal), 0);
		switch (cSignal[0]) {
		case SET_DEFAULT_POSITION:
			recv(hSocket, msg, sizeof(char) * nMaxUserCount * 2, 0);
			for (i = 0; i < nMaxUserCount; i++) {
				nCurTankPositions[i].X = msg[2*i];
				nCurTankPositions[i].X = msg[2*i+1];

			}
			break;
		}

		GAME();
	}


	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void handleError(char *message) {
	s_print(0, 0, message);
	//exit(1);
}