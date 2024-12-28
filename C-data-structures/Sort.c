#include <stdio.h>
#include <stdlib.h>

void selection(int* dizi, int N){
	int minI;
	int min;
	int i, j, tmp;
	
	for(i = 0; i < N - 1; i++){
		min = dizi[i];
		minI = i;
		for(j = i + 1; j < N; j++){
			if(dizi[j] < min){
				min = dizi[j];
				minI = j;
			}
		}
		tmp = dizi[minI];
		dizi[minI] = dizi[i];
		dizi[i] = tmp;
	}	
}

void bubble(int* dizi, int N){
	int i, j, tmp;
	
	for(i = 0; i < N-1; i++){
		for(j = 0; j < N-1; j++){
			if(dizi[j] > dizi[j+1]){
				tmp = dizi[j];
				dizi[j] = dizi[j+1];
				dizi[j+1] = tmp;
			}
		}
	}
}

void insertion(int* dizi, int N){
	int i, j, key;
	
	for(j = 1; j < N; j ++){
		key = dizi[j];
		i = j - 1;
		while(i >= 0 && dizi[i] > key){
			dizi[i+1] = dizi[i];
			dizi[i] = key;
			i--;
		}
	}
}

int main(){
	
	int dizi[] = {9,8,7,6,5,4,3,2,1,-2};
	int i, N = 10;
	
	//selection(dizi, N);
	//bubble(dizi, N);
	insertion(dizi, N);
	
	for(i = 0; i < N; i ++){
		printf(" %d ", dizi[i]);
	}
	
	printf("\n");
	system("PAUSE");
	return 0;
}
