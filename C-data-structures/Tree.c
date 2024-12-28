#include <stdio.h>
#include <stdlib.h>

struct node{
	char veri;
	struct node* left;
	struct node* right;
};

typedef struct node node;

node* createTree(char veri){
	node* T = (node*) malloc(sizeof(node));
	T->veri = veri;
	T->left = NULL;
	T->right = NULL;
	return T;
}

void visit(node* T){
	printf(" %c ", T->veri);
}

void preTraverse(node* T){
	if(T != NULL){
		visit(T);
		preTraverse(T->left);
		preTraverse(T->right);
	} else {
		return;
	}
}

int isEqual(node* T1, node* T2){
	return((!T1 == !T2) || (T1 && T2 && (T1->veri == T2->veri) && isEqual(T1->left, T2->left)
		&& isEqual(T1->right, T2->right)));
}

node* insert(node* T, char veri){
	if(T == NULL) return createTree(veri);
	if(veri < T->veri){
		T->left = insert(T->left, veri);
	} else if (veri > T->veri){
		T->right = insert(T->right, veri);
	}
	return T;
}

node* minValue(node* T){
	node* current = T;
	while(current->left != NULL){
		current = current->left;
	}
	return current;
}

node* del(node* root, char veri){
	if(root == NULL) 
		return root;
	if(veri > root->veri){
		root->right = del(root->right, veri);
	}else if(veri < root->veri){
		root->left = del(root->left, veri);
	}else{
		if(root->left == NULL){
			node* tmp = root->right;
			free(root);
			return tmp;
		} else if(root->right == NULL){
			node* tmp = root->left;
			free(root);
			return tmp;
		}else{
			node* tmp = minValue(root->right);
			root->veri = tmp->veri;
			root->right = del(root->right, tmp->veri);
		}
	}
	return root;
}

int main(){
	
	
	printf("\n");
	system("PAUSE");
	return 0;
}
