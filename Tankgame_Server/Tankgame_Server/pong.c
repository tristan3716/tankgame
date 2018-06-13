#include "pong.h"

int pong(SOCKET hClntSock) {
	int nSignal;
	//time_t nCurTime = clock();
	recv(hClntSock, (char *)&nSignal, sizeof(nSignal), 0);
	send(hClntSock, (char *)&nSignal, sizeof(nSignal), 0);
	//nSignal = clock() - nCurTime;
	recv(hClntSock, (char *)&nSignal, sizeof(nSignal), 0);
	printf("Pong %d ms\n", nSignal);
	//return nSignal ? nSignal : 1;

	return 0;
}


unsigned int WINAPI checkPing(void *arg) {
	SOCKET hSocket = (SOCKET)arg;
	while (1) {
		pong(hSocket);
		Sleep(11);
	}
}