#include <stdio.h>
#include <stdlib.h>

struct node{
	int veri;
	struct node* next;
}*head;

int top = -1;

typedef struct node node;

void create(){
	head = NULL;
}

int isEmpty(){
	if(head == NULL)
		return 1;
	return 0;
}

void push(int veri){
	node* tmp = head;
	if(head == NULL){
		head = (node*) malloc(sizeof(node));
		head->veri = veri;
		head->next = NULL;
		top++;
	} else {
		tmp = (node*) malloc(sizeof(node));
		tmp->next = head;
		tmp->veri = veri;
		head = tmp;
		top++;
	}
}

int pop(){
	int tmp;
	node* tmp2 = head;
	if(!isEmpty()){
		tmp = head->veri;
		tmp2 = tmp2->next;
		free(head);
		head = tmp2;
		top--;
		return tmp;
	}
	return 0;
}

void destroy(){
	node* tmp = head;
	while(tmp->next != NULL){
		head = tmp->next;
		free(tmp);
		tmp = head;
	}
	head = NULL;
	top = -1;
}

int main(){
	
	
	printf("\n");
	system("PAUSE");
	return 0;
}
