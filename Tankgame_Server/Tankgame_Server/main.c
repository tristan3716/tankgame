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
		closesocket(*hServSock);
		WSACleanup();
		exit(1);
	}
	printf("Server Established\n"); //DEBUG

	if (listen(*hServSock, 5) == SOCKET_ERROR) {
		handleError("listen() error");
		closesocket(*hServSock);
		WSACleanup();
		exit(1);
	}
	printf("    * Server Ip Address : %s\n", "127.0.0.1");
	printf("    * Server Port : %d\n\n", DEFAULT_PORT);
	printf("Wait Client ...\n"); //DEBUG
}

int getIDfromIndex(int index, int *indexlist) {
	return indexlist[index];
}

int getIndexfromID(int id, int *indexlist) {
	int i;
	for (i = 0; i < USER_COUNT; i++) {
		if(indexlist[i] == id)
			return i;
	}
	exit(1);
}

int main(void){
	SOCKET hServSock, hClntSock[USER_COUNT];
	SOCKADDR_IN servAdr, clntAdr;
	int nSignal;
	int clntAdrSize;
	int i, j, k;
	char msg[BUF_SIZE];
	time_t nCurTime; 
	int **map;
	int *indexlist = NULL;
	HANDLE *hThread;
	DWORD dwThreadID = (DWORD)NULL;

	time_t nStartTime;

	setupServer(&hServSock, &servAdr);
	clntAdrSize = sizeof(clntAdr);

	while (1) {
		indexlist = (int *)malloc(sizeof(int) * USER_COUNT);
		for (i = 0; i < USER_COUNT; i++) {
			hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
			indexlist[i] = hClntSock[i];
			printf("Client %d is conneted\n", i + 1); //DEBUG
			printf("\t(Cid : %d)\n", hClntSock[i]); //DEBUG
			pong(hClntSock[i]);
			msg[0] = DEFAULT_INFORMATION;
			//msg[1] = (short *)&hClntSock[i];
			//memcpy(msg[1], &hClntSock[i], sizeof(int));
			msg[1] = USER_COUNT;
			msg[2] = i;
			msg[3] = MAP_NUMBER;

#ifdef DEBUG
			printf("Send Default Informations.\n");
			printf("Signal : %d\n", msg[0]);
			printf("MaxUserCount : %d, MapNumber : %d\n", msg[1], msg[3]);
#endif
			send(hClntSock[i], msg, 4, 0);
			for (j = 0; j < i; j++) {
				msg[0] = JOIN_USER;
				send(hClntSock[j], msg, 1, 0);
			}
		}
		printf("All Clinet is conneted\n"); //DEBUG
		Sleep(500);
		msg[0] = MATCHED;
		for (i = 0; i < USER_COUNT; i++) {
			printf("Sent %d Signal to Client %d\n", msg[0], hClntSock[i]); //DEBUG
			send(hClntSock[i], msg, 1, 0);
		}
		nSignal = 0;
		parseMap(&map, MAP_NUMBER);
		printf("Game Start !\n"); //DEBUG

		// Initialized Server's Tank Default Information
		g_Tank = (struct Tank *)malloc(sizeof(struct Tank) * USER_COUNT);
		for (i = 0; i < USER_COUNT; i++) {
			g_Tank[i].CID = hClntSock[i];
			for (j = 0; j < MAX_BULLET_COUNT; j++) {
				g_Tank[i].bullet[j].nDirect = DIE;
				g_Tank[i].nCurBulletCount = 0;
			}
			switch (i) {
				case 0:
					g_Tank[i].nDirect = UP;
					g_Tank[i].x = 16;
					g_Tank[i].y = 28; break;
				case 1:
					g_Tank[i].nDirect = DOWN;
					g_Tank[i].x = 4;
					g_Tank[i].y = 3; break;
				case 2:
					g_Tank[i].nDirect = DOWN;
					g_Tank[i].x = 14;
					g_Tank[i].y = 9; break;
				case 3:
					g_Tank[i].nDirect = UP;
					g_Tank[i].x = 22;
					g_Tank[i].y = 18; break;
				case 4:
					g_Tank[i].nDirect = UP;
					g_Tank[i].x = 10;
					g_Tank[i].y = 24; break;
			}
		}
		Packet packet;
		packet.signal = SET_DEFAULT_POSITION;
		packet.objectId = USER_COUNT;
		enQueue(&queue, packet);

		nCurTime = clock();

		printf("malloc hThread\n");
		hThread = (HANDLE *)malloc(sizeof(HANDLE) * USER_COUNT);
		for (i = 0; i < USER_COUNT; i++) {
			printf("Thread %d is started hClntSock : %d\n", i, (SOCKET)(void *)hClntSock[i]);
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, receiver, (void *)hClntSock[i], 0, (unsigned*)&dwThreadID);
		}

		while (TRUE) {
			Packet tempPacket;
			BOOL oldPacket = FALSE;
			int turn = 1;
			int nAliveCount = 0;
			int winnerId;

			// 타임 싱크 요구됨
			// 다음 while문의 시작 시간이 2턴째가 되도록...

			nStartTime = clock();
			while (TRUE) {
				//Sleep(22);
				/* ==================================================
						Execute 1 Tick
				   -------------------------------------------------- */
				//printf("> Execute 1 Tick\n");
				for (i = 0; i < USER_COUNT; i++) {
					if (g_Tank[i].nDirect != DIE) {
						winnerId = i;
						nAliveCount++;
					}
				}
				if (nAliveCount == 1) {
					msg[0] = WINNER;
					msg[1] = winnerId;
					for (i = 0; i < USER_COUNT; i++) {
						send(hClntSock[i], msg, 5, 0);
						printf("Send Winner Packet to %d, id : %d\n", i, msg[1]);
					}
					exit(5555);
				}
				nAliveCount = 0;
				updateBullet();

				for (i = 0; i < USER_COUNT; i++) {
					for (j = 0; j < MAX_BULLET_COUNT; j++) {
						if (g_Tank[i].bullet[j].nDirect != DIE) {
							if (!collisionCheck(&map, &(g_Tank[i].bullet[j]), hClntSock)) {
								msg[0] = MOVE_BULLET;
								msg[1] = i * 5 + j;
								msg[2] = g_Tank[i].bullet[j].x;
								msg[3] = g_Tank[i].bullet[j].y;
								msg[4] = g_Tank[i].bullet[j].nDirect;
								for (k = 0; k < USER_COUNT; k++) {
									send(hClntSock[k], msg, 5, 0);
									//printf("Send Packet to %d > SIG:BULLET, id : %d (%d, %d - %d)\n",
									//	hClntSock[k], msg[1], msg[2], msg[3], msg[4]);
								}
							}
							else { 
								collision(&g_Tank[i].bullet[j], hClntSock, i * 5 + j);
							}
						}
					}
				}
				//printf("%d : %d\n", (int)clock(), (int)nStartTime + TICK_DELAY * turn);
				while ((int)clock() < (int)nStartTime + (TICK_DELAY * turn)) {
					//printf("LOOOOOOOOP\n");
					if (oldPacket == FALSE) {
						//printf("> Old Packet is not exist ...\n");
						if (!deQueue(&queue, &tempPacket)) {
							//printf("> Receive Queue is empty ...\n");
							//Sleep(250);
							continue;
						}
					}
					/* ==================================================
							Calculate Packet
					-------------------------------------------------- */
					//printf("Packet Calculation\n");
					//printf("if : %d < %d (%d)\n", (int)tempPacket.nRecvTime, (int)nStartTime + (TICK_DELAY * turn), (TICK_DELAY * turn));
					//printf(">> DEBUG MESSAGE :\n\tclock() : %lf, nStartTime : %lf\n", (double)clock()/CLOCKS_PER_SEC, (double)nStartTime/CLOCKS_PER_SEC);
					if ((int)tempPacket.nRecvTime < (int)nStartTime + (TICK_DELAY * turn)) {
						//printf("> Old Packet is not exist...\n");
						oldPacket = FALSE;
						switch (tempPacket.signal) {
							case SET_DEFAULT_POSITION:
								printf("send default informations\n");
								for (j = 0; j < USER_COUNT; j++) {
									for (i = 0; i < USER_COUNT; i++) {
										msg[0] = SET_DEFAULT_POSITION;
										msg[1] = i;
										msg[2] = g_Tank[i].x;
										msg[3] = g_Tank[i].y;
										msg[4] = g_Tank[i].nDirect;
										send(hClntSock[j], msg, 5, 0);
										//printf("Send Packet to %d > Signal : %d, id : %d (%d, %d - %d)\n", hClntSock[i], msg[0], msg[1], msg[2], msg[3], msg[4]);
									}
								}
								tempPacket.signal = 0;
								break;
							case TANK_MOVE:
								while (TRUE) {
									int i = getIndexfromID(tempPacket.cid, indexlist);
									if (g_Tank[i].nDirect == DIE)
										break;
									g_Tank[i].nDirect = tempPacket.nDirect;
									/*printf("nDirect : ");
									switch (g_Tank[i].nDirect) {
										case UP:printf("UP\n"); break;
										case DOWN:printf("DOWN\n"); break;
										case LEFT:printf("LEFT\n"); break;
										case RIGHT:printf("RIGHT\n"); break;
									}*/
									if (isPossibleMove(g_Tank[i], map, tempPacket.nDirect)) {
										//printf("Tank %d is Moved %d (%d, %d)",
										//	i, tempPacket.nDirect, g_Tank[i].x, g_Tank[i].y);
										switch (tempPacket.nDirect) {
										case UP:
											g_Tank[i].y--; break;
										case DOWN:
											g_Tank[i].y++; break;
										case LEFT:
											g_Tank[i].x--; break;
										case RIGHT:
											g_Tank[i].x++; break;
										}
										//printf(" -> (%d, %d)\n", g_Tank[i].x, g_Tank[i].y);
										msg[0] = TANK_MOVE;
										msg[1] = i;
										msg[2] = g_Tank[i].x;
										msg[3] = g_Tank[i].y;
										msg[4] = g_Tank[i].nDirect;
										for (j = 0; j < USER_COUNT; j++) {
											send(hClntSock[j], msg, 5, 0);
											//printf("Send Packet to %d > Signal : %d, id : %d (%d, %d - %d)\n",
											//	hClntSock[j], msg[0], msg[1], msg[2], msg[3], msg[4]);
										}
									}
									break;
								}
								tempPacket.signal = 0;
								break;
							case SHOT: // Packet Received
								while (TRUE) {
									int i = getIndexfromID(tempPacket.cid, indexlist);
									if (g_Tank[i].nDirect == DIE)
										break;
									for (j = 0; j < MAX_BULLET_COUNT; j++) {
										if (g_Tank[i].bullet[j].nDirect == DIE) {
											g_Tank[i].bullet[j].nDirect = g_Tank[i].nDirect;
											g_Tank[i].bullet[j].x = g_Tank[i].x;
											g_Tank[i].bullet[j].y = g_Tank[i].y;
											correctionBullet(&g_Tank[i].bullet[j]);
											if (collisionCheck(&map, &(g_Tank[i].bullet[j]), hClntSock)) {
												collision(&g_Tank[i].bullet[j], hClntSock, i * 5 + j);
												continue;
											}
											msg[0] = MOVE_BULLET;
											msg[1] = i * 5 + j;
											msg[2] = g_Tank[i].bullet[j].x;
											msg[3] = g_Tank[i].bullet[j].y;
											msg[4] = g_Tank[i].bullet[j].nDirect;
											for (k = 0; k < USER_COUNT; k++) {
												send(hClntSock[k], msg, 5, 0);
												//printf("Send Packet to %d > SIG:BULLET, id : %d (%d, %d - %d)\n",
												//	hClntSock[k], msg[1], msg[2], msg[3], msg[4]);
											}
											break;
										}
									}

									tempPacket.signal = 0;
									break;
								}

							case 0:
								break;
						} // END OF SWITCH
					}

					/* ==================================================
							Set Old Packet...
							Time Sync to Next Tick
					-------------------------------------------------- */
					else {
						//int num = ((int)clock() - (int)nStartTime + TICK_DELAY * turn)/2;
						oldPacket = TRUE;
						//printf("nCurTime : %d, Next Tick : %d", (int)(time_t)clock(), (int)nStartTime + TICK_DELAY * turn);
						//printf("Screen Thread will be sleep %d", num);
						//Sleep(num);
						//turn++;
						break;
					}
				}
				turn++;
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


BOOL isPossibleMove(struct Tank tank, int **map, int nDirect) {
	int i;
	//printf("Current Position : %d, %d\n",  tank.x, tank.y);
	//printf("%d %d %d\n", map[tank.y - 1][tank.x - 1], map[tank.y - 1][tank.x], map[tank.y - 1][tank.x + 1]);
	//printf("%d %d %d\n", map[tank.y ][tank.x - 1], map[tank.y][tank.x], map[tank.y][tank.x + 1]);
	//printf("%d %d %d\n", map[tank.y + 1][tank.x - 1], map[tank.y + 1][tank.x], map[tank.y + 1][tank.x + 1]);

	if (tank.nDirect == DIE)
		return FALSE;
	switch (nDirect) {
		case UP:
			if (map[tank.y - 1][tank.x] >= 1)
				return FALSE;
			else {
				for (i = 0; i < USER_COUNT; i++) {
					if (g_Tank[i].nDirect != DIE) {
						if (g_Tank[i].x == tank.x && g_Tank[i].y == tank.y - 1) {
							//printf("Meet Tank...\n");
							return FALSE;
						}
					}
				}
			}
			break;
		case DOWN:
			if (map[tank.y + 1][tank.x] >= 1)
				return FALSE;
			else {
				for (i = 0; i < USER_COUNT; i++) {
					if (g_Tank[i].nDirect != DIE) {
						if (g_Tank[i].x == tank.x && g_Tank[i].y == tank.y + 1) {
							//printf("Meet Tank...\n");
							return FALSE;
						}
					}
				}
			}
			break;
		case LEFT:
			if (map[tank.y][tank.x - 1] >= 1)
				return FALSE;
			else {
				for (i = 0; i < USER_COUNT; i++) {
					if (g_Tank[i].nDirect != DIE) {
						if (g_Tank[i].x == tank.x - 1 && g_Tank[i].y == tank.y) {
							//printf("Meet Tank...\n");
							return FALSE;
						}
					}
				}
			}
			break;
		case RIGHT:
			if (map[tank.y][tank.x + 1] >= 1)
				return FALSE;
			else {
				for (i = 0; i < USER_COUNT; i++) {
					if (g_Tank[i].nDirect != DIE) {
						if (g_Tank[i].x == tank.x + 1 && g_Tank[i].y == tank.y) {
							//printf("Meet Tank...\n");
							return FALSE;
						}
					}
				}
			}
			break;
	}
	return TRUE;
}