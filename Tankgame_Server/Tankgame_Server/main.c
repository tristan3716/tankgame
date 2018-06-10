#include "main.h"


int stoms(const int num) {
	return num * 1000;
}

/* =======================================================================================
		Initiate Server
			Ip Address	: 127.0.0.1
			Port		: DEFAULT_PORT(46464, main.h)
	--------------------------------------------------------------------------------------- */
void startServer(SOCKET *hServSock, SOCKADDR_IN *servAdr) {
	WSADATA	wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		handleError("WSAStartup() error!");

	*hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (*hServSock == INVALID_SOCKET)
		handleError("socket() error");

	memset(servAdr, 0, sizeof(servAdr));
	servAdr->sin_family = AF_INET;
	servAdr->sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr->sin_port = htons(DEFAULT_PORT);

	if (bind(*hServSock, (SOCKADDR*)servAdr, sizeof(*servAdr)) == SOCKET_ERROR)
		handleError("bind() error");
	printf("Server Established\n"); //DEBUG

	if (listen(*hServSock, USER_COUNT) == SOCKET_ERROR)
		handleError("listen() error");
	printf("Wait Client ...\n"); //DEBUG
}

int pong(SOCKET hClntSock) {
	int nSignal;
	//time_t nCurTime = clock();
	recv(hClntSock, (char *)&nSignal, sizeof(nSignal), 0);
	send(hClntSock, (char *)&nSignal, sizeof(nSignal), 0);
	//nSignal = clock() - nCurTime;
	recv(hClntSock, (char *)&nSignal, sizeof(nSignal), 0);
	printf("Pong %d ms\n", nSignal);
	return nSignal;
}

int main(void){
	SOCKET hServSock, hClntSock[USER_COUNT];
	SOCKADDR_IN servAdr, clntAdr;
	int nSignal;
	int clntAdrSize;
	int i;
	char msg[BUF_SIZE];

	time_t nCurTime;

	startServer(&hServSock, &servAdr);
	clntAdrSize = sizeof(clntAdr);

	for (i = 0; i<USER_COUNT; i++){
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		printf("Client %d is conneted\n", i+1); //DEBUG
		printf("\t(Cid : %d)\n", hClntSock[i]); //DEBUG
		pong(hClntSock[i]);
	}
	printf("2 Clinet is conneted\n"); //DEBUG

	nSignal = 1;
	for (i = 0; i<USER_COUNT; i++) {
		printf("Sent %d Signal to Client %d\n", nSignal, hClntSock[i]); //DEBUG
		send(hClntSock[i], (char *)&nSignal, sizeof(nSignal), 0);
	}
	nSignal = 0;
	printf("Game Start !\n"); //DEBUG

	nCurTime = clock();
	while(TRUE) {
		if (clock() - nCurTime == stoms(10)) { // Condition of Game End
			printf("Game is end\n"); //DEBUG
			nSignal = 1;
			send(hClntSock[i], (char *)&nSignal, sizeof(nSignal), 0);
			for (i = 0; i<USER_COUNT; i++) {
				printf("Sent %d Signal to Client %d\n", nSignal, hClntSock[i]); //DEBUG
			}
			break;
		}
		// Space of Game Progressing

		//
	}

	for (i = 0; i<USER_COUNT; i++){
		closesocket(hClntSock[i]);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void handleError(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}