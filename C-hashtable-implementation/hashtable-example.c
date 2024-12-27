#include <stdio.h>
#include <stdlib.h>

/* hashTable'a ayrýlan alaný serbest býrakýr. */
void destroyTable(char** hashTable, int tableLength) {
	int i;
	for(i = 0; i < tableLength; i++) {
		free(hashTable[i]);
	}
	free(hashTable);
}

/* O(logn) karmaþýklýðýnda üs alýr. */
int power(int a, int b) {
	if (b == 0)
		return 1;
	int tmp = power(a, b/2);
	if (b % 2 == 0)
		return tmp * tmp;
	else
		return a * tmp * tmp;
}

/* Verilen kelimenin keyini bulur. */
int findKey(char* word, int wordLength, int R) {
	int i;
	unsigned long key = 0;
	
	for(i = 0; i < wordLength; i++) {
		key += word[i] * power(R, wordLength-1-i);
	}
	
	//printf("Word: %s, Key:%u\n", word, key);
	return key;
}

/* Hash hesaplar. */
int findHash(int key, int i, int M) {
	int h1 = key % (unsigned long) M;
	int h2 = 1 + (key % (unsigned long) (M-1));
	return ((h1 + i * h2) % M);
}

int isEqualContent(char* fileName1, char* fileName2) {
	FILE* fp1 = fopen(fileName1, "r");
	FILE* fp2 = fopen(fileName2, "r");
	char tmpChar1;
	char tmpChar2;
	
	do {
		tmpChar1 = fgetc(fp1);
		tmpChar2 = fgetc(fp2);
		if (tmpChar1 != tmpChar2) {
			fclose(fp1);
			fclose(fp2);
			return 0;		
		}
	} while (tmpChar1 != EOF && tmpChar2 != EOF);
	
	if (tmpChar1 == tmpChar2) {
		fclose(fp1);
		fclose(fp2);
		return 1;
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}

/* Verdiðim dosyanýn keyini bulup hashTablosundaki uygun yere yerleþtiren,
   eðer dosya zaten mevcut ise kullanýcýya mesaj gönderen, ödevin asýl parçasý. */
void insertIntoTable(char* fileName, char** hashTable, int tableLength) {
	//DOSYANIN KEYÝNÝ BULMA ÝÞLEMLERÝ BAÞLANGICI
	FILE* fp = fopen(fileName, "r");	//Dosyayý açtým.
	if (fp == NULL) {
		printf("%s isimli dosya acilamadi!\n", fileName);
		exit(EXIT_FAILURE);
	}
	int R = 53;			//Bir stringin keyini hesaplarken kullanacaðýmýz R sabiti.
	unsigned long fileKey = 0;	//Dosyadaki kelimelerin keylerinin toplamýný tutacaðým deðiþken.
	
	int wordLength = 0;	//Dosyadan okuyacaðým kelimelerin uzunluklarýný tutarken kullandýðým deðiþken.
	char* word = (char*) malloc( sizeof(char) * 1 );	//Dosyadan okuduðum kelimeleri tuttuðum deðiþken.
	char tmpChar = fgetc(fp);	//Dosyadan okuduðum karakterleri tuttuðum deðiþken.
	
	/* Dosyadan kelimeleri okuduðum ve bu kelimelerin key deðerlerini 
	   dosyanýn key deðerine eklediðim döngü. */
	while (tmpChar != EOF) {
		//Bu karakterler okunduðundan bir kelimeyi okumam tamamlanmýþtýr.
		if (tmpChar == ' ' || tmpChar == ',' || tmpChar == '.') {
			word[wordLength] = '\0';	//Karakter dizisini stringe çevirdim.
			fileKey += findKey(word, wordLength, R);	//Kelimenin keyini bulup dosyanýn keyine ekledim.
			free(word);	//Kelimenin içini boþalttým.
			word = (char*) malloc( sizeof(char) * 1 );	//Bir sonraki alacaðým kelime için tekrar adres aldým.
			wordLength = 0;	//Kelime uzunluðunu resetledim.
			
			/* Kelimeler arasýndaki boþluklarý atlamak için 
			   alttaki kod parçasýný kullandým. */
			tmpChar = fgetc(fp);
			while (tmpChar == ' ') {
				tmpChar = fgetc(fp);
			}
			
		//newLine karakterini dahil etmeden kelimemi oluþturduðum kýsým.
		} else if (tmpChar != '\n'){
			word[wordLength] = tmpChar;	//Kelimeye okuduðum karakteri ekledim.
			wordLength++;	//Kelime uzunluðunu güncelledim.
			word = (char*) realloc( word, sizeof(char) * (wordLength+1) ); //Yeni karakter ekleyebilmek için yer açtým.	
			
			tmpChar = fgetc(fp);
			
		//newLine karakteri gelmiþ ise hiçbir þey yapma, sýradaki karaktere geç.
		} else {
			tmpChar = fgetc(fp);
		}
	}
	//printf("%lu\n", fileKey);
	/* DOSYANIN KEYÝNÝ BULMA ÝÞLEMLERÝ BÝTÝÞÝ */
	
	//Döngüden çýktýðýmda dosyanýn keyini fileKey içinde elde etmiþ oldum.

	/* TABLOYA YERLEÞTÝRME ÝÞLEMLERÝ BAÞLANGICI */
	int j;
	int flag = 0;
	int i = 0;
	int hash = findHash(fileKey, i, tableLength);
	int fileNameLength = 0;
	while (fileName[fileNameLength] != '\0')
		fileNameLength++;
	
	while (flag == 0) {
		if (hashTable[hash][0] == '\0') {
			hashTable[hash] = (char*) realloc( hashTable[hash], sizeof(char*) * (fileNameLength + 1) );
			for (j = 0; j < fileNameLength; j++) {
				hashTable[hash][j] = fileName[j];
			}
			hashTable[hash][fileNameLength] = '\0';
			printf("%s isimli dosya hash tablosunun %d. gozune eklendi.\n", fileName, hash);
			flag = 1;
		} else {
			if (!isEqualContent(fileName, hashTable[hash])) {
				i++;
				hash = findHash(fileKey, i, tableLength);
			} else {
				printf("%s tabloya eklenemedi. Ayni icerige sahip %s adinda bir dosya zaten %d. gozde mevcut!\n", fileName, hashTable[hash], hash);
				flag = 1;
			}
		}
	}
	/* TABLOYA YERLEÞTÝRME ÝÞLEMLERÝ BÝTÝÞÝ */
	fclose(fp);
}

int main() {
	
	/* HASH TABLOSUNU YARATMA BAÞLANGICI */
	int i;
	int tableLength = 11;
	char** hashTable = (char**) malloc( sizeof(char*) * tableLength );
	if (hashTable == NULL) {
		printf("Failed while allocating memory to hashTable!");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < tableLength; i++) {
		hashTable[i] = (char*) malloc( sizeof(char) );
		if (hashTable[i] == NULL) {
			printf("Failed while allocating memory to hashTable!");
			exit(EXIT_FAILURE);
		}
		hashTable[i][0] = '\0';
	}
	/* HASH TABLOSUNU YARATMA BÝTÝÞÝ */
	
	/* SAMPLES'TA YAZAN DOSYALARI KULLANARAK HASHTABLE'I DOLDURMA ÝÞLEMLERÝ BAÞLANGICI */
	FILE *samples = fopen("samples.txt", "r");	//Dosya adlarýnýn yazdýðý txt'yi açar.
	FILE *fp;	//samples.txt'nin içindeki dosyalarý açarken kullandýðým file pointer.
	char *fileName = (char*) malloc( sizeof(char) * 1 );	//samples.txt'nin içindeki dosya isimlerini tuttuðum deðiþken.
	char tmpChar = ' ';		//fgetc ile kullandýðým geçici deðiþken.
	char previousTmpChar;	//Programýn, samples.txt dosyasýnýn en sonunda newLine olsa da olmasa da çalýþmasýný saðlarken kullandýðým geçici deðiþken.
	i = 0;	//samples.txt'nin içindeki dosya isimlerini okurken kullandýðým, fileName'in hangi gözüne karakter yazýlacaðýný belirten deðiþken.
	
	//samples.txt'yi okuma döngüsü.
	do {
		previousTmpChar = tmpChar;	//En son okuduðum karakteri previousTmpChar içinde sakladým.
		tmpChar = fgetc(samples);	//Yeni karakter okudum.
		
		/* Eðer '\n' okuduysam, bir dosyanýn adýný okumam bitmiþtir. 
		   O dosyayý açýp tabloya yerleþtirmem gereklidir. */
		if (tmpChar == '\n') {
			fileName[i] = '\0';		//fileName karakter dizisini bir stringe dönüþtürmek için sonuna NULL ekledim.
			insertIntoTable(fileName, hashTable, tableLength);	//Açtýðým dosyayý ödevin algoritmasýna uygun olarak tabloya yerleþtirdim.
			i = 0;					//Göz sayacýný sýfýrladým.
			free(fileName);			//fileName deðiþkenini temizledim.
			fileName = (char*) malloc( sizeof(char) * 1 );	//Bir sonraki dosya adýný okurken kullanmak için fileName'e yeni bir adres atadým.
			
		/* Eðer EOF okuduysam ve bir önceki okuduðum deðer '\n' deðil ise, samples.txt dosyasýnýn en sonunda newLine yoktur.
		   Bu durumda normalde son dosyayý açamayacaðým için, bu kontrol sayesinde son dosyayý da açabilmeyi saðladým.
		   Eðer samples.txt dosyasý newLine ile bittiyse bu kýsma gerek kalmaz.
		   Bir üstteki if bloðu ile ayný iþlemleri yaptým. */
		} else if (tmpChar == EOF) {	
			if (previousTmpChar != '\n') {
				fileName[i] = '\0';
				insertIntoTable(fileName, hashTable, tableLength);
				i = 0;
				free(fileName);
				fileName = (char*) malloc( sizeof(char) * 1 );
			}
			
		/* Eðer '\n' ya da EOF okumadýysam, okuduðum karakter dosya adýnýn sýradaki karakteridir.
		   Bu karakteri dosya adýnýn en sonuna eklerim. */
		} else {
			fileName[i] = tmpChar;	//Dosya adýna okuduðum karakteri ekledim.
			i++;	//Sayacý bir sonraki göze çektim.
			fileName = (char*) realloc( fileName, sizeof(char) * (i+1) ); //Yeni karakter ekleyebilmek için yer açtým.
		}
	} while( tmpChar != EOF ); //samples.txt'nin sonuna geldim.
	fclose(samples); //samples.txt'yi kapattým.
	/* SAMPLES'TA DOSYALARI KULLANARAK HASHTABLE'I DOLDURMA ÝÞLEMLERÝ BÝTÝÞÝ */
	
	/* YENÝ DOSYA EKLEME ÝÞLEMLERÝ BAÞLANGICI */
	char input[100];
	do {
		printf("\n\nCikis yapmak icin 0 giriniz.\nEklemek istediginiz dosyanin ismini giriniz: ");
		scanf ("%[^\n]%*c", input);
		printf("\n%s girdiniz.\n", input);
		if (input[0] != '0')
			insertIntoTable(input, hashTable, tableLength);
	} while (input[0] != '0');
	/* YENÝ DOSYA EKLEME ÝÞLEMLERÝ BÝTÝÞÝ */
	
	/* YAPILACAKLAR
	 1.Move directory
	 2.Dosya adýný tabloya eklerken pathi silme ve dosya adýný samples.txt'ye ekleme
	 3.samples.txt oluþturma
	 */
	
	destroyTable(hashTable, tableLength);
	return 0;
}
