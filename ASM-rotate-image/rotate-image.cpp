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
		MOV CX, n		//Uyguladýðým algoritma resme üçgen þeklinde eriþip,
		SHR CX, 1		//deðiþmesi gereken 4 noktayý tek seferde uygun yere koymayý gerektiriyor.
		XOR ESI, ESI	//Bunun için dýþ döngü n/2 , iç döngü ise n-1-2i kere dönmeli.
		MOV EDI, resim	//Dýþ döngü i = 0'dan, iç döngü ise j = i den baþlamalý.
						//(i = ESI/2 ve j = EBX/2) (ESI ve EBX registerlarýný 2þer 2þer artýrdýðým için böyle.)
	L2: PUSH CX
		MOV CX, n
		DEC CX			//CX = n-1-2i
		SUB CX, SI		//SI yý 2 þer 2 þer artýrdýðým için 1 kere çýkarmak yeterli oldu.
		MOV EBX, ESI	//(j = EBX/2 = ESI/2 = i)
	
	L1: PUSH ESI    
		PUSH EBX		//i ve j deðerlerinin kaybolmamasý için gerekli PUSH iþlemleri.

		MOV EAX, DWORD PTR n //Bu 4 satýr ile EAX deðerini, deðiþmesi gereken 1. noktanýn (yani sol üst parça)
		MUL ESI				 //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
	    ADD EAX, EBX		 //(daha sonra bu deðer ESI ya alýnýp [EDI + ESI] þeklinde istenilen noktaya ulaþmak
		PUSH EAX			 //için kullanýlýyor.) (EAX = 2*( i*n + j ) )
							 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		
		MOV EAX, DWORD PTR n //Bu 9 satýr ile EAX deðerini, deðiþmesi gereken 4. noktanýn (yani sol alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu deðer EBX'e alýnýp [EDI + EBX] þeklinde istenilen noktaya ulaþmak
		SUB EAX, EBX		 //için kullanýlýyor.) (EAX = 2*( (n-1-j)*n + i )
		MUL DWORD PTR n		 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		ADD EAX, ESI		 //(EBX ve ESI deðerlerini 2 þer olarak artýrdýðým için 2 kere çýkarmaya gerek yok.)
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX iþlemleri 1. noktayý stack'in en üstüne taþýmak için kullanýlýyor.
		PUSH EDX

		JMP L4
	L0: JMP L1  //LOOP iþlemlerinin zýplama mesafesi yetersiz kaldýðý için oluþturduðum ara zýplama basamaðý
	L3: JMP L2

	L4: MOV EAX, DWORD PTR n //Bu 12 satýr ile EAX deðerini, deðiþmesi gereken 3. noktanýn (yani sað alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu deðer EBX'e alýnýp [EDI + EBX] þeklinde istenilen noktaya ulaþmak
		SUB EAX, ESI		 //için kullanýlýyor.) (EAX = 2*( (n-1-i)*n + (n-1-j) )
		MUL DWORD PTR n      //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		ADD EAX, DWORD PTR n
		ADD EAX, DWORD PTR n
		SUB EAX, 2
		SUB EAX, EBX
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX iþlemleri 1. noktayý stack'in en üstüne taþýmak için kullanýlýyor.
		PUSH EDX
		
		MOV EAX, EBX		 //Bu 9 satýr ile EAX deðerini, deðiþmesi gereken 2. noktanýn (yani sað üst parça)
		MUL DWORD PTR n		 //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
		ADD EAX, DWORD PTR n //(daha sonra bu deðer EBX'e alýnýp [EDI + EBX] þeklinde istenilen noktaya ulaþmak
		ADD EAX, DWORD PTR n //için kullanýlýyor.) (EAX = 2*( j*n + (n-1-i) )
		SUB EAX, 2			 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		SUB EAX, ESI
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX iþlemleri 1. noktayý stack'in en üstüne taþýmak için kullanýlýyor.
		PUSH EDX

		//Gerekli tüm noktalarý aldýk. Buradan itibaren swap iþlemlerine baþlýyoruz

		POP ESI //ESI 1.noktanýn uzaklýk deðerini alýyor.
		POP EBX //EBX 2.noktanýn uzaklýk deðerini alýyor.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu iþlemler ile 1. ve 2.nokta yer deðiþtirmiþ oldu.
		
		POP EBX //EBX 3.noktanýn uzaklýk deðerini alýyor. ESI hala 1.noktada fakat artýk içerisinde 2.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu iþlemler ile 1. ve 3.nokta yer deðiþtirmiþ oldu.
								//Bu iþlemden sonra 1.nokta 3.noktayý, 3.nokta 2.noktayý içeriyor.

		POP EBX //EBX 4.noktanýn uzaklýk deðerini alýyor. ESI hala 1.noktada fakat artýk içerisinde 3.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu iþlemler ile 1. ve 4.nokta yer deðiþtirmiþ oldu.
		
		//Swap iþlemleri bitti. Sonuç olarak:
		//1.noktanýn içerisinde 4.nokta,
		//2.noktanýn içerisinde 1.nokta,
		//3.noktanýn içerisinde 2.nokta,
		//4.noktanýn içerisinde 3.nokta bulunuyor.

		POP EBX 
		POP ESI //deðiþtirdiðimiz EBX ve ESI yý tekrar indis deðerlerine getiriyoruz.
		ADD EBX, 2 //j indisini artýrýyoruz
	    LOOP L0 //L0 üzerinden L1'e zýplýyor.
	    
		ADD ESI, 2 //i indisini artýrýyoruz
		POP CX
	    LOOP L3 //L3 üzerinden L2'ye zýplýyor.
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}

