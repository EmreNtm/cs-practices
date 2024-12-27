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
	printf("Ogrenci adi: ---\n");
	printf("Ogrenci numarasi: ---\n");
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
		MOV CX, n		//Uyguladığım algoritma resme üçgen şeklinde erişip,
		SHR CX, 1		//değişmesi gereken 4 noktayı tek seferde uygun yere koymayı gerektiriyor.
		XOR ESI, ESI	//Bunun için dış döngü n/2 , iç döngü ise n-1-2i kere dönmeli.
		MOV EDI, resim	//Dış döngü i = 0'dan, iç döngü ise j = i den başlamalı.
						//(i = ESI/2 ve j = EBX/2) (ESI ve EBX registerlarını 2şer 2şer artırdığım için böyle.)
	L2: PUSH CX
		MOV CX, n
		DEC CX			//CX = n-1-2i
		SUB CX, SI		//SI yı 2 şer 2 şer artırdığım için 1 kere çıkarmak yeterli oldu.
		MOV EBX, ESI	//(j = EBX/2 = ESI/2 = i)
	
	L1: PUSH ESI    
		PUSH EBX		//i ve j değerlerinin kaybolmaması için gerekli PUSH işlemleri.

		MOV EAX, DWORD PTR n //Bu 4 satır ile EAX değerini, değişmesi gereken 1. noktanın (yani sol üst parça)
		MUL ESI				 //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
	    ADD EAX, EBX		 //(daha sonra bu değer ESI ya alınıp [EDI + ESI] şeklinde istenilen noktaya ulaşmak
		PUSH EAX			 //için kullanılıyor.) (EAX = 2*( i*n + j ) )
							 //buradaki çarpı 2 dizinin word olmasından geliyor.)
		
		MOV EAX, DWORD PTR n //Bu 9 satır ile EAX değerini, değişmesi gereken 4. noktanın (yani sol alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu değer EBX'e alınıp [EDI + EBX] şeklinde istenilen noktaya ulaşmak
		SUB EAX, EBX		 //için kullanılıyor.) (EAX = 2*( (n-1-j)*n + i )
		MUL DWORD PTR n		 //buradaki çarpı 2 dizinin word olmasından geliyor.)
		ADD EAX, ESI		 //(EBX ve ESI değerlerini 2 şer olarak artırdığım için 2 kere çıkarmaya gerek yok.)
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX işlemleri 1. noktayı stack'in en üstüne taşımak için kullanılıyor.
		PUSH EDX

		JMP L4
	L0: JMP L1  //LOOP işlemlerinin zıplama mesafesi yetersiz kaldığı için oluşturduğum ara zıplama basamağı
	L3: JMP L2

	L4: MOV EAX, DWORD PTR n //Bu 12 satır ile EAX değerini, değişmesi gereken 3. noktanın (yani sağ alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu değer EBX'e alınıp [EDI + EBX] şeklinde istenilen noktaya ulaşmak
		SUB EAX, ESI		 //için kullanılıyor.) (EAX = 2*( (n-1-i)*n + (n-1-j) )
		MUL DWORD PTR n      //buradaki çarpı 2 dizinin word olmasından geliyor.)
		ADD EAX, DWORD PTR n
		ADD EAX, DWORD PTR n
		SUB EAX, 2
		SUB EAX, EBX
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX işlemleri 1. noktayı stack'in en üstüne taşımak için kullanılıyor.
		PUSH EDX
		
		MOV EAX, EBX		 //Bu 9 satır ile EAX değerini, değişmesi gereken 2. noktanın (yani sağ üst parça)
		MUL DWORD PTR n		 //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
		ADD EAX, DWORD PTR n //(daha sonra bu değer EBX'e alınıp [EDI + EBX] şeklinde istenilen noktaya ulaşmak
		ADD EAX, DWORD PTR n //için kullanılıyor.) (EAX = 2*( j*n + (n-1-i) )
		SUB EAX, 2			 //buradaki çarpı 2 dizinin word olmasından geliyor.)
		SUB EAX, ESI
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX işlemleri 1. noktayı stack'in en üstüne taşımak için kullanılıyor.
		PUSH EDX

		//Gerekli tüm noktaları aldık. Buradan itibaren swap işlemlerine başlıyoruz

		POP ESI //ESI 1.noktanın uzaklık değerini alıyor.
		POP EBX //EBX 2.noktanın uzaklık değerini alıyor.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu işlemler ile 1. ve 2.nokta yer değiştirmiş oldu.
		
		POP EBX //EBX 3.noktanın uzaklık değerini alıyor. ESI hala 1.noktada fakat artık içerisinde 2.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu işlemler ile 1. ve 3.nokta yer değiştirmiş oldu.
								//Bu işlemden sonra 1.nokta 3.noktayı, 3.nokta 2.noktayı içeriyor.

		POP EBX //EBX 4.noktanın uzaklık değerini alıyor. ESI hala 1.noktada fakat artık içerisinde 3.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu işlemler ile 1. ve 4.nokta yer değiştirmiş oldu.
		
		//Swap işlemleri bitti. Sonuç olarak:
		//1.noktanın içerisinde 4.nokta,
		//2.noktanın içerisinde 1.nokta,
		//3.noktanın içerisinde 2.nokta,
		//4.noktanın içerisinde 3.nokta bulunuyor.

		POP EBX 
		POP ESI //değiştirdiğimiz EBX ve ESI yı tekrar indis değerlerine getiriyoruz.
		ADD EBX, 2 //j indisini artırıyoruz
	    LOOP L0 //L0 üzerinden L1'e zıplıyor.
	    
		ADD ESI, 2 //i indisini artırıyoruz
		POP CX
	    LOOP L3 //L3 üzerinden L2'ye zıplıyor.
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}

