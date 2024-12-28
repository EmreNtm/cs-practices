#include <stdio.h>
#include <stdlib.h>

void maxHeapify(int* dizi, int i, int N){
	
	int l = 2*i;
	int r = 2*i + 1;
	
	int max, tmp;
	
	if(l < N && dizi[l] > dizi[i]){
		max = l;
	} else {
		max = i;
	}
	
	if(r <= N && dizi[r] > dizi[max]){
		max = r;
	}
	
	if(max != i){
		tmp = dizi[max];
		dizi[max] = dizi[i];
		dizi[i] = tmp;
		maxHeapify(dizi, max, N);
	}
}

void Build_MaxHeap(int* dizi, int N){
	int i;
	for(i = N/2; i > 0; i--){
		maxHeapify(dizi, i, N);
	}
}

void HeapSort(int* dizi, int N){
	int i, tmp;
	Build_MaxHeap(dizi, N);
	for(i = N; i > 1; i--){
		tmp = dizi[i];
		dizi[i] = dizi[1];
		dizi[1] = tmp;
		maxHeapify(dizi, 1, i-1);
	}
}

int main(){
	
	int dizi[] = {9,8,7,6,5,4,3,2,1,-2};
	int N = 10;
	int i;
	
	HeapSort(dizi, N);
	
	for(i = 0; i < N; i++){
		printf(" %d ", dizi[i]);
	}
	
	printf("\n");
	system("PAUSE");
	return 0;
}
