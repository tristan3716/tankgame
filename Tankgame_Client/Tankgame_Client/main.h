#ifndef _MAIN_H
#define _MAIN_H

#pragma comment(lib, "WS2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <time.h>
#include <process.h>
#include <conio.h>

#include "screen.h"
#include "keyhandler.h"
#include "ping.h"
#include "tank.h"

#define BUF_SIZE 555

#define DEBUG

#define GAME_END 1
#define DEFAULT_PORT 46464
//#define PING 102

typedef enum _SIGNAL {
	DEFAULT_INFORMATION = 101, PING = 102, MATCHED = 103, JOIN_USER = 104,
	TANK_MOVE = 111, SHOT = 112,
	SET_DEFAULT_POSITION = 121
} SIGNAL;

void handleError(char *message);
void startClient(SOCKET *hSocket, SOCKADDR_IN *servAdr);
void ping(SOCKET hSocket);

extern int g_nScreenFlag_Sub;
extern int g_nScreenFlag;

extern struct Tank *g_Tank;

#endif _MAIN_H