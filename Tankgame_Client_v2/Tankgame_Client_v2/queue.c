#include "stdafx.h"
#include "queue.h"

Queue queue; // extern
ExpQueue expQueue; // extern

void initQueue(Queue *queue) {
	queue->front = queue->rear = NULL;
	queue->count = 0;;
}

int isEmpty(Queue *queue) {
	return queue->count == 0;
}

void enQueue(Queue *queue, Packet packet) {
	Node *now = (Node *)malloc(sizeof(Node));
	now->packet = packet;
	now->next = NULL;

	if (isEmpty(queue)) {
		queue->front = now;
	}
	else {
		queue->rear->next = now;
	}
	queue->rear = now;
	queue->count++;
}

BOOL deQueue(Queue *queue, Packet *packet) {
	Node *now;
	if (isEmpty(queue)) {
		//free(now);
		return 0;
	}
	now = queue->front;
	*packet = now->packet;
	queue->front = now->next;
	queue->count--;
	free(now);
	return 1;
}

void initQueueR(ExpQueue *queue) {
	queue->front = queue->rear = NULL;
	queue->count = 0;;
}

int isEmptyR(ExpQueue *queue) {
	return queue->count == 0;
}

void enQueueR(ExpQueue *queue, ExplosionPos pos) {
	NodeE *now = (NodeE *)malloc(sizeof(NodeE));
	now->pos = pos;
	now->next = NULL;

	if (isEmptyR(queue)) {
		queue->front = now;
	}
	else {
		queue->rear->next = now;
	}
	queue->rear = now;
	queue->count++;
}

BOOL deQueueR(ExpQueue *queue, ExplosionPos *pos) {
	NodeE *now;
	if (isEmptyR(queue)) {
		//free(now);
		return 0;
	}
	now = queue->front;
	*pos = now->pos;
	queue->front = now->next;
	queue->count--;
	free(now);
	return 1;
}