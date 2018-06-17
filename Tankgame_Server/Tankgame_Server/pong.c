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


unsigned int WINAPI receiver(void *arg) {
	SOCKET hSocket = (SOCKET)arg;
	Packet packet;
	char buf[5];

	while (1) {
		recv(hSocket, buf, 5, 0);

		packet.signal = buf[0];
		packet.id = (short)&buf[1]; // CID range 2^8 ÃÊ°úÇÔ -> short or int
		packet.x = buf[3];
		packet.y = buf[4];

		enQueue(&queue, packet);
	}
}