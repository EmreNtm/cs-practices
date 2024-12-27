#include <stdio.h>
#include <stdlib.h>

/* hashTable'a ayr�lan alan� serbest b�rak�r. */
void destroyTable(char** hashTable, int tableLength) {
	int i;
	for(i = 0; i < tableLength; i++) {
		free(hashTable[i]);
	}
	free(hashTable);
}

/* O(logn) karma��kl���nda �s al�r. */
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

/* Verdi�im dosyan�n keyini bulup hashTablosundaki uygun yere yerle�tiren,
   e�er dosya zaten mevcut ise kullan�c�ya mesaj g�nderen, �devin as�l par�as�. */
void insertIntoTable(char* fileName, char** hashTable, int tableLength) {
	//DOSYANIN KEY�N� BULMA ��LEMLER� BA�LANGICI
	FILE* fp = fopen(fileName, "r");	//Dosyay� a�t�m.
	if (fp == NULL) {
		printf("%s isimli dosya acilamadi!\n", fileName);
		exit(EXIT_FAILURE);
	}
	int R = 53;			//Bir stringin keyini hesaplarken kullanaca��m�z R sabiti.
	unsigned long fileKey = 0;	//Dosyadaki kelimelerin keylerinin toplam�n� tutaca��m de�i�ken.
	
	int wordLength = 0;	//Dosyadan okuyaca��m kelimelerin uzunluklar�n� tutarken kulland���m de�i�ken.
	char* word = (char*) malloc( sizeof(char) * 1 );	//Dosyadan okudu�um kelimeleri tuttu�um de�i�ken.
	char tmpChar = fgetc(fp);	//Dosyadan okudu�um karakterleri tuttu�um de�i�ken.
	
	/* Dosyadan kelimeleri okudu�um ve bu kelimelerin key de�erlerini 
	   dosyan�n key de�erine ekledi�im d�ng�. */
	while (tmpChar != EOF) {
		//Bu karakterler okundu�undan bir kelimeyi okumam tamamlanm��t�r.
		if (tmpChar == ' ' || tmpChar == ',' || tmpChar == '.') {
			word[wordLength] = '\0';	//Karakter dizisini stringe �evirdim.
			fileKey += findKey(word, wordLength, R);	//Kelimenin keyini bulup dosyan�n keyine ekledim.
			free(word);	//Kelimenin i�ini bo�altt�m.
			word = (char*) malloc( sizeof(char) * 1 );	//Bir sonraki alaca��m kelime i�in tekrar adres ald�m.
			wordLength = 0;	//Kelime uzunlu�unu resetledim.
			
			/* Kelimeler aras�ndaki bo�luklar� atlamak i�in 
			   alttaki kod par�as�n� kulland�m. */
			tmpChar = fgetc(fp);
			while (tmpChar == ' ') {
				tmpChar = fgetc(fp);
			}
			
		//newLine karakterini dahil etmeden kelimemi olu�turdu�um k�s�m.
		} else if (tmpChar != '\n'){
			word[wordLength] = tmpChar;	//Kelimeye okudu�um karakteri ekledim.
			wordLength++;	//Kelime uzunlu�unu g�ncelledim.
			word = (char*) realloc( word, sizeof(char) * (wordLength+1) ); //Yeni karakter ekleyebilmek i�in yer a�t�m.	
			
			tmpChar = fgetc(fp);
			
		//newLine karakteri gelmi� ise hi�bir �ey yapma, s�radaki karaktere ge�.
		} else {
			tmpChar = fgetc(fp);
		}
	}
	//printf("%lu\n", fileKey);
	/* DOSYANIN KEY�N� BULMA ��LEMLER� B�T��� */
	
	//D�ng�den ��kt���mda dosyan�n keyini fileKey i�inde elde etmi� oldum.

	/* TABLOYA YERLE�T�RME ��LEMLER� BA�LANGICI */
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
	/* TABLOYA YERLE�T�RME ��LEMLER� B�T��� */
	fclose(fp);
}

