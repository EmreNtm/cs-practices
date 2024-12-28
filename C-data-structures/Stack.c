#include <stdio.h>
#include <stdlib.h>

int MAXSIZE = 10;
int stack[MAXSIZE];
int top = -1;

int isEmpty(){
	if(top == -1)
		return 1;
	return 0;
}

int isFull(){
	if(top == MAXSIZE - 1)
		return 1;
	return 0;
}

int peek(){
	if(!isEmpty())
		return stack[top];
	return 0;
}

void push(int veri){
	if(!isFull()){
		top++;
		stack[top] = veri;
	}
}

int pop(){
	if(!isEmpty()){
		return stack[top];
		top--;
	}
	return 0;
}

int main(){
	
	
	
	printf("\n");
	system("PAUSE");
	return 0;
}