void solaDondur(short n, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	//Çözümün sağa döndürmekten tek farklı yanı, swap yapılma sırası.
	__asm {
		MOV CX, n		//Uyguladığım algoritma resme üçgen şeklinde erişip,
		SHR CX, 1		//değişmesi gereken 4 noktayı tek seferde uygun yere koymayı gerektiriyor.
		XOR ESI, ESI	//Bunun için dış döngü n/2 , iç döngü ise n-1-2i kere dönmeli.
		MOV EDI, resim  //Dış döngü i = 0'dan, iç döngü ise j = i den başlamalı.
						//(i = ESI/2 ve j = EBX/2) (ESI ve EBX registerlarını 2şer 2şer artırdığım için böyle.)
	L2: PUSH CX
		MOV CX, n		
		DEC CX			//CX = n-1-2i
		SUB CX, SI		//SI yı 2 şer 2 şer artırdığım için 1 kere çıkarmak yeterli oldu.
		MOV EBX, ESI	//(j = EBX/2 = ESI/2 = i)

	L1: PUSH ESI
		PUSH EBX		//i ve j değerlerinin kaybolmaması için gerekli PUSH işlemleri.

		MOV EAX, DWORD PTR n //Bu 4 satır ile EAX değerini, değişmesi gereken 1. noktanın (yani sol üst parça)
		MUL ESI				 //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
	   	ADD EAX, EBX		 //(daha sonra bu değer ESI ya alınıp [EDI + ESI] şeklinde istenilen noktaya ulaşmak
		PUSH EAX			 //için kullanılıyor.) (EAX = 2*( i*n + j ) )
							 //buradaki çarpı 2 dizinin word olmasından geliyor.)
			
		MOV EAX, EBX		 //Bu 9 satır ile EAX değerini, değişmesi gereken 2. noktanın (yani sağ üst parça)
		MUL DWORD PTR n		 //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
		ADD EAX, DWORD PTR n //(daha sonra bu değer EBX'e alınıp [EDI + EBX] şeklinde istenilen noktaya ulaşmak
		ADD EAX, DWORD PTR n //için kullanılıyor.) (EAX = 2*( j*n + (n-1-i) )
		SUB EAX, 2			 //buradaki çarpı 2 dizinin word olmasından geliyor.)
		SUB EAX, ESI
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX işlemleri 1. noktayı stack'in en üstüne taşımak için kullanılıyor.
		PUSH EDX

		JMP L4
	L0: JMP L1 //LOOP işlemlerinin zıplama mesafesi yetersiz kaldığı için oluşturduğum ara zıplama basamağı
	L3: JMP L2

	L4: MOV EAX, DWORD PTR n //Bu 12 satır ile EAX değerini, değişmesi gereken 3. noktanın (yani sağ alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu değer EBX'e alınıp [EDI + EBX] şeklinde istenilen noktaya ulaşmak
		SUB EAX, ESI		 //için kullanılıyor.) (EAX = 2*( (n-1-i)*n + (n-1-j) )
		MUL DWORD PTR n		 //buradaki çarpı 2 dizinin word olmasından geliyor.)
		ADD EAX, DWORD PTR n
		ADD EAX, DWORD PTR n
		SUB EAX, 2
		SUB EAX, EBX
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX işlemleri 1. noktayı stack'in en üstüne taşımak için kullanılıyor.
		PUSH EDX

		MOV EAX, DWORD PTR n //Bu 9 satır ile EAX değerini, değişmesi gereken 4. noktanın (yani sol alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklığına eşitleyip değerin kaybolmaması için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu değer EBX'e alınıp [EDI + EBX] şeklinde istenilen noktaya ulaşmak
		SUB EAX, EBX		 //için kullanılıyor.) (EAX = 2*( (n-1-j)*n + i )
		MUL DWORD PTR n		 //buradaki çarpı 2 dizinin word olmasından geliyor.)
		ADD EAX, ESI		 //(EBX ve ESI değerlerini 2 şer olarak artırdığım için 2 kere çıkarmaya gerek yok.)
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX işlemleri 1. noktayı stack'in en üstüne taşımak için kullanılıyor.
		PUSH EDX

		//Gerekli tüm noktaları aldık. Buradan itibaren swap işlemlerine başlıyoruz

		POP ESI //ESI 1.noktanın uzaklık değerini alıyor.
		POP EBX //EBX 4.noktanın uzaklık değerini alıyor.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu işlemler ile 1. ve 4.nokta yer değiştirmiş oldu.

		POP EBX //EBX 3.noktanın uzaklık değerini alıyor. ESI hala 1.noktada fakat artık içerisinde 4.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu işlemler ile 1. ve 3.nokta yer değiştirmiş oldu.
								//Bu işlemden sonra 1.nokta 3.noktayı, 3.nokta 4.noktayı içeriyor.

		POP EBX //EBX 2.noktanın uzaklık değerini alıyor. ESI hala 1.noktada fakat artık içerisinde 3.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu işlemler ile 1. ve 2.nokta yer değiştirmiş oldu.

		//Swap işlemleri bitti. Sonuç olarak:
		//1.noktanın içerisinde 2.nokta,
		//2.noktanın içerisinde 3.nokta,
		//3.noktanın içerisinde 4.nokta,
		//4.noktanın içerisinde 1.nokta bulunuyor.

		POP EBX
		POP ESI //değiştirdiğimiz EBX ve ESI yı tekrar indis değerlerine getiriyoruz.
		ADD EBX, 2 //j indisini artırıyoruz
		LOOP L0 //L0 üzerinden L1'e zıplıyor

		ADD ESI, 2
		POP CX
		LOOP L3 //L3 üzerinden L2'ye zıplıyor.
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}