int main() {
	
	/* HASH TABLOSUNU YARATMA BA�LANGICI */
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
	/* HASH TABLOSUNU YARATMA B�T��� */
	
	/* SAMPLES'TA YAZAN DOSYALARI KULLANARAK HASHTABLE'I DOLDURMA ��LEMLER� BA�LANGICI */
	FILE *samples = fopen("samples.txt", "r");	//Dosya adlar�n�n yazd��� txt'yi a�ar.
	FILE *fp;	//samples.txt'nin i�indeki dosyalar� a�arken kulland���m file pointer.
	char *fileName = (char*) malloc( sizeof(char) * 1 );	//samples.txt'nin i�indeki dosya isimlerini tuttu�um de�i�ken.
	char tmpChar = ' ';		//fgetc ile kulland���m ge�ici de�i�ken.
	char previousTmpChar;	//Program�n, samples.txt dosyas�n�n en sonunda newLine olsa da olmasa da �al��mas�n� sa�larken kulland���m ge�ici de�i�ken.
	i = 0;	//samples.txt'nin i�indeki dosya isimlerini okurken kulland���m, fileName'in hangi g�z�ne karakter yaz�laca��n� belirten de�i�ken.
	
	//samples.txt'yi okuma d�ng�s�.
	do {
		previousTmpChar = tmpChar;	//En son okudu�um karakteri previousTmpChar i�inde saklad�m.
		tmpChar = fgetc(samples);	//Yeni karakter okudum.
		
		/* E�er '\n' okuduysam, bir dosyan�n ad�n� okumam bitmi�tir. 
		   O dosyay� a��p tabloya yerle�tirmem gereklidir. */
		if (tmpChar == '\n') {
			fileName[i] = '\0';		//fileName karakter dizisini bir stringe d�n��t�rmek i�in sonuna NULL ekledim.
			insertIntoTable(fileName, hashTable, tableLength);	//A�t���m dosyay� �devin algoritmas�na uygun olarak tabloya yerle�tirdim.
			i = 0;					//G�z sayac�n� s�f�rlad�m.
			free(fileName);			//fileName de�i�kenini temizledim.
			fileName = (char*) malloc( sizeof(char) * 1 );	//Bir sonraki dosya ad�n� okurken kullanmak i�in fileName'e yeni bir adres atad�m.
			
		/* E�er EOF okuduysam ve bir �nceki okudu�um de�er '\n' de�il ise, samples.txt dosyas�n�n en sonunda newLine yoktur.
		   Bu durumda normalde son dosyay� a�amayaca��m i�in, bu kontrol sayesinde son dosyay� da a�abilmeyi sa�lad�m.
		   E�er samples.txt dosyas� newLine ile bittiyse bu k�sma gerek kalmaz.
		   Bir �stteki if blo�u ile ayn� i�lemleri yapt�m. */
		} else if (tmpChar == EOF) {	
			if (previousTmpChar != '\n') {
				fileName[i] = '\0';
				insertIntoTable(fileName, hashTable, tableLength);
				i = 0;
				free(fileName);
				fileName = (char*) malloc( sizeof(char) * 1 );
			}
			
		/* E�er '\n' ya da EOF okumad�ysam, okudu�um karakter dosya ad�n�n s�radaki karakteridir.
		   Bu karakteri dosya ad�n�n en sonuna eklerim. */
		} else {
			fileName[i] = tmpChar;	//Dosya ad�na okudu�um karakteri ekledim.
			i++;	//Sayac� bir sonraki g�ze �ektim.
			fileName = (char*) realloc( fileName, sizeof(char) * (i+1) ); //Yeni karakter ekleyebilmek i�in yer a�t�m.
		}
	} while( tmpChar != EOF ); //samples.txt'nin sonuna geldim.
	fclose(samples); //samples.txt'yi kapatt�m.
	/* SAMPLES'TA DOSYALARI KULLANARAK HASHTABLE'I DOLDURMA ��LEMLER� B�T��� */
	
	/* YEN� DOSYA EKLEME ��LEMLER� BA�LANGICI */
	char input[100];
	do {
		printf("\n\nCikis yapmak icin 0 giriniz.\nEklemek istediginiz dosyanin ismini giriniz: ");
		scanf ("%[^\n]%*c", input);
		printf("\n%s girdiniz.\n", input);
		if (input[0] != '0')
			insertIntoTable(input, hashTable, tableLength);
	} while (input[0] != '0');
	/* YEN� DOSYA EKLEME ��LEMLER� B�T��� */
	
	/* YAPILACAKLAR
	 1.Move directory
	 2.Dosya ad�n� tabloya eklerken pathi silme ve dosya ad�n� samples.txt'ye ekleme
	 3.samples.txt olu�turma
	 */
	
	destroyTable(hashTable, tableLength);
	return 0;
}
