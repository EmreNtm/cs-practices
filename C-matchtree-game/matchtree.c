#include <stdio.h>
#include <stdlib.h>  
#include <time.h>      
#define t 1


//Tablolar arasý gecikme yaratan fonksiyon.
void delay(float saniye){
	float milisaniye = 1000 * saniye;
	
	clock_t baslangic = clock();
	while(clock() < baslangic + milisaniye);
}


//Boþluklara sayýlarý atayan fonksiyon.
void doldur(char mtr[][50], int n, int m){
	int i,j;
	
	srand(time(NULL));
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			if(mtr[i][j] == 0){
				mtr[i][j] = 48 + rand()%4;    //0-9 arasý sayýlar
			}
		}
	}
}


//Zincir gruplarýnýn baþlangýçlarýný bulup, onlarý iþaretleyen fonksiyonu çaðýýran fonksiyon.
void izbul(char mtr[][50], int n, int m, int i, int j, int *skor){
	while((i != n - 1) || (j != m - 1)){
		if((mtr[i][j] != 42) && (mtr[i][j] != 0) && ((mtr[i][j] == mtr[i][j + 1]) || (mtr[i][j] == mtr [i + 1][j]))){
			isaretle(mtr,n,m,i,j,skor);
		}                               //Fonksiyon zincir gruplarýnýn baþýný buluyor ve iþaretleyi
		j++;                            //Çaðýrýp zincirin tüm elemanlarýný iþaretliyor.
		if(j == m){
			j = 0;
			i++;
		}
	}
}


//Zincir gruplarýný iþaretleyen fonksiyon.
int isaretle(char mtr[][50],int n, int m, int i, int j, int *skor){
	if(mtr[i][j] == mtr[i][j + 1]){
		if((mtr[i][j] == mtr[i][j - 1]) && ((mtr[i][j - 1] != mtr[i][j - 2]) && (mtr[i][j - 1] != mtr[i + 1][j - 1]))){
			mtr[i][j - 1] = 42;  (*skor)++;         //Bu if
		}    //42 = * karakteri
		if((mtr[i][j] == mtr[i + 1][j]) && ((mtr[i + 1][j] != mtr[i + 1][j - 1]) && (mtr[i + 1][j] != mtr[i + 2][j]))){
			mtr[i + 1][j] = 42;  (*skor)++;      //Bu if
		}
		mtr[i][j] = 42;  (*skor)++;
		isaretle(mtr,n,m,i,j + 1,skor);
	}
	else if(mtr[i][j] == mtr[i + 1][j]){
		if((mtr[i][j] == mtr[i][j - 1]) && ((mtr[i][j - 1] != mtr[i][j - 2]) && (mtr[i][j - 1] != mtr[i + 1][j - 1]))){
			mtr[i][j - 1] = 42;  (*skor)++;       //Ve bu if dönüþlerde köþede tek kalan ama patlamasý
		}                                         //gereken sayýlarý iþaretliyor.
		mtr[i][j] = 42;  (*skor)++;
		isaretle(mtr,n,m,i + 1,j,skor);
	}
	else if(mtr[i][j] == mtr[i][j - 1]){
		mtr[i][j] = 42;  (*skor)++;
		isaretle(mtr,n,m,i,j - 1,skor);
	}
	else if(mtr[i][j] == mtr[i - 1][j]){
		mtr[i][j] = 42;  (*skor)++;
		isaretle(mtr,n,m,i - 1,j,skor);
	}
	else{
		mtr[i][j] = 42;  (*skor)++;
		return 0;
	}
}


//Ýþaretli elemanlarý yok eden fonksiyon.
void patlat(char mtr[][50], int n, int m){
	int i,j;
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			if(mtr[i][j] == 42){
				mtr[i][j] = 0;
			}
		}
	}
}


//Elemanlarý aþaðý çeken fonksiyon.
void dusur(char mtr[][50], int n, int m){
	int i,j,k,count = 0;
	char tmp;
	
	for(j = 0; j < m; j++){
		for(i = 0; i < n; i++){
			if(mtr[i][j] == 0){
				count++;
			}
		}
		for(k = 0; k < count; k++){
			for(i = n - 1; i > 0; i--){
				if(mtr[i][j] == 0){
					tmp = mtr[i][j];
					mtr[i][j] = mtr[i - 1][j];
					mtr[i - 1][j] = tmp;
				}	
			}
		}
	}
}


//Ýki elemanýn yerini deðiþtiren fonksiyon.
void degistir(char mtr[][50]){
	int satir1,sutun1,satir2,sutun2;
	char tmp;
	
	do{
		printf("\n2 Elemani yer degistiriniz.\n1.Elemanin satiri:");
		scanf("%d",&satir1);
		printf("1.Elemanin sutunu:");
		scanf("%d",&sutun1);
		printf("2.Elemanin satiri:");
		scanf("%d",&satir2);
		printf("2.Elemanin sutunu:");
		scanf("%d",&sutun2);
		printf("\n");
	}while((satir1 == satir2) && (sutun1 == sutun2));
	
	tmp = mtr[satir1 - 1][sutun1 - 1];
	mtr[satir1 - 1][sutun1 - 1] = mtr[satir2 - 1][sutun2 - 1];
	mtr[satir2 - 1][sutun2 - 1] = tmp;
}


