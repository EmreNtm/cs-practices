#include <stdio.h>             //Emre Nitim 17011079    
#include <stdlib.h>     //Gerçek matrisi hayali olarak küçülterek çözüme ulaþtým.Her þeklin içinde öncekinden daha küçük bir hayali matris oluþuyor.
  
char mtr[100][100];
int x,y; //x baþlangýcý , y uzunluðu temsil edecek.

void dortgen(baslangic,uzunluk){ //Baþlangýç: hayali matrisin gerçek matris içindeki baþlangýç koordinatlarý , uzunluk: hayali matrisin kenar uzunluðu.
	int i,j,k;
	
	k = baslangic + (uzunluk + 1) / 2 - 1;                                                    // Baþlangýç koordinatý bu noktanýn koordinatý.
	j = k;                                                                                    // V
	for(i = baslangic; i < baslangic + (uzunluk - 1) / 2; i++){                               // . . * . .
		mtr[i][j] = '*';                                                                      // . * . * .              Eþkenar dörtgen için
		mtr[i][k] = '*';                                                                      // * . . . *     bir kenar uzunluðu 5 olan hayali matris örneði
		j--;                                                                                  // . * . * .
		k++;                                                                                  // . . * . .
	}
	x = baslangic + (uzunluk - 1) / 4 + 1;  //Bir sonraki þekil için gerekli
	y = 2 * (i - x) + 1;                  //baþlangýç ve uzunluk deðerlerini buluyor.
	for(i = baslangic + (uzunluk - 1) / 2; i < baslangic + uzunluk; i++){
		mtr[i][j] = '*';                                                                      // * * * * *
		mtr[i][k] = '*';                                                                      // * . . . *                  Kare için
		j++;                                                                                  // * . . . *    bir kenar uzunluðu 5 olan hayali matris örneði 
		k--;                                                                                  // * . . . *
	}                                                                                         // * * * * *
}

void kare(baslangic,uzunluk){
	int i,j,k;
	
	i = baslangic;
	k = 2 * baslangic + uzunluk - 1;
	i = k - i;
	for(j = baslangic; j < baslangic + uzunluk; j++){
		i = k - i;
		mtr[i][j] = '*';         // i sýrasýyla baslangic ve baslangic + uzunluk - 1 deðerlerini alýyor.
		i = k - i;
		mtr[i][j] = '*';
	}
	j = baslangic;
	k = 2 * baslangic + uzunluk - 1;
	j = k - j;
	for(i = baslangic + 1; i < baslangic + uzunluk - 1; i++){
		j = k - j;
		mtr[i][j] = '*';         // j sýrasýyla baslangic ve baslangic + uzunluk - 1 deðerlerini alýyor.
		j = k - j;
		mtr[i][j] = '*';
	}
	x = baslangic + 1;     //Bir sonraki þekil için gerekli baþlangýç ve uzunluk deðerlerini buluyor.
	y = uzunluk - 2;
}


int main(){
	int i,j,a,m,n;
	printf("M sayisini giriniz:");
	scanf("%d",&m);
	n = 4 * m + 1;
	
	a = 1;
	x = 0;
	y = n;
	
	while(mtr[(n - 1) / 2][(n - 1) / 2] != '*' && mtr[(n - 1) / 2][(n - 1) / 2 - 1] != '*'){
		if(a == 1){
			dortgen(x,y);
			a *= -1;
		}
		else{
			kare(x,y);
			a *= -1;
		}
	}
	
	
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%c ",mtr[i][j]);
		}
		printf("\n");
	}
	
	system("PAUSE");
	return 0;
}
