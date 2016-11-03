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
};

struct Queue_Header Q_header = { NULL, NULL };

void *mythread(void *arg);
void Enqueue(struct Queue_Header *q, int value);
int* Dequeue(struct Queue_Header *q);
int isEmpty(struct Queue_Header *q);

static volatile int count = 0;
pthread_mutex_t Lock;

int main(int argc, char* argv[]) {
	pthread_t p1, p2;

	pthread_mutex_init(&Lock, NULL);

	pthread_create(&p1, NULL, mythread, NULL);
	pthread_create(&p2, NULL, mythread, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	if (isEmpty(&Q_header) == 0) {
		printf(" Queue isn't Empty\n");
	}

	printf("%d\n", count);
	return 0;
}

void *mythread(void *arg) {
	int *value = NULL;
	int i;
	for (i = 0; i<20000000; i++) {
		pthread_mutex_lock(&Lock);
		count = count + 1;
		pthread_mutex_unlock(&Lock);
		Enqueue(&Q_header, i);
		value = Dequeue(&Q_header);
		if (value) {
			free(value);
		}
		else {
			printf("Queue is empty");
		}
	}
}

void Enqueue(struct Queue_Header *q, int value) {
	struct Queue_Node *New_Node = (struct Queue_Node *)malloc(sizeof(struct Queue_Node));

	pthread_mutex_lock(&Lock);

	if (q->head == NULL) {
		New_Node->value = value;
		New_Node->next = NULL;
		q->head = New_Node;
	}
	else {
		q->tail->next = New_Node;
	}

	q->tail = New_Node;

	pthread_mutex_unlock(&Lock);
}

int* Dequeue(struct Queue_Header *q) {
	int *returnValue = (int*)malloc(sizeof(int));

	pthread_mutex_lock(&Lock);

	if (q->head) {
		struct Queue_Node *Temp = q->head;
		*returnValue = q->head->value;

		if (q->head == q->tail) {
			q->tail = NULL;
		}

		q->head = q->head->next;
		Temp->next = NULL;
		free(Temp);
		pthread_mutex_unlock(&Lock);
		return returnValue;
	}
	else {
		pthread_mutex_unlock(&Lock);
		return NULL;
	}
}

int isEmpty(struct Queue_Header *q) {
	if (q->head == NULL && q->tail == NULL)
		return TRUE;
	else
		return FALSE;
}