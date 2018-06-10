#ifndef MAIN_H
#define MAIN_H

#pragma comment(lib, "WS2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef DEBUG
	#define DEBUG
#endif

#define MATCHED 1
#define GAME_END 1

void handleError(char *message);

#endif MAIN_H