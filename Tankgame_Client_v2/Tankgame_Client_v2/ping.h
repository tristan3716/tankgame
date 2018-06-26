#ifndef _PING_H
#define _PING_H

#include "main.h"
#include "queue.h"

void ping(SOCKET hSocket);
unsigned int WINAPI checkPing(void *arg);
unsigned int WINAPI receiver(void *arg);

extern Queue queue;

#endif _PING_H