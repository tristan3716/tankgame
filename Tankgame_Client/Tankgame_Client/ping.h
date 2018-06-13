#ifndef _PING_H
#define _PING_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <time.h>
#include "main.h"

void ping(SOCKET hSocket);
unsigned int WINAPI checkPing(void *arg);

#endif _PING_H