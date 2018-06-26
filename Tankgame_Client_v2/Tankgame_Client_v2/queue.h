#ifndef _NET_H
#define _NET_H

typedef struct Packet {
	char signal;
	char id;
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

typedef struct ExplosionPos {
	char id;
	char x;
	char y;
} ExplosionPos;


typedef struct NodeE {
	ExplosionPos pos;
	struct NodeE *next;
} NodeE;

typedef struct ExpQueue {
	NodeE *front;
	NodeE *rear;
	int count;
} ExpQueue;

void initQueue(Queue *queue);
int isEmpty(Queue *queue);
void enQueue(Queue *queue, Packet packet);
BOOL deQueue(Queue *queue, Packet *packet);

void initQueueR(ExpQueue *queue);
int isEmptyR(ExpQueue *queue);
void enQueueR(ExpQueue *queue, ExplosionPos packet);
BOOL deQueueR(ExpQueue *queue, ExplosionPos *packet);

//void showQueue(Queue *queue);

#endif _NET_H