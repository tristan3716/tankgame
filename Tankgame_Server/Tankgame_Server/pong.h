#ifndef _PING_H
#define _PING_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
//#include "main.h"
#include "net.h"

#ifndef BUF_SIZE
	#define BUF_SIZE 1024
#endif

#ifndef FALSE
	#define FALSE (0)
#endif
#ifndef TRUE
	#define TRUE !FALSE
#endif

int pong(SOCKET hSocket);
unsigned int WINAPI receiver(void *arg);

extern Queue queue;

#endif _PING_H