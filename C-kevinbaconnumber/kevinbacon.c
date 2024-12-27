#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 2147483647
/* Emre Nitim 17011079 */

/* Filmleri tuttuðum graf düðümü */
struct movie {
	char* name;
	/* Filmde oynayan aktörler */
	struct actor** actors;
	int actorsSize;
};

typedef struct movie movie;

struct actor {
	char* name;
	/* Aktörün oynadýðý filmler */
	movie** movies;
	int moviesSize;
	/* next sadece hashMap'te Seperate Chaining için kullanýldý. */
	struct actor* next;
	/* qNext sadece queue için kullanýldý. */
	struct actor* qNext;
	/* BFS uzaklýðý */
	int distance;
	/* BFS'de bu aktörü queue'ya sokan film */
	movie* parentMovie;
	/* BFS'de bu aktör, queue'ya sokan filmi aldýðýmýz aktör */
	struct actor* parentActor;
};

typedef struct actor actor;

struct queue {
	int count;
	/* ilk eleman */
	actor* front;
	/* son eleman */
	actor* rear;
};

typedef struct queue queue;

/* Queue'nun default deðerlerini atar. */
void init(queue* q){
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
}

int isEmpty(queue* q){
	return q->count == 0;
}

/* Queue'ya aktör ekler. */
void enqueue(queue* q, actor* a){
	/* Boþsa baþa ekle. */
	if(isEmpty(q)){
		q->rear = a;
		q->front = q->rear;
		q->rear->qNext = NULL;
		q->count = q->count + 1;
	} else { /* Boþ deðilse sona ekle */
		q->rear->qNext = a;
		q->rear->qNext->qNext = NULL;
		q->rear = q->rear->qNext;
		q->count = q->count + 1;
	}
}

/* Sýradaki elemaný queue'dan çeker. */
actor* dequeue(queue* q){
	actor* tmp;
	if(!isEmpty(q)){
		tmp = q->front;
		q->front = q->front->qNext;
		q->count = q->count - 1;
		return tmp;
	}
	return NULL;
}

/* Verilen aktör tabloda var mý diye bakar. */
actor* findActor(actor** hashMap, char* name) {
	unsigned long key = findKey(name);
	int hash = findHash(key, 10001);
	/* hashTable'ýn verilen aktör için ayrýlan gözü boþsa aktör tabloda yoktur. */
	if (strcmp(hashMap[hash]->name, "Empty") == 0) {
		return NULL;
	} else {
		/* hashTable'ýn verilen aktör için ayrýlan gözü doluysa ve aktör bu gözün içindeki
		linkli listede bulunuyorsa aktör tabloda vardýr. */
		actor* tmp = hashMap[hash];
		while ( strcmp(tmp->name, name) != 0 && tmp->next != NULL)
			tmp = tmp->next;
		if (strcmp(tmp->name, name) != 0) {
			return NULL;
		}
		return tmp;
	}
}

/* Verilen aktörü tabloya ekler. */
void addActor(actor** hashMap, actor* a) {
	unsigned long key = findKey(a->name);
	int hash = findHash(key, 10001);
	/* Olmasý gerekn göz boþsa direk ekler. */
	if (strcmp(hashMap[hash]->name, "Empty") == 0) {
		hashMap[hash] = a;
		a->next = NULL;
	} else {
		/* Göz doluysa ve eðer daha önce bu aktörü oraya eklememiþsek þimdi ekler. */
		actor* tmp = hashMap[hash];
		while ( strcmp(tmp->name, a->name) != 0 && tmp->next != NULL)
			tmp = tmp->next;
		if (strcmp(tmp->name, a->name) != 0) {
			tmp->next = a;
		}
	}
}

/* Hash hesaplar. */
int findHash(unsigned long key, int M) {
	return key % (unsigned long) M;
}

