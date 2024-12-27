#include <stdio.h>             //Emre Nitim 17011079    
#include <stdlib.h>     //Ger�ek matrisi hayali olarak k���lterek ��z�me ula�t�m.Her �eklin i�inde �ncekinden daha k���k bir hayali matris olu�uyor.
  
char mtr[100][100];
int x,y; //x ba�lang�c� , y uzunlu�u temsil edecek.

void dortgen(baslangic,uzunluk){ //Ba�lang��: hayali matrisin ger�ek matris i�indeki ba�lang�� koordinatlar� , uzunluk: hayali matrisin kenar uzunlu�u.
	int i,j,k;
	
	k = baslangic + (uzunluk + 1) / 2 - 1;                                                    // Ba�lang�� koordinat� bu noktan�n koordinat�.
	j = k;                                                                                    // V
	for(i = baslangic; i < baslangic + (uzunluk - 1) / 2; i++){                               // . . * . .
		mtr[i][j] = '*';                                                                      // . * . * .              E�kenar d�rtgen i�in
		mtr[i][k] = '*';                                                                      // * . . . *     bir kenar uzunlu�u 5 olan hayali matris �rne�i
		j--;                                                                                  // . * . * .
		k++;                                                                                  // . . * . .
	}
	x = baslangic + (uzunluk - 1) / 4 + 1;  //Bir sonraki �ekil i�in gerekli
	y = 2 * (i - x) + 1;                  //ba�lang�� ve uzunluk de�erlerini buluyor.
	for(i = baslangic + (uzunluk - 1) / 2; i < baslangic + uzunluk; i++){
		mtr[i][j] = '*';                                                                      // * * * * *
		mtr[i][k] = '*';                                                                      // * . . . *                  Kare i�in
		j++;                                                                                  // * . . . *    bir kenar uzunlu�u 5 olan hayali matris �rne�i 
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
		mtr[i][j] = '*';         // i s�ras�yla baslangic ve baslangic + uzunluk - 1 de�erlerini al�yor.
		i = k - i;
		mtr[i][j] = '*';
	}
	j = baslangic;
	k = 2 * baslangic + uzunluk - 1;
	j = k - j;
	for(i = baslangic + 1; i < baslangic + uzunluk - 1; i++){
		j = k - j;
		mtr[i][j] = '*';         // j s�ras�yla baslangic ve baslangic + uzunluk - 1 de�erlerini al�yor.
		j = k - j;
		mtr[i][j] = '*';
	}
	x = baslangic + 1;     //Bir sonraki �ekil i�in gerekli ba�lang�� ve uzunluk de�erlerini buluyor.
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
