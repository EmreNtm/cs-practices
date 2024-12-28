#include <stdio.h>
#include <stdlib.h>
#define MAX 10

struct node{
	int veri;
	struct node* next;
};

typedef struct node node;

struct queue {
	int count;
	node* front;
	node* rear;
};

typedef struct queue queue;

void init(queue* q){
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
}

int isEmpty(queue* q){
	return q->rear == NULL;
}

void enqueue(queue* q, int veri){
	if(q->count < MAX){
		if(isEmpty(q)){
			q->rear = (node*) malloc(sizeof(node));
			q->front = q->rear;
			q->rear->veri = veri;
			q->rear->next = NULL;
			q->count = q->count + 1;
		} else {
			q->rear->next = (node*) malloc(sizeof(node));
			q->rear->next->veri = veri;
			q->rear->next->next = NULL;
			q->rear = q->rear->next;
			q->count = q->count + 1;
		}
	}
}

int dequeue(queue* q){
	node* tmp;
	int tmp2;
	if(!isEmpty(q)){
		tmp = q->front;
		tmp2 = q->front->veri;
		q->front = q->front->next;
		free(tmp);
		q->count = q->count - 1;
		return tmp2;
	}
	return 0;
}

int main(){
	
	
	printf("\n");
	system("PAUSE");
	return 0;
}
