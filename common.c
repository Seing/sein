#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

struct Queue_Node {   //ť ����ü
	int value;
	struct Queue_Node *next;
};

struct Queue_Header {   //ť�� ����� ������ ����Ű�� ����ü
	struct Queue_Node *head;
	struct Queue_Node *tail;
};

struct Queue_Header Q_header = { NULL, NULL };  //����� ������ NULL�� ����Ű�� ����ü ����

void *mythread(void *arg);   //�����尡 ������ �Լ�(ť�� ���� �־��� ���� �۾��� �ݺ�)
void Enqueue(struct Queue_Header *q, int value);   //ť�� �� ����
int* Dequeue(struct Queue_Header *q);   //ť���� �� ����
int isEmpty(struct Queue_Header *q);   //ť�� ������� Ȯ��

int main(int argc, char* argv[]) {
	pthread_t p1, p2;

	pthread_create(&p1, NULL, mythread, NULL);   //������ ����
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
	for (i = 0; i<1000000; i++) {   //�� �����尡 Enqueue, Dequeue �۾��� for���� ������ i�� ��ŭ �ݺ� ����
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
	struct Queue_Node *New_Node = (struct Queue_Node *)malloc(sizeof(struct Queue_Node));   //�� ��� ����

	if (q->head == NULL) {   //����� NULL�̸�(ť�� ���������)
		New_Node->value = value;   //������ ��忡 value���� �ִ´�
		New_Node->next = NULL;
		q->head = New_Node;   //ť�� ��尡 ���� ������ ��带 ����Ű�� �Ѵ�
	}
	else {
		New_Node->value = value;
		q->tail->next = New_Node;   //������ ���� ������ ��带 ����Ű�� �ؼ� ������ ���� �����Ų�� 
	}

	q->tail = New_Node;   //���� ������ ��尡 ������ �ȴ�
}
int* Dequeue(struct Queue_Header *q) {
	int *returnValue = (int*)malloc(sizeof(int));   //������ ���� ������ ���� ����

	if (q->head) {
		struct Queue_Node *Temp = q->head;   //�� ��带 �����Ͽ� ��尪�� �ִ´�
		*returnValue = q->head->value;   //��忡 �ִ� ���� returnValue�� �ִ´�

		if (q->head == q->tail) {
			q->tail = NULL;
		}
		q->head = q->head->next;   //��尡 ���� ��带 ����Ű�� �Ѵ�
		Temp->next = NULL;   //���� ��尪�� NULL�� ����Ű�� �Ѵ�
		free(Temp);   //���� ��尪�� ������ �ִ� Temp�� ���ش�

		return returnValue;   //���� ��尪�� �ִ� value�� �����Ѵ�
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