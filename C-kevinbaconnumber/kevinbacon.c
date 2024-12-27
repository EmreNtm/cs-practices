#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 2147483647
/* Emre Nitim 17011079 */

/* Filmleri tuttu�um graf d���m� */
struct movie {
	char* name;
	/* Filmde oynayan akt�rler */
	struct actor** actors;
	int actorsSize;
};

typedef struct movie movie;

struct actor {
	char* name;
	/* Akt�r�n oynad��� filmler */
	movie** movies;
	int moviesSize;
	/* next sadece hashMap'te Seperate Chaining i�in kullan�ld�. */
	struct actor* next;
	/* qNext sadece queue i�in kullan�ld�. */
	struct actor* qNext;
	/* BFS uzakl��� */
	int distance;
	/* BFS'de bu akt�r� queue'ya sokan film */
	movie* parentMovie;
	/* BFS'de bu akt�r, queue'ya sokan filmi ald���m�z akt�r */
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

/* Queue'nun default de�erlerini atar. */
void init(queue* q){
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
}

int isEmpty(queue* q){
	return q->count == 0;
}

/* Queue'ya akt�r ekler. */
void enqueue(queue* q, actor* a){
	/* Bo�sa ba�a ekle. */
	if(isEmpty(q)){
		q->rear = a;
		q->front = q->rear;
		q->rear->qNext = NULL;
		q->count = q->count + 1;
	} else { /* Bo� de�ilse sona ekle */
		q->rear->qNext = a;
		q->rear->qNext->qNext = NULL;
		q->rear = q->rear->qNext;
		q->count = q->count + 1;
	}
}

/* S�radaki eleman� queue'dan �eker. */
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

/* Verilen akt�r tabloda var m� diye bakar. */
actor* findActor(actor** hashMap, char* name) {
	unsigned long key = findKey(name);
	int hash = findHash(key, 10001);
	/* hashTable'�n verilen akt�r i�in ayr�lan g�z� bo�sa akt�r tabloda yoktur. */
	if (strcmp(hashMap[hash]->name, "Empty") == 0) {
		return NULL;
	} else {
		/* hashTable'�n verilen akt�r i�in ayr�lan g�z� doluysa ve akt�r bu g�z�n i�indeki
		linkli listede bulunuyorsa akt�r tabloda vard�r. */
		actor* tmp = hashMap[hash];
		while ( strcmp(tmp->name, name) != 0 && tmp->next != NULL)
			tmp = tmp->next;
		if (strcmp(tmp->name, name) != 0) {
			return NULL;
		}
		return tmp;
	}
}

/* Verilen akt�r� tabloya ekler. */
void addActor(actor** hashMap, actor* a) {
	unsigned long key = findKey(a->name);
	int hash = findHash(key, 10001);
	/* Olmas� gerekn g�z bo�sa direk ekler. */
	if (strcmp(hashMap[hash]->name, "Empty") == 0) {
		hashMap[hash] = a;
		a->next = NULL;
	} else {
		/* G�z doluysa ve e�er daha �nce bu akt�r� oraya eklememi�sek �imdi ekler. */
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

/* Graph'�n bir k�sm�n� freeler */
void freeGraph(actor** hashMap, int M) {
	int i;
	int j;
	for (i = 0; i < M; i++) {
		free(hashMap[i]->movies);
		free(hashMap[i]);
	}
}

/* Soruda istenen as�l k�s�m.
	mode 0: Kevin Bacon say�s� bulma
	mode 1: Verilen akt�rler aras� uzakl�k bulma */
int findKevinBacon(actor** hashMap, char* actorName, char* actorName2, int mode) {
	/* i ve j indis ded�i�kenleri, flag akt�r�n bulunup bulunmad���n� tutan de�i�ken. */
	int i, j, flag;
	
	/* E�er akt�rler ayn�ysa i�leme girmeye gerek yok. Uzakl�k 0'd�r. */
	if ( (mode == 1 && strcmp(actorName, actorName2) == 0) || (mode == 0 && strcmp(actorName, "Bacon, Kevin") == 0)) {
		if (mode == 0) {
			printf("\n\nKevin Bacon Sayisi: %d\n(Artistler ayni.)\n", 0);
		} else {
			printf("\n\nUzaklik: %d\n(Artistler ayni.)\n", 0);
		}
		return 0;
	}
	
	/* BFS'ye ba�lanacak d���m.
		Hedef d���m� ba�lang�� olarak se�ip, kaynak d���me ula��ld���nda
		geriye do�ru isimleri yazd�r�rsak sorudaki gibi ��kt� elde ederiz. */
	actor* start;
	/* Mode'a g�re ba�lang�� d���m� se�imi */
	if (mode == 0)
		start = findActor(hashMap, "Bacon, Kevin");
	else
		start = findActor(hashMap, actorName2);
	
	/* mainQueue: BFS i�leminde kullan�lan queue.
	   resetQueue: ��lem bittikten sonra, kullan�lan d���mlerin
	   uzakl�klar�n� ve parentlar�n� default de�ere �eker. Bu sayede
	   kod tekrar tekrar kullan�labilir. */
	queue* mainQueue = (queue*) malloc( sizeof(queue) );
	queue* resetQueue = (queue*) malloc( sizeof(queue) );
	
	init(mainQueue);
	init(resetQueue);
	
	/* ilk d���m�n de�erleri olu�turulur. */
	start->distance = 0;
	start->parentMovie = NULL;
	start->parentActor = NULL;
	/* ilk d���m s�raya al�n�r. */
	enqueue(mainQueue, start);
	enqueue(resetQueue, start);
	/* flag 0: aranan d���m bulunamad� */
	flag = 0;
	
	/* Queue bo�alana ya da aranan d���m bulunana kadar d�ner. */
	while(!isEmpty(mainQueue) && flag == 0) {
		/* S�radaki eleman� (Akt�r) al�r */
		actor* tmpActor = dequeue(mainQueue);
		/* E�er uzakl�k limiti a��ld�ysa i�lem yapmaz. */
		if (tmpActor->distance < 6) {
			movie* tmpMovie;
			i = 0;
			/* Al�nan akt�r�n filmlerini dola�an d�ng�. */
			while (i < tmpActor->moviesSize && flag == 0) {
				tmpMovie = tmpActor->movies[i];
				j = 0;
				/* Her film i�in akt�rleri s�raya ekleyen d�ng�. */
				while (j < tmpMovie->actorsSize && flag == 0) {
					/* Bu d���me daha �nce ula��lmad�ysa distance ve parent bilgilerini g�nceller ve s�raya ekler. */
					if (tmpMovie->actors[j]->distance == INT_MAX) {
						tmpMovie->actors[j]->distance = tmpActor->distance + 1;
						tmpMovie->actors[j]->parentMovie = tmpMovie;
						tmpMovie->actors[j]->parentActor = tmpActor;
						enqueue(mainQueue, tmpMovie->actors[j]);
						enqueue(resetQueue, tmpMovie->actors[j]);
					}
					/* E�er aranan akt�r bulunduysa flagi set eder. */
					if (strcmp(actorName, tmpMovie->actors[j]->name) == 0) {
						flag = 1;
					}
					j++;
				}
				i++;
			}
		}
	}
	/* E�er flag 1 ise aranan akt�re ula��lmu�t�r. */
	if (flag == 1) {
		/* Queue'ya en son eklenen akt�r aranan akt�rd�r. */
		actor* current = mainQueue->rear;
		/* value: kevin bacon say�s� */
		int value = current->distance;
		if (mode == 0)
			printf("\n\nKevin Bacon Sayisi: %d", value);
		else
			printf("\n\nUzaklik: %d", value);
		/* Son d���mden geriye do�ru giderek yolu ekrana yazd�r�r. */
		while (current->parentActor != NULL) {
			printf("\n%s - %s : \"%s\"", current->name, current->parentActor->name, current->parentMovie->name);
			current = current->parentActor;
		}
		printf("\n");
		/* Kullan�lan d���mleri tekrar kullan�ma haz�r hale getirir. */
		while (!isEmpty(resetQueue)) {
			actor* reset = dequeue(resetQueue);
			reset->parentMovie = NULL;
			reset->parentActor = NULL;
			reset->distance = INT_MAX;
			reset->qNext = NULL;
		}
		return value;
	}
	/* flag 0 ise akt�re ula��lamam��t�r. */
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
	
	/* HashTable Olu�turma Ba�lang�c� */
	/* Her g�ze dummy bir eleman(Akt�r) koydum. Daha sonra eklenen d���mleri bu
		dummy ile de�i�tirdim. �ak��ma sorununu Seperate Chaining y�ntemi ile
		��zd�m. */
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
	/* HashTable Olu�turma Biti�i */
	
	/* FILE IO + HASHTABLE DOLDURMA BA�LANGICI */
	char fileName[100];
	
	printf("Dosya adi: ");
	scanf ("%[^\n]%*c", fileName);
	/* Dosyay� a� */
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
			state 1: akt�rleri al. */
		if (state == 0) {
			while (tmpChar != '/') {
				tmpName[nameSize] = tmpChar;
				nameSize++;
				tmpName = (char*) realloc( tmpName, sizeof(char) * (nameSize+1) );
				tmpChar = fgetc(fp);
			}
			tmpChar = fgetc(fp);
			tmpName[nameSize] = '\0';
			/* Film ismi tmpName'de olu�tu. */
			state++;
			nameSize = 0;
			
			/* film d���m� olu�tur. */
			tmpMovie = (movie*) malloc( sizeof(movie) );
			tmpMovie->name = tmpName;
			tmpMovie->actors = (actor**) malloc( sizeof(actor*) );
			tmpMovie->actorsSize = 0;
			
			/* Tekrar kullanmak i�in tmpName'e yeni adres ata. */
			tmpName = (char*) malloc( sizeof(char*) );
		} else {
			/* Akt�rleri alan blok */
			while (tmpChar != '/' && tmpChar != '\n' && tmpChar != EOF) {
				tmpName[nameSize] = tmpChar;
				nameSize++;
				tmpName = (char*) realloc( tmpName, sizeof(char) * (nameSize+1) );
				tmpChar = fgetc(fp);
			}
			if (tmpChar == '\n') {
				state--;
				/* \n geldi�inde akt�rleri alma tamamlanm��t�r. */
			}
			tmpChar = fgetc(fp);
			tmpName[nameSize] = '\0';
			/* Akt�r ismi tmpName �zerinde olu�tu. */
			nameSize = 0;
			
			/* E�er bu isimde bir akt�r yoksa yeni akt�r olu�tur.
				Akt�r�n filmlerine az �nce bulunan filmi ekle.
				E�er bu isimde bir akt�r varsa yeni akt�r olu�turma.
				Az �nce bulunan filmi bu akt�r�n filmlerine ekle. */
			actor* tmpActor;
			if (findActor(hashMap, tmpName) == NULL) {
				/* E�er bu isimde akt�r yoksa */
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
				/* E�er bu isimde akt�r varsa */
				tmpActor = findActor(hashMap, tmpName);
				tmpActor->movies[tmpActor->moviesSize] = tmpMovie;
				tmpActor->moviesSize = tmpActor->moviesSize + 1;
				tmpActor->movies = (movie**) realloc( tmpActor->movies, sizeof(movie*) * (tmpActor->moviesSize+1) );
				tmpActor->distance = INT_MAX;
				tmpActor->qNext = NULL;
			}
			/* Az �nce bulunan filmin akt�rlerine bu akt�r� ekle. */
			tmpMovie->actors[tmpMovie->actorsSize] = tmpActor;
			tmpMovie->actorsSize = tmpMovie->actorsSize + 1;
			tmpMovie->actors = (actor**) realloc( tmpMovie->actors, sizeof(actor*) * (tmpMovie->actorsSize+1) );
			
			/* tmpName'i tekrar kullanmak i�in yeni adres ata. */
			tmpName = (char*) malloc( sizeof(char*) );
		}
	}
	
	free(tmpName);
	fclose(fp);
	/* FILE IO + HASHTABLE DOLDURMA B�T��� */
	
	/* BFS Ba�lang�c� */
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
			/* Girilen akt�r�n kevin bacon say�s�n� bul. 
				hashMap: Akt�rlerin tutuldu�u hashMap
				input: Girilen akt�r
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
			/* Girilen akt�rler aras�ndaki mesafeyi bul. 
				hashMap: Akt�rlerin tutuldu�u hashMap
				input: Girilen 1.akt�r
				input2: Girilen 2.akt�r
				1: mode 1 (Mesafe modu) */
			findKevinBacon(hashMap, input, input2, 1);
		}
	} while (mode != -1);
	/* BFS Biti�i */
	
	freeGraph(hashMap, 10001);
	printf("\n");
	system("PAUSE");
	return 0;
}
