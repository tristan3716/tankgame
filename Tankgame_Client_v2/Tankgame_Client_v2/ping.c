#include "stdafx.h"
#include "ping.h"

int g_nPing = 0;
void ping(SOCKET hSocket) {
	static int nSignal;
	static time_t nCurTime;

	nSignal = PING;
	nCurTime = clock();
	srand((unsigned)time(NULL));// DEBUG
	srand((rand() % 300 * 54319) % 300);// DEBUG
	send(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
	Sleep(rand() % 20); // DEBUG
	recv(hSocket, (char *)&nSignal, sizeof(nSignal), 0);
	nSignal = (int)(clock() - nCurTime);
	send(hSocket, (char *)&nSignal, sizeof(nSignal), 0);

	// 0일시 1로 보정
	g_nPing = nSignal ? nSignal : 1;
}


unsigned int WINAPI checkPing(void *arg) {
	static time_t nOldTime;
	SOCKET hSocket = (SOCKET)arg;
	while (1) {
		if (clock() - nOldTime >= 1000) {
			//ping(hSocket);
			nOldTime = clock();
		}
		Sleep(333);
	}
}

unsigned int WINAPI receiver(void *arg) { // socket *&hClntSock[i]
	SOCKET hSocket = (SOCKET)arg;
	Packet packet;
	char buf[5];

	while (1) {
		Sleep(33);
		if (recv(hSocket, buf, 5, 0) != SOCKET_ERROR) {
			packet.signal = buf[0];
			packet.id = buf[1];
			packet.x = buf[2];
			packet.y = buf[3];
			packet.nDirect = buf[4];

			enQueue(&queue, packet);
		}
	}
}