void solaDondur(short n, int resim) {
	//KODUNUZU BURADAN BASLAYARAK YAZINIZ
	//Çözümün saða döndürmekten tek farklý yaný, swap yapýlma sýrasý.
	__asm {
		MOV CX, n		//Uyguladýðým algoritma resme üçgen þeklinde eriþip,
		SHR CX, 1		//deðiþmesi gereken 4 noktayý tek seferde uygun yere koymayý gerektiriyor.
		XOR ESI, ESI	//Bunun için dýþ döngü n/2 , iç döngü ise n-1-2i kere dönmeli.
		MOV EDI, resim  //Dýþ döngü i = 0'dan, iç döngü ise j = i den baþlamalý.
						//(i = ESI/2 ve j = EBX/2) (ESI ve EBX registerlarýný 2þer 2þer artýrdýðým için böyle.)
	L2: PUSH CX
		MOV CX, n		
		DEC CX			//CX = n-1-2i
		SUB CX, SI		//SI yý 2 þer 2 þer artýrdýðým için 1 kere çýkarmak yeterli oldu.
		MOV EBX, ESI	//(j = EBX/2 = ESI/2 = i)

	L1: PUSH ESI
		PUSH EBX		//i ve j deðerlerinin kaybolmamasý için gerekli PUSH iþlemleri.

		MOV EAX, DWORD PTR n //Bu 4 satýr ile EAX deðerini, deðiþmesi gereken 1. noktanýn (yani sol üst parça)
		MUL ESI				 //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
	   	ADD EAX, EBX		 //(daha sonra bu deðer ESI ya alýnýp [EDI + ESI] þeklinde istenilen noktaya ulaþmak
		PUSH EAX			 //için kullanýlýyor.) (EAX = 2*( i*n + j ) )
							 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
			
		MOV EAX, EBX		 //Bu 9 satýr ile EAX deðerini, deðiþmesi gereken 2. noktanýn (yani sað üst parça)
		MUL DWORD PTR n		 //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
		ADD EAX, DWORD PTR n //(daha sonra bu deðer EBX'e alýnýp [EDI + EBX] þeklinde istenilen noktaya ulaþmak
		ADD EAX, DWORD PTR n //için kullanýlýyor.) (EAX = 2*( j*n + (n-1-i) )
		SUB EAX, 2			 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		SUB EAX, ESI
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX iþlemleri 1. noktayý stack'in en üstüne taþýmak için kullanýlýyor.
		PUSH EDX

		JMP L4
	L0: JMP L1 //LOOP iþlemlerinin zýplama mesafesi yetersiz kaldýðý için oluþturduðum ara zýplama basamaðý
	L3: JMP L2

	L4: MOV EAX, DWORD PTR n //Bu 12 satýr ile EAX deðerini, deðiþmesi gereken 3. noktanýn (yani sað alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu deðer EBX'e alýnýp [EDI + EBX] þeklinde istenilen noktaya ulaþmak
		SUB EAX, ESI		 //için kullanýlýyor.) (EAX = 2*( (n-1-i)*n + (n-1-j) )
		MUL DWORD PTR n		 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		ADD EAX, DWORD PTR n
		ADD EAX, DWORD PTR n
		SUB EAX, 2
		SUB EAX, EBX
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX iþlemleri 1. noktayý stack'in en üstüne taþýmak için kullanýlýyor.
		PUSH EDX

		MOV EAX, DWORD PTR n //Bu 9 satýr ile EAX deðerini, deðiþmesi gereken 4. noktanýn (yani sol alt parça)
		ADD EAX, DWORD PTR n //resim dizisinden uzaklýðýna eþitleyip deðerin kaybolmamasý için pushluyoruz.
		SUB EAX, 2			 //(daha sonra bu deðer EBX'e alýnýp [EDI + EBX] þeklinde istenilen noktaya ulaþmak
		SUB EAX, EBX		 //için kullanýlýyor.) (EAX = 2*( (n-1-j)*n + i )
		MUL DWORD PTR n		 //buradaki çarpý 2 dizinin word olmasýndan geliyor.)
		ADD EAX, ESI		 //(EBX ve ESI deðerlerini 2 þer olarak artýrdýðým için 2 kere çýkarmaya gerek yok.)
		POP EDX
		PUSH EAX //Buradaki POP EDX VE PUSH EDX iþlemleri 1. noktayý stack'in en üstüne taþýmak için kullanýlýyor.
		PUSH EDX

		//Gerekli tüm noktalarý aldýk. Buradan itibaren swap iþlemlerine baþlýyoruz

		POP ESI //ESI 1.noktanýn uzaklýk deðerini alýyor.
		POP EBX //EBX 4.noktanýn uzaklýk deðerini alýyor.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu iþlemler ile 1. ve 4.nokta yer deðiþtirmiþ oldu.

		POP EBX //EBX 3.noktanýn uzaklýk deðerini alýyor. ESI hala 1.noktada fakat artýk içerisinde 4.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu iþlemler ile 1. ve 3.nokta yer deðiþtirmiþ oldu.
								//Bu iþlemden sonra 1.nokta 3.noktayý, 3.nokta 4.noktayý içeriyor.

		POP EBX //EBX 2.noktanýn uzaklýk deðerini alýyor. ESI hala 1.noktada fakat artýk içerisinde 3.nokta var.
		PUSH WORD PTR[EDI + ESI]
		PUSH WORD PTR[EDI + EBX]
		POP WORD PTR[EDI + ESI]
		POP WORD PTR[EDI + EBX] //Bu iþlemler ile 1. ve 2.nokta yer deðiþtirmiþ oldu.

		//Swap iþlemleri bitti. Sonuç olarak:
		//1.noktanýn içerisinde 2.nokta,
		//2.noktanýn içerisinde 3.nokta,
		//3.noktanýn içerisinde 4.nokta,
		//4.noktanýn içerisinde 1.nokta bulunuyor.

		POP EBX
		POP ESI //deðiþtirdiðimiz EBX ve ESI yý tekrar indis deðerlerine getiriyoruz.
		ADD EBX, 2 //j indisini artýrýyoruz
		LOOP L0 //L0 üzerinden L1'e zýplýyor

		ADD ESI, 2
		POP CX
		LOOP L3 //L3 üzerinden L2'ye zýplýyor.
	}
	//KODUNUZU YAZMAYI BURADA BITIRINIZ
}
