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

int main(void) {
	WSADATA	wsaData;
	SOCKET hSocket;
	char msg[BUF_SIZE];
	int nSignal, opndCnt, i;
	SOCKADDR_IN	servAdr;
	int match_state = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		handleError("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		handleError("socket() error");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(46464);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		handleError("connect() error!");
	else
		puts("Connected...........\n");
	while (TRUE) {
		printf("matching ...\n");
		recv(hSocket, &nSignal, sizeof(nSignal), 0);
		printf("Received Signal is %d\n", nSignal);
		if (isMatched(nSignal)) {
			printf("matching FINISH!\n");
			break;
		}
	}

	while (TRUE) {
		recv(hSocket, &nSignal, sizeof(nSignal), 0);
		if (isGameProgressing(nSignal)) {
			printf("Game End !\n");
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