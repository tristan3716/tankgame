#include "main.h"

int stoms(const int num) {
	return num * 1000;
}

/* =======================================================================================
		Initialized Server
			Ip Address	: 127.0.0.1
			Port		: DEFAULT_PORT(46464, main.h)
	--------------------------------------------------------------------------------------- */
void setupServer(SOCKET *hServSock, SOCKADDR_IN *servAdr) {
	WSADATA	wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		handleError("WSAStartup() error!");
		exit(1);
	}

	*hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (*hServSock == INVALID_SOCKET) {
		handleError("socket() error");
		exit(1);
	}

	memset(servAdr, 0, sizeof(servAdr));
	servAdr->sin_family = PF_INET;
	servAdr->sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr->sin_port = htons(DEFAULT_PORT);

	if (bind(*hServSock, (SOCKADDR*)servAdr, sizeof(*servAdr)) == SOCKET_ERROR) {
		handleError("bind() error");
		closesocket(hServSock);
		WSACleanup();
		exit(1);
	}
	printf("Server Established\n"); //DEBUG

	if (listen(*hServSock, 5) == SOCKET_ERROR) {
		handleError("listen() error");
		closesocket(hServSock);
		WSACleanup();
		exit(1);
	}
	printf("Wait Client ...\n"); //DEBUG
}
int main(void){
	SOCKET hServSock, hClntSock[USER_COUNT];
	SOCKADDR_IN servAdr, clntAdr;
	int nSignal;
	int clntAdrSize;
	int i, j;
	char msg[BUF_SIZE];
	time_t nCurTime; 
	int **map;
	HANDLE *hThread;
	DWORD dwThreadID = (DWORD)NULL;

/*
	HANDLE hIOCP;
	SYSTEM_INFO systemInfo;
	int threadCount, i;
	unsigned long threadId;
	HANDLE *hThread;
*/

	//printf("¢Ë¢Æ¡á");
	setupServer(&hServSock, &servAdr);
	clntAdrSize = sizeof(clntAdr);

	/*
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&systemInfo);
	threadCount = systemInfo.dwNumberOfProcessors * 2;
	hThread = (HANDLE *)malloc(threadCount * sizeof(HANDLE));
	for (i = 0; i < threadCount; i++) {
		hThread[i] = CreateThread(NULL, 0, makeThread, &hIOCP, 0, &threadId);
	}
	*/

	while (1) {
		for (i = 0; i < USER_COUNT; i++) {
			hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
			printf("Client %d is conneted\n", i + 1); //DEBUG
			printf("\t(Cid : %d)\n", hClntSock[i]); //DEBUG
			pong(hClntSock[i]);
			msg[0] = DEFAULT_INFORMATION;
			msg[1] = hClntSock[i];
			msg[2] = USER_COUNT;
			msg[3] = i + 1;
			msg[4] = MAP_NUMBER;
			send(hClntSock[i], msg, 4, 0);
			for (j = 0; j < i; j++) {
				msg[0] = JOIN_USER;
				send(hClntSock[j], msg, 1, 0);
			}
		}
		printf("2 Clinet is conneted\n"); //DEBUG
		Sleep(500);
		nSignal = MATCHED;
		for (i = 0; i < USER_COUNT; i++) {
			printf("Sent %d Signal to Client %d\n", nSignal, hClntSock[i]); //DEBUG
			send(hClntSock[i], (char *)&nSignal, sizeof(nSignal), 0);
		}
		nSignal = 0;
		parseMap(&map, MAP_NUMBER);
		printf("Game Start !\n"); //DEBUG

		nCurTime = clock();
		while (TRUE) {
			if (clock() - nCurTime == stoms(10)) { // Condition of Game End
				printf("Game is end\n"); //DEBUG
				nSignal = 1;
				send(hClntSock[i], (char *)&nSignal, sizeof(nSignal), 0);
				for (i = 0; i < USER_COUNT; i++) {
					printf("Sent %d Signal to Client %d\n", nSignal, hClntSock[i]); //DEBUG
				}
				break;
			}
			// Space of Game Progressing

			// Start Receiver Threads
			hThread = (HANDLE *)malloc(sizeof(HANDLE) * USER_COUNT);
			for (i = 0; i < USER_COUNT; i++) {
				hThread[i] = (HANDLE)_beginthreadex(NULL, 0, receiver, &hClntSock[i], 0, (unsigned*)&dwThreadID);
			}
		}
		

		for (i = 0; i < USER_COUNT; i++) {
			closesocket(hClntSock[i]);
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void handleError(const char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	//exit(1);
}