/* Verilen kelimenin keyini bulur. */
int findKey(char* word) {
	int R = 31;
	int i;
	unsigned long key = 0;
	
	int wordLength = 0;
	while (word[wordLength] != '\0') 
		wordLength++;
	
	for(i = 0; i < wordLength; i++) {
		key += word[i] * power(R, wordLength-1-i);
	}
	
	return key;
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

/* Graph'ýn bir kýsmýný freeler */
void freeGraph(actor** hashMap, int M) {
	int i;
	int j;
	for (i = 0; i < M; i++) {
		free(hashMap[i]->movies);
		free(hashMap[i]);
	}
}

/* Soruda istenen asýl kýsým.
	mode 0: Kevin Bacon sayýsý bulma
	mode 1: Verilen aktörler arasý uzaklýk bulma */
int findKevinBacon(actor** hashMap, char* actorName, char* actorName2, int mode) {
	/* i ve j indis dedðiþkenleri, flag aktörün bulunup bulunmadýðýný tutan deðiþken. */
	int i, j, flag;
	
	/* Eðer aktörler aynýysa iþleme girmeye gerek yok. Uzaklýk 0'dýr. */
	if ( (mode == 1 && strcmp(actorName, actorName2) == 0) || (mode == 0 && strcmp(actorName, "Bacon, Kevin") == 0)) {
		if (mode == 0) {
			printf("\n\nKevin Bacon Sayisi: %d\n(Artistler ayni.)\n", 0);
		} else {
			printf("\n\nUzaklik: %d\n(Artistler ayni.)\n", 0);
		}
		return 0;
	}
	
	/* BFS'ye baþlanacak düðüm.
		Hedef düðümü baþlangýç olarak seçip, kaynak düðüme ulaþýldýðýnda
		geriye doðru isimleri yazdýrýrsak sorudaki gibi çýktý elde ederiz. */
	actor* start;
	/* Mode'a göre baþlangýç düðümü seçimi */
	if (mode == 0)
		start = findActor(hashMap, "Bacon, Kevin");
	else
		start = findActor(hashMap, actorName2);
	
	/* mainQueue: BFS iþleminde kullanýlan queue.
	   resetQueue: Ýþlem bittikten sonra, kullanýlan düðümlerin
	   uzaklýklarýný ve parentlarýný default deðere çeker. Bu sayede
	   kod tekrar tekrar kullanýlabilir. */
	queue* mainQueue = (queue*) malloc( sizeof(queue) );
	queue* resetQueue = (queue*) malloc( sizeof(queue) );
	
	init(mainQueue);
	init(resetQueue);
	
	/* ilk düðümün deðerleri oluþturulur. */
	start->distance = 0;
	start->parentMovie = NULL;
	start->parentActor = NULL;
	/* ilk düðüm sýraya alýnýr. */
	enqueue(mainQueue, start);
	enqueue(resetQueue, start);
	/* flag 0: aranan düðüm bulunamadý */
	flag = 0;
	
	/* Queue boþalana ya da aranan düðüm bulunana kadar döner. */
	while(!isEmpty(mainQueue) && flag == 0) {
		/* Sýradaki elemaný (Aktör) alýr */
		actor* tmpActor = dequeue(mainQueue);
		/* Eðer uzaklýk limiti aþýldýysa iþlem yapmaz. */
		if (tmpActor->distance < 6) {
			movie* tmpMovie;
			i = 0;
			/* Alýnan aktörün filmlerini dolaþan döngü. */
			while (i < tmpActor->moviesSize && flag == 0) {
				tmpMovie = tmpActor->movies[i];
				j = 0;
				/* Her film için aktörleri sýraya ekleyen döngü. */
				while (j < tmpMovie->actorsSize && flag == 0) {
					/* Bu düðüme daha önce ulaþýlmadýysa distance ve parent bilgilerini günceller ve sýraya ekler. */
					if (tmpMovie->actors[j]->distance == INT_MAX) {
						tmpMovie->actors[j]->distance = tmpActor->distance + 1;
						tmpMovie->actors[j]->parentMovie = tmpMovie;
						tmpMovie->actors[j]->parentActor = tmpActor;
						enqueue(mainQueue, tmpMovie->actors[j]);
						enqueue(resetQueue, tmpMovie->actors[j]);
					}
					/* Eðer aranan aktör bulunduysa flagi set eder. */
					if (strcmp(actorName, tmpMovie->actors[j]->name) == 0) {
						flag = 1;
					}
					j++;
				}
				i++;
			}
		}
	}
	/* Eðer flag 1 ise aranan aktöre ulaþýlmuþtýr. */
	if (flag == 1) {
		/* Queue'ya en son eklenen aktör aranan aktördür. */
		actor* current = mainQueue->rear;
		/* value: kevin bacon sayýsý */
		int value = current->distance;
		if (mode == 0)
			printf("\n\nKevin Bacon Sayisi: %d", value);
		else
			printf("\n\nUzaklik: %d", value);
		/* Son düðümden geriye doðru giderek yolu ekrana yazdýrýr. */
		while (current->parentActor != NULL) {
			printf("\n%s - %s : \"%s\"", current->name, current->parentActor->name, current->parentMovie->name);
			current = current->parentActor;
		}
		printf("\n");
		/* Kullanýlan düðümleri tekrar kullanýma hazýr hale getirir. */
		while (!isEmpty(resetQueue)) {
			actor* reset = dequeue(resetQueue);
			reset->parentMovie = NULL;
			reset->parentActor = NULL;
			reset->distance = INT_MAX;
			reset->qNext = NULL;
		}
		return value;
	}
	/* flag 0 ise aktöre ulaþýlamamýþtýr. */
	printf("\n\nKevin Bacon Sayisi: %d", -1);
	while (!isEmpty(resetQueue)) {
		actor* reset = dequeue(resetQueue);
		reset->parentMovie = NULL;
		reset->parentActor = NULL;
		reset->distance = INT_MAX;
		reset->qNext = NULL;
	}
	
	printf("\n%s ile Kevin Bacon arasinda baglanti yok.\n(Veya mesafe 6'dan buyuk.)\n", actorName);
	return 0;
}

int main() {
	
	/* HashTable Oluþturma Baþlangýcý */
	/* Her göze dummy bir eleman(Aktör) koydum. Daha sonra eklenen düðümleri bu
		dummy ile deðiþtirdim. Çakýþma sorununu Seperate Chaining yöntemi ile
		çözdüm. */
	actor** hashMap = (actor**) malloc(sizeof(actor*) * 10001);
	int hashSize = 0;
	int i;
	for (i = 0; i < 10001; i++) {
		hashMap[i] = (actor*) malloc(sizeof(actor));
		hashMap[i]->name = "Empty";
		hashMap[i]->moviesSize = 0;
		hashMap[i]->movies = (movie**) malloc(sizeof(movie));
		hashMap[i]->distance = INT_MAX;
		hashMap[i]->qNext = NULL;
	}
	/* HashTable Oluþturma Bitiþi */
	
	/* FILE IO + HASHTABLE DOLDURMA BAÞLANGICI */
	char fileName[100];
	
	printf("Dosya adi: ");
	scanf ("%[^\n]%*c", fileName);
	/* Dosyayý aç */
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("%s isimli dosya acilamadi!\n", fileName);
		exit(EXIT_FAILURE);
	}
	
	char tmpChar = fgetc(fp);
	int state = 0;
	char* tmpName = (char*) malloc( sizeof(char) * 1 );
	int nameSize = 0;
	movie* tmpMovie;
	while (tmpChar != EOF) {
		/* state 0: film ismini al.
			state 1: aktörleri al. */
		if (state == 0) {
			while (tmpChar != '/') {
				tmpName[nameSize] = tmpChar;
				nameSize++;
				tmpName = (char*) realloc( tmpName, sizeof(char) * (nameSize+1) );
				tmpChar = fgetc(fp);
			}
			tmpChar = fgetc(fp);
			tmpName[nameSize] = '\0';
			/* Film ismi tmpName'de oluþtu. */
			state++;
			nameSize = 0;
			
			/* film düðümü oluþtur. */
			tmpMovie = (movie*) malloc( sizeof(movie) );
			tmpMovie->name = tmpName;
			tmpMovie->actors = (actor**) malloc( sizeof(actor*) );
			tmpMovie->actorsSize = 0;
			
			/* Tekrar kullanmak için tmpName'e yeni adres ata. */
			tmpName = (char*) malloc( sizeof(char*) );
		} else {
			/* Aktörleri alan blok */
			while (tmpChar != '/' && tmpChar != '\n' && tmpChar != EOF) {
				tmpName[nameSize] = tmpChar;
				nameSize++;
				tmpName = (char*) realloc( tmpName, sizeof(char) * (nameSize+1) );
				tmpChar = fgetc(fp);
			}
			if (tmpChar == '\n') {
				state--;
				/* \n geldiðinde aktörleri alma tamamlanmýþtýr. */
			}
			tmpChar = fgetc(fp);
			tmpName[nameSize] = '\0';
			/* Aktör ismi tmpName üzerinde oluþtu. */
			nameSize = 0;
			
			/* Eðer bu isimde bir aktör yoksa yeni aktör oluþtur.
				Aktörün filmlerine az önce bulunan filmi ekle.
				Eðer bu isimde bir aktör varsa yeni aktör oluþturma.
				Az önce bulunan filmi bu aktörün filmlerine ekle. */
			actor* tmpActor;
			if (findActor(hashMap, tmpName) == NULL) {
				/* Eðer bu isimde aktör yoksa */
				tmpActor = (actor*) malloc( sizeof(actor) );
				tmpActor->name = tmpName;
				tmpActor->movies = (movie**) malloc( sizeof(movie*) * 2 );
				tmpActor->moviesSize = 0;
				tmpActor->movies[tmpActor->moviesSize] = tmpMovie;
				tmpActor->moviesSize = tmpActor->moviesSize + 1;
				tmpActor->distance = INT_MAX;
				tmpActor->qNext = NULL;
				
				tmpActor->next = NULL;
				addActor(hashMap, tmpActor);
			} else {
				/* Eðer bu isimde aktör varsa */
				tmpActor = findActor(hashMap, tmpName);
				tmpActor->movies[tmpActor->moviesSize] = tmpMovie;
				tmpActor->moviesSize = tmpActor->moviesSize + 1;
				tmpActor->movies = (movie**) realloc( tmpActor->movies, sizeof(movie*) * (tmpActor->moviesSize+1) );
				tmpActor->distance = INT_MAX;
				tmpActor->qNext = NULL;
			}
			/* Az önce bulunan filmin aktörlerine bu aktörü ekle. */
			tmpMovie->actors[tmpMovie->actorsSize] = tmpActor;
			tmpMovie->actorsSize = tmpMovie->actorsSize + 1;
			tmpMovie->actors = (actor**) realloc( tmpMovie->actors, sizeof(actor*) * (tmpMovie->actorsSize+1) );
			
			/* tmpName'i tekrar kullanmak için yeni adres ata. */
			tmpName = (char*) malloc( sizeof(char*) );
		}
	}
	
	free(tmpName);
	fclose(fp);
	/* FILE IO + HASHTABLE DOLDURMA BÝTÝÞÝ */
	
	/* BFS Baþlangýcý */
	int mode;
	char input[100];
	do {
		printf("\n(-1: Cikis.)\n1.Kevin Bacon sayisi bul.\n2.Iki artist arasi uzaklik bul.\nSecim: ");
		scanf("%d", &mode);
		/*Dummy*/
		scanf("%c");
		if (mode == 1) {
			printf("\nArtist ismi (Soyisim, Isim): ");
			scanf ("%[^\n]%*c", input);
			while (findActor(hashMap, input) == NULL) {
				printf("Bu isimde bir artist yok!\nArtist ismi (Soyisim, Isim): ");
				scanf ("%[^\n]%*c", input);
			}
			/* Girilen aktörün kevin bacon sayýsýný bul. 
				hashMap: Aktörlerin tutulduðu hashMap
				input: Girilen aktör
				"":	Dummy
				0: mode 0 (kevin bacon mod) */
			findKevinBacon(hashMap, input, "", 0);
		} else if (mode == 2) {
			printf("\n1.Artist ismi (Soyisim, Isim): ");
			scanf ("%[^\n]%*c", input);
			while (findActor(hashMap, input) == NULL) {
				printf("Bu isimde bir artist yok!\nArtist ismi (Soyisim, Isim): ");
				scanf ("%[^\n]%*c", input);
			}
			char input2[100];
			printf("2.Artist ismi (Soyisim, Isim): ");
			scanf ("%[^\n]%*c", input2);
			while (findActor(hashMap, input2) == NULL) {
				printf("Bu isimde bir artist yok!\nArtist ismi (Soyisim, Isim): ");
				scanf ("%[^\n]%*c", input2);
			}
			/* Girilen aktörler arasýndaki mesafeyi bul. 
				hashMap: Aktörlerin tutulduðu hashMap
				input: Girilen 1.aktör
				input2: Girilen 2.aktör
				1: mode 1 (Mesafe modu) */
			findKevinBacon(hashMap, input, input2, 1);
		}
	} while (mode != -1);
	/* BFS Bitiþi */
	
	freeGraph(hashMap, 10001);
	printf("\n");
	system("PAUSE");
	return 0;
}
