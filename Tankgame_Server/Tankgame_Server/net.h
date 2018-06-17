#ifndef _NET_H
#define _NET_H

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct Packet {
	char signal;
	short id;
	char x;
	char y;
	char zero[3];
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