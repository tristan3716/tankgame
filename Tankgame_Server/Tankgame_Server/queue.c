#include "queue.h"

Queue queue; // extern

void initQueue(Queue *queue){
	queue->front = queue->rear = NULL;
	queue->count = 0;;
}

int isEmpty(Queue *queue){
	return queue->count == 0;
}

void enQueue(Queue *queue, Packet packet){
	Node *now = (Node *)malloc(sizeof(Node));
	now->packet = packet;
	now->next = NULL;

	if (isEmpty(queue)){
		queue->front = now;     
	}
	else {
		queue->rear->next = now;
	}
	queue->rear = now; 
	queue->count++;
}

BOOL deQueue(Queue *queue, Packet *packet){
	Node *now;
	if (isEmpty(queue)){
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

void showQueue(Queue *queue) {
	struct Node *now;
	int counter = 0;

	now = queue->front;
	if (now == NULL) {
		printf("The queue is empty.\n");
		return;
	}
	while (now != NULL) {
		printf("node=%i, val=%i\n", counter++, now->packet.signal);
		now = (*now).next;
	}
}