//Patlat , düþür ve doldur fonksiyonlarýnýn durana kadar birbirini tekrar etmesini ayarlayan fonksiyon.
//Bu fonksiyon bittiðinde kullanýcý deðiþimi yapar ve sistem tekrar bu döngüye girer.
void surekli_patlat(char mtr[][50], int n, int m, int * skor){
	int ctrl,i,j,full = 1;
	
	izbul(mtr,n,m,0,0,skor);
	ctrl = kontrol(mtr,n,m);
	
	while(ctrl == 1){
		izbul(mtr,n,m,0,0,skor);
		
		ctrl = kontrol(mtr,n,m);
		
		if(ctrl == 1){
			patlat(mtr,n,m);
			delay(t);
			printf("Patlayacak!\n");
			devam_et();
			delay(t);
			yazdir(mtr,n,m);
			printf("\n Patlayan toplam eleman sayisi:%d \n\n",*skor);
		
			dusur(mtr,n,m);
			delay(t);
			printf("Dusecek!\n");
			devam_et();
			delay(t);
			yazdir(mtr,n,m);
			printf("\n Patlayan toplam eleman sayisi:%d \n\n",*skor);
		}
	}
	
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			if(mtr[i][j] == 0){
				full = 0;
			}
		}
	}
	if(full == 0){
		doldur(mtr,n,m);
		delay(t);
		printf("Dolacak!\n");
		devam_et();
		delay(t);
		yazdir(mtr,n,m);
		printf("\n Patlayan toplam eleman sayisi:%d \n\n",*skor);
		surekli_patlat(mtr,n,m,skor);
	}
}


//Tabloda iþaretli eleman bulunuyor mu diye araþtýran fonksiyon.
int kontrol(char mtr[][50], int n, int m){
	int i,j,flag = 0;
	
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			if(mtr[i][j] == 42){
				flag = 1;
			}
		}
	}	
	
	if(flag == 1){
		return 1;
	}
	else{
		return 0;
	}
}


//Matrisi kutular içinde yazdýran fonksiyon.
void yazdir(char mtr[][50], int n, int m){
	int i,j;
	
	printf("     ");
	for(j = 0; j < m; j++){
		if ( j < 9){
			printf("  %d. ",j + 1);
		}
		else{
			printf(" %d. ",j + 1);
		}
	}
	printf("\n");
	printf("     ");
	for(j = 0; j < m; j++){
		printf("_____");
	}
	printf("\n");
	for(i = 0; i < n; i++){
		if ( i < 9){
			printf("  %d. ",i + 1);
		}
		else{
			printf(" %d. ",i + 1);
		}
		for(j = 0; j < m; j++){
			if(mtr[i][j] != 0){
					printf(" |%c| ",mtr[i][j]);
			}
			else{
				printf("  %c  ",mtr[i][j]);
			}
		}
		printf("\n     ");
		for(j = 0; j < m; j++){
			if(mtr[i][j] != 0){
				printf(" --- ");
			}
			else{
				printf("     ");
			}
		}
		printf("\n");
	}
}


//Sonraki matrise geçmeden önce bir tuþ isteyen fonksiyon.
void devam_et(){
	printf("\nDevam etmek icin herhangi bir tusa basin.");
	getch();
	printf("\n\n");
}

int main() {
	int i,j,skor = 0;
	char devam,mtr[50][50];
	int n,m;
	
	printf("Oyun alanini giriniz\n");
	printf("N:");
	scanf("%d",&n);
	printf("M:");
	scanf("%d",&m);
	
	srand(time(NULL));
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			mtr[i][j] = 48 + rand()%4;    //0-9 arasý sayýlar
		}
	}
	
	delay(t);
	system("cls");
	printf("Tablo olusturuluyor!\n");
	delay(t);
	yazdir(mtr,n,m);
	
	surekli_patlat(mtr,n,m,&skor);
	
	devam = 'e';
	
	while((devam == 'e') || (devam == 'E')){
		degistir(mtr);
		
		delay(t);
		printf("Degisecek!\n");
		devam_et();
		delay(t);
		yazdir(mtr,n,m);
		printf("\n Patlayan toplam eleman sayisi:%d\n\n",skor);	
		surekli_patlat(mtr,n,m,&skor);
		
		printf("Devam etmek istiyorsaniz 'e' 'ye basiniz.'");
		devam = getch();
		printf("\n");
	}
	printf("\nOyun bitti.  Skorunuz:%d\n",skor * skor);
	printf("\n");
	system("PAUSE");
	return 0;
}
