#include <stdio.h>
#include <stdlib.h>
#define MAX 10

int queue[MAX];
int front = -1;
int rear = -1;
int size = 0;

int isEmpty(){
	return size <= 0;
}

int isFull(){
	return size == MAX;
}

void enqueue(int veri){
	if(!isFull()){
		if(isEmpty()){
			queue[0] = veri;
			size++;
			front = 0;
			rear = 0;
		} else if(rear == MAX - 1){
			rear = 0;
			queue[0] = veri;
			size++;
		} else {
			rear++;
			queue[rear] = veri;
			size++;
		}
	}
}

int dequeue(){
	if(!isEmpty()){
		if(front == MAX - 1){
			front = 0;
			size--;
			return queue[MAX-1];
		} else {
			front++;
			size--;
			return queue[front-1];
		}
	}
}

int main(){
	
	
	printf("\n");
	system("PAUSE");
	return 0;
}
