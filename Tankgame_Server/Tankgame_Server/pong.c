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


unsigned int WINAPI receiver(void *arg) { // arg : socket *&hClntSock[i]
	SOCKET hSocket = (SOCKET)arg;
	Packet packet;
	char buf[5];

	while (1) {
		//printf("receiver is spamming ...\n");
		Sleep(33);
		if (recv(hSocket, buf, 5, 0) != SOCKET_ERROR) {

			if (buf[0] == CLIENT_ACTIVE_CLOSE) {
				printf("Client %d send active close ...\n", hSocket);
				closesocket(hSocket);
				return 0;
			}

			packet.signal = buf[0];
			packet.objectId = buf[1];
			packet.x = buf[2];
			packet.y = buf[3];
			packet.nDirect = buf[4];
			packet.cid = hSocket;
			packet.nRecvTime = clock();
			  
			printf("packet Receive from %d (Signal : %d, id : %d, Coord : (%d, %d))\n",
				hSocket, packet.signal, packet.objectId, packet.x, packet.y);

			enQueue(&queue, packet);
		}
	}
}