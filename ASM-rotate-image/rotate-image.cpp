#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void sagaDondur(short n, int resim);
void solaDondur(short n, int resim);

int main(void) {
	int M, N, Q, i, j, k;
	bool type;
	int efile, islem;
	char resimadi[100], sonek[10];
	printf("Ogrenci adi: Emre Nitim\n");
	printf("Ogrenci numarasi: 17011079\n");
	do {
		printf("Islem yapilacak resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &resimadi);
		system("CLS");
		efile = readImageHeader(resimadi, N, M, Q, type);
	} while (efile > 1);
	printf("%s\n", resimadi);
	int** resim = resimOku(resimadi);

	short *resimdizi;
	resimdizi = (short*) malloc(N*M * sizeof(short));

	for (i = 0; i < N; i++) 
		for (j = 0; j < M; j++) 
			resimdizi[i*N + j] = (short) resim[i][j];

	int resimadres = (int) resimdizi;

	do {
		system("CLS");
		printf("\t     ISLEMLER\n");
		printf("------------------------------------\n");
		printf("1)  Resmi saga dondur\n");
		printf("2)  Resmi sola dondur\n");
		printf("0)  Cikis\n\n");
		printf("\"%s\" yolundaki resim icin yapilacak islemi seciniz\n-> ", resimadi);
		scanf("%d", &islem);
	} while (islem > 2 || islem < 0);

	switch (islem) {
		case 0:
			exit(0);
		case 1:
			sagaDondur(N, resimadres);
			strcpy(sonek, "_sag.pgm");
			break;
		case 2:
			solaDondur(N , resimadres);
			strcpy(sonek, "_sol.pgm");
			break;
		default:
			strcpy(sonek, "_orj.pgm");
			break;
	}

	for (k = 0; k < N * M; k++) {
		j = k % N;
		i = k / N;
		resim[i][j] = (int)resimdizi[k];
	}
	
	string::size_type pos = string(resimadi).find_last_of(".");
	resimadi[pos] = '\0';
	strcat(resimadi, sonek);
	resimYaz(resimadi, resim, N, M, Q);
	printf("\nIslem basariyla tamamlandi :)\n\"%s\" yolunda resim olusturuldu.\n\n", resimadi);
	system("PAUSE");
	return 0;
}

void sagaDondur(short n, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	__asm {
		MOV CX, n		//Uygulad���m algoritma resme ��gen �eklinde eri�ip,
		SHR CX, 1		//de�i�mesi gereken 4 noktay� tek seferde uygun yere koymay� gerektiriyor.
		XOR ESI, ESI	//Bunun i�in d�� d�ng� n/2 , i� d�ng� ise n-1-2i kere d�nmeli.
		MOV EDI, resim	//D�� d�ng� i = 0'dan, i� d�ng� ise j = i den ba�lamal�.
						//(i = ESI/2 ve j = EBX/2) (ESI ve EBX registerlar�n� 2�er 2�er art�rd���m i�in b�yle.)
	L2: PUSH CX
		MOV CX, n
		DEC CX			//CX = n-1-2i
		SUB CX, SI		//SI y� 2 �er 2 �er art�rd���m i�in 1 kere ��karmak yeterli oldu.
		MOV EBX, ESI	//(j = EBX/2 = ESI/2 = i)
	
	L1: PUSH ESI    
		PUSH EBX		//i ve j de�erlerinin kaybolmamas� i�in gerekli PUSH i�lemleri.

		MOV EAX, DWORD PTR n //Bu 4 sat�r ile EAX de�erini, de�i�mesi gereken 1. noktan�n (yani sol �st par�a)
		MUL ESI				 //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
	    ADD EAX, EBX		 //(daha sonra bu de�er ESI ya al�n�p [EDI + ESI] �eklinde istenilen noktaya ula�mak
		PUSH EAX			 //i�in kullan�l�yor.) (EAX = 2*( i*n + j ) )
							 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		
		MOV EAX, DWORD PTR n //Bu 9 sat�r ile EAX de�erini, de�i�mesi gereken 4. noktan�n (yani sol alt par�a)
		ADD EAX, DWORD PTR n //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu de�er EBX'e al�n�p [EDI + EBX] �eklinde istenilen noktaya ula�mak
		SUB EAX, EBX		 //i�in kullan�l�yor.) (EAX = 2*( (n-1-j)*n + i )
		MUL DWORD PTR n		 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		ADD EAX, ESI		 //(EBX ve ESI de�erlerini 2 �er olarak art�rd���m i�in 2 kere ��karmaya gerek yok.)
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX i�lemleri 1. noktay� stack'in en �st�ne ta��mak i�in kullan�l�yor.
		PUSH EDX

		JMP L4
	L0: JMP L1  //LOOP i�lemlerinin z�plama mesafesi yetersiz kald��� i�in olu�turdu�um ara z�plama basama��
	L3: JMP L2

	L4: MOV EAX, DWORD PTR n //Bu 12 sat�r ile EAX de�erini, de�i�mesi gereken 3. noktan�n (yani sa� alt par�a)
		ADD EAX, DWORD PTR n //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu de�er EBX'e al�n�p [EDI + EBX] �eklinde istenilen noktaya ula�mak
		SUB EAX, ESI		 //i�in kullan�l�yor.) (EAX = 2*( (n-1-i)*n + (n-1-j) )
		MUL DWORD PTR n      //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		ADD EAX, DWORD PTR n
		ADD EAX, DWORD PTR n
		SUB EAX, 2
		SUB EAX, EBX
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX i�lemleri 1. noktay� stack'in en �st�ne ta��mak i�in kullan�l�yor.
		PUSH EDX
		
		MOV EAX, EBX		 //Bu 9 sat�r ile EAX de�erini, de�i�mesi gereken 2. noktan�n (yani sa� �st par�a)
		MUL DWORD PTR n		 //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
		ADD EAX, DWORD PTR n //(daha sonra bu de�er EBX'e al�n�p [EDI + EBX] �eklinde istenilen noktaya ula�mak
		ADD EAX, DWORD PTR n //i�in kullan�l�yor.) (EAX = 2*( j*n + (n-1-i) )
		SUB EAX, 2			 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		SUB EAX, ESI
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX i�lemleri 1. noktay� stack'in en �st�ne ta��mak i�in kullan�l�yor.
		PUSH EDX

		//Gerekli t�m noktalar� ald�k. Buradan itibaren swap i�lemlerine ba�l�yoruz

		POP ESI //ESI 1.noktan�n uzakl�k de�erini al�yor.
		POP EBX //EBX 2.noktan�n uzakl�k de�erini al�yor.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu i�lemler ile 1. ve 2.nokta yer de�i�tirmi� oldu.
		
		POP EBX //EBX 3.noktan�n uzakl�k de�erini al�yor. ESI hala 1.noktada fakat art�k i�erisinde 2.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu i�lemler ile 1. ve 3.nokta yer de�i�tirmi� oldu.
								//Bu i�lemden sonra 1.nokta 3.noktay�, 3.nokta 2.noktay� i�eriyor.

		POP EBX //EBX 4.noktan�n uzakl�k de�erini al�yor. ESI hala 1.noktada fakat art�k i�erisinde 3.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu i�lemler ile 1. ve 4.nokta yer de�i�tirmi� oldu.
		
		//Swap i�lemleri bitti. Sonu� olarak:
		//1.noktan�n i�erisinde 4.nokta,
		//2.noktan�n i�erisinde 1.nokta,
		//3.noktan�n i�erisinde 2.nokta,
		//4.noktan�n i�erisinde 3.nokta bulunuyor.

		POP EBX 
		POP ESI //de�i�tirdi�imiz EBX ve ESI y� tekrar indis de�erlerine getiriyoruz.
		ADD EBX, 2 //j indisini art�r�yoruz
	    LOOP L0 //L0 �zerinden L1'e z�pl�yor.
	    
		ADD ESI, 2 //i indisini art�r�yoruz
		POP CX
	    LOOP L3 //L3 �zerinden L2'ye z�pl�yor.
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}

void solaDondur(short n, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	//��z�m�n sa�a d�nd�rmekten tek farkl� yan�, swap yap�lma s�ras�.
	__asm {
		MOV CX, n		//Uygulad���m algoritma resme ��gen �eklinde eri�ip,
		SHR CX, 1		//de�i�mesi gereken 4 noktay� tek seferde uygun yere koymay� gerektiriyor.
		XOR ESI, ESI	//Bunun i�in d�� d�ng� n/2 , i� d�ng� ise n-1-2i kere d�nmeli.
		MOV EDI, resim  //D�� d�ng� i = 0'dan, i� d�ng� ise j = i den ba�lamal�.
						//(i = ESI/2 ve j = EBX/2) (ESI ve EBX registerlar�n� 2�er 2�er art�rd���m i�in b�yle.)
	L2: PUSH CX
		MOV CX, n		
		DEC CX			//CX = n-1-2i
		SUB CX, SI		//SI y� 2 �er 2 �er art�rd���m i�in 1 kere ��karmak yeterli oldu.
		MOV EBX, ESI	//(j = EBX/2 = ESI/2 = i)

	L1: PUSH ESI
		PUSH EBX		//i ve j de�erlerinin kaybolmamas� i�in gerekli PUSH i�lemleri.

		MOV EAX, DWORD PTR n //Bu 4 sat�r ile EAX de�erini, de�i�mesi gereken 1. noktan�n (yani sol �st par�a)
		MUL ESI				 //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
	   	ADD EAX, EBX		 //(daha sonra bu de�er ESI ya al�n�p [EDI + ESI] �eklinde istenilen noktaya ula�mak
		PUSH EAX			 //i�in kullan�l�yor.) (EAX = 2*( i*n + j ) )
							 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
			
		MOV EAX, EBX		 //Bu 9 sat�r ile EAX de�erini, de�i�mesi gereken 2. noktan�n (yani sa� �st par�a)
		MUL DWORD PTR n		 //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
		ADD EAX, DWORD PTR n //(daha sonra bu de�er EBX'e al�n�p [EDI + EBX] �eklinde istenilen noktaya ula�mak
		ADD EAX, DWORD PTR n //i�in kullan�l�yor.) (EAX = 2*( j*n + (n-1-i) )
		SUB EAX, 2			 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		SUB EAX, ESI
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX i�lemleri 1. noktay� stack'in en �st�ne ta��mak i�in kullan�l�yor.
		PUSH EDX

		JMP L4
	L0: JMP L1 //LOOP i�lemlerinin z�plama mesafesi yetersiz kald��� i�in olu�turdu�um ara z�plama basama��
	L3: JMP L2

	L4: MOV EAX, DWORD PTR n //Bu 12 sat�r ile EAX de�erini, de�i�mesi gereken 3. noktan�n (yani sa� alt par�a)
		ADD EAX, DWORD PTR n //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu de�er EBX'e al�n�p [EDI + EBX] �eklinde istenilen noktaya ula�mak
		SUB EAX, ESI		 //i�in kullan�l�yor.) (EAX = 2*( (n-1-i)*n + (n-1-j) )
		MUL DWORD PTR n		 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		ADD EAX, DWORD PTR n
		ADD EAX, DWORD PTR n
		SUB EAX, 2
		SUB EAX, EBX
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX i�lemleri 1. noktay� stack'in en �st�ne ta��mak i�in kullan�l�yor.
		PUSH EDX

		MOV EAX, DWORD PTR n //Bu 9 sat�r ile EAX de�erini, de�i�mesi gereken 4. noktan�n (yani sol alt par�a)
		ADD EAX, DWORD PTR n //resim dizisinden uzakl���na e�itleyip de�erin kaybolmamas� i�in pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu de�er EBX'e al�n�p [EDI + EBX] �eklinde istenilen noktaya ula�mak
		SUB EAX, EBX		 //i�in kullan�l�yor.) (EAX = 2*( (n-1-j)*n + i )
		MUL DWORD PTR n		 //buradaki �arp� 2 dizinin word olmas�ndan geliyor.)
		ADD EAX, ESI		 //(EBX ve ESI de�erlerini 2 �er olarak art�rd���m i�in 2 kere ��karmaya gerek yok.)
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX i�lemleri 1. noktay� stack'in en �st�ne ta��mak i�in kullan�l�yor.
		PUSH EDX

		//Gerekli t�m noktalar� ald�k. Buradan itibaren swap i�lemlerine ba�l�yoruz

		POP ESI //ESI 1.noktan�n uzakl�k de�erini al�yor.
		POP EBX //EBX 4.noktan�n uzakl�k de�erini al�yor.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu i�lemler ile 1. ve 4.nokta yer de�i�tirmi� oldu.

		POP EBX //EBX 3.noktan�n uzakl�k de�erini al�yor. ESI hala 1.noktada fakat art�k i�erisinde 4.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu i�lemler ile 1. ve 3.nokta yer de�i�tirmi� oldu.
								//Bu i�lemden sonra 1.nokta 3.noktay�, 3.nokta 4.noktay� i�eriyor.

		POP EBX //EBX 2.noktan�n uzakl�k de�erini al�yor. ESI hala 1.noktada fakat art�k i�erisinde 3.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu i�lemler ile 1. ve 2.nokta yer de�i�tirmi� oldu.

		//Swap i�lemleri bitti. Sonu� olarak:
		//1.noktan�n i�erisinde 2.nokta,
		//2.noktan�n i�erisinde 3.nokta,
		//3.noktan�n i�erisinde 4.nokta,
		//4.noktan�n i�erisinde 1.nokta bulunuyor.

		POP EBX
		POP ESI //de�i�tirdi�imiz EBX ve ESI y� tekrar indis de�erlerine getiriyoruz.
		ADD EBX, 2 //j indisini art�r�yoruz
		LOOP L0 //L0 �zerinden L1'e z�pl�yor

		ADD ESI, 2
		POP CX
		LOOP L3 //L3 �zerinden L2'ye z�pl�yor.
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}
