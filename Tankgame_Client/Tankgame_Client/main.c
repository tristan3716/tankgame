#include "main.h"

BOOL isMatched(int signal) {
	return signal == MATCHED;
}

BOOL isGameProgressing(int signal) {
	return signal == GAME_END;
}

void GAME(void) {
	printf("Game is Progressing...\n");
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

int ping(SOCKET hSocket) {
	int nSignal = PING;
	time_t nCurTime = clock();
	srand((unsigned)time(NULL));
	srand((rand()%300*5)%300);
	send(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
	Sleep(rand()%300);
	recv(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
	nSignal = (int)(clock() - nCurTime);
	send(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
	printf("Ping %d ms\n", nSignal);
	return nSignal;
	COORD coord;
}

int main(void) {
	SOCKET hSocket;
	int nSignal,i;
	SOCKADDR_IN	servAdr;
	char msg[BUF_SIZE];

	startClient(&hSocket, &servAdr);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		handleError("connect() error!");
	else
		puts("Connected...........\n"); //DEBUG
	ping(hSocket);

	while (TRUE) {
		printf("matching ...\n"); //DEBUG
		recv(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
		printf("Received Signal is %d\n", nSignal); //DEBUG
		if (isMatched(nSignal)) {
			printf("matching FINISH!\n"); //DEBUG
			break;
		}
	}

	while (TRUE) {
		recv(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
		if (isGameProgressing(nSignal)) {
			printf("Game End !\n"); //DEBUG
			break;
		}

		GAME();
	}


	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void handleError(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}