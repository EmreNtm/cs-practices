#include <stdio.h>
#include <stdlib.h>

struct node{
	int veri;
	struct node* next;
}*head;

typedef struct node node;

void create(){
	head = NULL;
}

void add(int veri){
	node* tmp = head;
	if(head == NULL){
		head = (node*) malloc(sizeof(node));
		head->next = NULL;
		head->veri = veri;	
	} else {
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = (node*) malloc(sizeof(node));
		tmp->next->veri = veri;
		tmp->next->next = NULL;
	}
}

void insert(int veri, int v){
	node* tmp2;
	node* tmp = head;
	while(tmp->veri != v){
		tmp = tmp->next;
	}
	tmp2 = tmp->next;
	tmp->next = (node*) malloc(sizeof(node));
	tmp->next->veri = veri;
	tmp->next->next = tmp2;
}

void rmv(int veri){
	node* tmp2;
	node* tmp = head;
	while(tmp->next->veri != veri){
		tmp = tmp->next;
	}
	tmp2 = tmp->next->next;
	free(tmp->next);
	tmp->next = tmp2;
}

void show(){
	node* tmp = head;
	while(tmp->next != NULL){
		printf(" %d ", tmp->veri);
		tmp = tmp->next;
	}
	printf(" %d ", tmp->veri);
}

int main(){
	
	head = NULL;
	
	create();
	add(1);
	add(2);
	add(4);
	insert(3, 2);
	show();
	printf("\n");
	rmv(2);
	show();
	
	printf("\n");
	system("PAUSE");
	return 0;
}
