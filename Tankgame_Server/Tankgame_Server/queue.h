#ifndef _NET_H
#define _NET_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct Packet {
	char signal;
	char objectId;
	char x;
	char y;
	char nDirect;
	int cid;
	time_t nRecvTime;
} Packet;

typedef struct Node {
	Packet packet;
	struct Node *next;
} Node;

typedef struct Queue {
	Node *front;
	Node *rear;
	int count;
} Queue;

void initQueue(Queue *queue);
int isEmpty(Queue *queue);
void enQueue(Queue *queue, Packet packet);
BOOL deQueue(Queue *queue, Packet *packet);

void showQueue(Queue *queue);

#endif _NET_H