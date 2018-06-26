#ifndef _MAIN_H
#define _MAIN_H

#pragma comment(lib, "WS2_32.lib")

#include <mmsystem.h>

#include "screen.h"
#include "keyhandler.h"
#include "ping.h"
#include "tank.h"
#include "queue.h"
#include "bullet.h"

#include "signal.h"

#define BUF_SIZE 555

//#define DEBUG

#define GAME_END 1
#define DEFAULT_PORT 46464
//#define PING 102

void handleError(char *message);
void startClient(SOCKET *hSocket, SOCKADDR_IN *servAdr);
void ping(SOCKET hSocket);

extern int g_nScreenFlag_Sub;
extern int g_nScreenFlag;

extern struct Tank *g_Tank;

extern Queue queue;
extern ExpQueue expQueue;
extern struct Bullet *g_bullet;

#endif _MAIN_H