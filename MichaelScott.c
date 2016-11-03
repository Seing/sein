#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct Queue_Node {
	int value;
	struct Queue_Node *next;
};

struct Queue_Header {
	struct Queue_Node *head;
	struct Queue_Node *tail;
	pthread_mutex_t head_Lock;
	pthread_mutex_t tail_Lock;
};

struct Queue_Header Q_header = { NULL, NULL };

void *mythread(void *arg);
void Queue_Init(struct Queue_Header *q);
void Enqueue(struct Queue_Header *q, int value);
int Dequeue(struct Queue_Header *q, int *value);
int isEmpty(struct Queue_Header *q);

static volatile int count = 0;
pthread_mutex_t Lock;

int main(int argc, char* argv[]) {
	pthread_t p1, p2;

	pthread_mutex_init(&Lock, NULL);

	Queue_Init(&Q_header);
	pthread_create(&p1, NULL, mythread, NULL);
	pthread_create(&p2, NULL, mythread, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	if (isEmpty(&Q_header) == 0) {
		printf("\n");
	}

	printf("%d\n", count);
	return 0;
}

void *mythread(void *arg) {
	int value;
	int i;
	for (i = 0; i<20000000; i++) {
		pthread_mutex_lock(&Lock);
		count = count + 1;
		pthread_mutex_unlock(&Lock);
		Enqueue(&Q_header, i);
		Dequeue(&Q_header, &i);
	}
}

void Queue_Init(struct Queue_Header *q) {
	struct Queue_Node *New_Node = (struct Queue_Node *)malloc(sizeof(struct Queue_Node));
	New_Node->next = NULL;
	q->head = q->tail = New_Node;
	pthread_mutex_init(&q->head_Lock, NULL);
	pthread_mutex_init(&q->tail_Lock, NULL);
}

void Enqueue(struct Queue_Header *q, int value) {
	struct Queue_Node *New_Node = (struct Queue_Node *)malloc(sizeof(struct Queue_Node));

	New_Node->value = value;
	New_Node->next = NULL;

	pthread_mutex_lock(&q->tail_Lock);
	q->tail->next = New_Node;
	q->tail = New_Node;
	pthread_mutex_unlock(&q->tail_Lock);
}

int Dequeue(struct Queue_Header *q, int *value) {
	pthread_mutex_lock(&q->head_Lock);
	struct Queue_Node *New_Node = q->head;
	struct Queue_Node *New = New_Node->next;
	if (New == NULL) {
		printf("It is already Empty");
		pthread_mutex_unlock(&q->head_Lock);
		return -1;
	}
	*value = New->value;
	q->head = New;
	pthread_mutex_unlock(&q->head_Lock);
	free(New_Node);
	return 0;
}

int isEmpty(struct Queue_Header *q) {
	if (q->head == NULL && q->tail == NULL)
		return TRUE;
	else
		return FALSE;
}