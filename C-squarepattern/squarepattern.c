#include <stdio.h>
#include <stdlib.h>     //Gerçek matrisi hayali olarak küçülterek çözüme ulaştım.Her şeklin içinde öncekinden daha küçük bir hayali matris oluşuyor.
  
char mtr[100][100];
int x,y; //x başlangıcı , y uzunluğu temsil edecek.

void dortgen(baslangic,uzunluk){ //Başlangıç: hayali matrisin gerçek matris içindeki başlangıç koordinatları , uzunluk: hayali matrisin kenar uzunluğu.
	int i,j,k;
	
	k = baslangic + (uzunluk + 1) / 2 - 1;                                                    // Başlangıç koordinatı bu noktanın koordinatı.
	j = k;                                                                                    // V
	for(i = baslangic; i < baslangic + (uzunluk - 1) / 2; i++){                               // . . * . .
		mtr[i][j] = '*';                                                                      // . * . * .              Eşkenar dörtgen için
		mtr[i][k] = '*';                                                                      // * . . . *     bir kenar uzunluğu 5 olan hayali matris örneği
		j--;                                                                                  // . * . * .
		k++;                                                                                  // . . * . .
	}
	x = baslangic + (uzunluk - 1) / 4 + 1;  //Bir sonraki şekil için gerekli
	y = 2 * (i - x) + 1;                  //başlangıç ve uzunluk değerlerini buluyor.
	for(i = baslangic + (uzunluk - 1) / 2; i < baslangic + uzunluk; i++){
		mtr[i][j] = '*';                                                                      // * * * * *
		mtr[i][k] = '*';                                                                      // * . . . *                  Kare için
		j++;                                                                                  // * . . . *    bir kenar uzunluğu 5 olan hayali matris örneği 
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
		mtr[i][j] = '*';         // i sırasıyla baslangic ve baslangic + uzunluk - 1 değerlerini alıyor.
		i = k - i;
		mtr[i][j] = '*';
	}
	j = baslangic;
	k = 2 * baslangic + uzunluk - 1;
	j = k - j;
	for(i = baslangic + 1; i < baslangic + uzunluk - 1; i++){
		j = k - j;
		mtr[i][j] = '*';         // j sırasıyla baslangic ve baslangic + uzunluk - 1 değerlerini alıyor.
		j = k - j;
		mtr[i][j] = '*';
	}
	x = baslangic + 1;     //Bir sonraki şekil için gerekli başlangıç ve uzunluk değerlerini buluyor.
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
