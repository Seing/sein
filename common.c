#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct Queue_Node {   //큐 구조체
	int value;
	struct Queue_Node *next;
};

struct Queue_Header {   //큐의 헤더와 테일을 가르키는 구조체
	struct Queue_Node *head;
	struct Queue_Node *tail;
};

struct Queue_Header Q_header = { NULL, NULL };  //헤더와 테일이 NULL을 가르키는 구조체 생성

void *mythread(void *arg);   //스레드가 수행할 함수(큐에 값을 넣었다 빼는 작업을 반복)
void Enqueue(struct Queue_Header *q, int value);   //큐에 값 삽입
int* Dequeue(struct Queue_Header *q);   //큐에서 값 추출
int isEmpty(struct Queue_Header *q);   //큐가 비었는지 확인

int main(int argc, char* argv[]) {
	pthread_t p1, p2;

	pthread_create(&p1, NULL, mythread, NULL);   //스레드 생성
	pthread_create(&p2, NULL, mythread, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	if (isEmpty(&Q_header) == 0) {
		printf(" Queue isn't Empty\n");
	}
	return 0;
}

void *mythread(void *arg) {
	int *value = NULL;
	int i;
	for (i = 0; i<1000000; i++) {   //각 스레드가 Enqueue, Dequeue 작업을 for문에 선언한 i값 만큼 반복 수행
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
	struct Queue_Node *New_Node = (struct Queue_Node *)malloc(sizeof(struct Queue_Node));   //새 노드 생성

	if (q->head == NULL) {   //헤더가 NULL이면(큐가 비어있으면)
		New_Node->value = value;   //생성한 노드에 value값을 넣는다
		New_Node->next = NULL;
		q->head = New_Node;   //큐의 헤드가 새로 생성한 노드를 가리키게 한다
	}
	else {
		New_Node->value = value;
		q->tail->next = New_Node;   //테일이 새로 생성한 노드를 가리키게 해서 기존의 노드와 연결시킨다 
	}

	q->tail = New_Node;   //새로 생성한 노드가 테일이 된다
}
int* Dequeue(struct Queue_Header *q) {
	int *returnValue = (int*)malloc(sizeof(int));   //리턴할 값을 저장할 변수 선언

	if (q->head) {
		struct Queue_Node *Temp = q->head;   //새 노드를 생성하여 헤드값을 넣는다
		*returnValue = q->head->value;   //헤드에 있는 값을 returnValue에 넣는다

		if (q->head == q->tail) {
			q->tail = NULL;
		}
		q->head = q->head->next;   //헤드가 다음 노드를 가리키게 한다
		Temp->next = NULL;   //원래 헤드값이 NULL을 가리키게 한다
		free(Temp);   //원래 헤드값을 가지고 있던 Temp를 없앤다

		return returnValue;   //원래 헤드값에 있던 value를 리턴한다
	}
	else {
		return NULL;
	}
}

int isEmpty(struct Queue_Header *q) {
	if (q->head == NULL && q->tail == NULL)
		return TRUE;
	else
		return FALSE;
}