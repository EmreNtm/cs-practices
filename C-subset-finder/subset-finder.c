#include <stdio.h>
#include <stdlib.h>

int main() {
	int n, i, j, k, temp;
	int psn = 1;      //psn : power set number
	char kume[50][10];
	int tablo[50];
	int sayi = 1;
	
	printf("Kumenin eleman sayisi(Max 50): ");
	scanf("%d",&n);
	for(i = 0; i < n; i++){
		printf("Kumenin %d. elemani: ",i + 1);
		scanf("%s",&kume[i][0]);
	}
	
	for(i = 0; i < n; i++){
		psn *= 2;
	}	

	printf("Alt kume 1: Bos Kume\n");
	for(i = 0; i < psn - 1; i++){
		
		j = n - 1;
		temp = sayi;
		while(temp != 0){
			if(temp % 2 == 0){
				tablo[j] = 0;   //1 den ba�lay�p her d�n��te 1 artan say�n�n her de�erdeki
			}else{              	          //2 bitlik sistemde yaz�m�n� tutan tablonun olu�umu
				tablo[j] = 1;
			}
			j--;
			temp /= 2;
		}
		
		printf("Alt kume %d: ",i + 2);
		for(k = 0; k < n; k++){
			if(tablo[k] == 1){
				printf("%s",kume[k]);
			}
		}
		printf("\n");
		sayi++;
	}
	system("PAUSE");
	return 0;
}
