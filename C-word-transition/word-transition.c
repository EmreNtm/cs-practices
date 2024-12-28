#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000		/*queue'nun kapasitesi. */
#define MAX_CHAR 10		/*char* allocate ederken kulland���m bir sabit.*/

struct node {				/*Kelimeleri tutan struct yap�s�*/
	char* word;				/*Okunan kelime*/
	int nodeNo;				/*D���m�n matristeki indisi*/
	int marked;				/*Default olarak 0'd�r, queue' ya girdi�inde 1 yap�l�r.*/
	struct node* parent;	/*BFS yaparken bir d���me ula��ld���nda o d���me nereden geldi�imizi tutar.*/
	struct node* child;		/*BFS yapt�ktan sonra bir yol bulunmu�sa yolu takip ederken kulland���m de�i�ken.*/
	struct node* next;		/*Dosyadan okunan t�m d���mleri bir linkli listede tuttum. Linkli liste i�in gerekli next de�i�keni.*/
}*head = NULL;				/*D���mleri tutan linkli listenin ba��.*/

typedef struct node node;

void addNode(char* w, int no) {		/*D���m linkli listesine yeni d���m ekler.*/
	node* tmp = head;
	while(tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = (node*) malloc(sizeof(node));
	tmp->next->word = (char*) malloc(sizeof(char) * 10);
	strcpy(tmp->next->word, w);
	tmp->next->nodeNo = no;
	tmp->next->marked = 0;
	tmp->next->parent = NULL;
	tmp->next->child = NULL;
	tmp->next->next = NULL;
}

node* findNode(int no) {	/*Numaras� verilen d���m� bulup d�nd�r�r.*/
	node* tmp = head;
	while(tmp != NULL && tmp->nodeNo != no) {
		tmp = tmp->next;
	}
	return tmp;
}

node* findNode2(char* w) {	/*Kelimesi verilen d���m� bulup d�nd�r�r.*/
	node* tmp = head;
	while(tmp != NULL && strcmp(tmp->word, w) != 0) {
		tmp = tmp->next;
	}
	return tmp;
}

void displayList() {		/*Bug ��zerken kulland���m linkli listeyi g�steren fonksiyon.*/
	node* tmp = head;
	while(tmp->next != NULL) {
		printf("%s\n", tmp->word);
		tmp = tmp->next;
	}
	printf("%s\n", tmp->word);
}

void freeNodeList() {	/*D���m linkli listesini serbest b�rak�r.*/
	node* tmp;
	while(head->next != NULL) {
		tmp = head->next;
		free(head);
		head = tmp;
	}
	free(head);
}

void freeMatrix(int** m, int N) {	/*Verilen matrisi serbest b�rak�r.*/
	int i;
    for (i = 0; i < N; i++) {
        free (m[i]);
    }
    free (m);
}


struct qNode {				/*Queue yap�s�nda kulland���m d���m.*/
	node* veri;				/*Queue d���m�n�n verisi graf d���m�*/
	struct qNode* next;		/*Queue yap�s�nda bu d���mden sonra gelen d���m� tutar.*/
};

typedef struct qNode qNode;

struct queue {		/*Queue yap�s�*/
	int count;		/*Queue'daki eleman say�s�*/
	qNode* front;	/*En �n s�radaki queue d���m�*/
	qNode* rear;	/*En arka s�radaki queue d���m�*/
};

typedef struct queue queue;

void init(queue* q){	/*Bo� bir queue olu�turur.*/
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
}

int isEmpty(queue* q){	/*Queue bo� de�il ise 0 d�nd�r�r.*/
	return q->count == 0;
}

void enqueue(queue* q, node* v, node* p){	/*Queue'ya d���m ekler.*/
	if(q->count < MAX){
		if(isEmpty(q)){
			q->rear = (qNode*) malloc(sizeof(qNode));
			q->front = q->rear;
			q->rear->veri = v;
			q->rear->next = NULL;
			q->rear->veri->parent = p;
			q->count = q->count + 1;
		} else {
			q->rear->next = (qNode*) malloc(sizeof(qNode));
			q->rear->next->veri = v;
			q->rear->next->next = NULL;
			q->rear->next->veri->parent = p;
			q->rear = q->rear->next;
			q->count = q->count + 1;
		}
	}
}

node* dequeue(queue* q){	/*Queue'dan en �ndeki d���m� ��kar�r.*/
	qNode* tmp;
	node* tmp2;
	if(!isEmpty(q)){
		tmp = q->front;
		tmp2 = q->front->veri;
		q->front = q->front->next;
		free(tmp);
		q->count = q->count - 1;
		return tmp2;
	}
	return NULL;
}

void emptyQueue(q) {		/*Tekrar d�n���m bulunmak istendi�inde queue'yi eski haline getirmek i�in bo�alt�r.*/
	while(!isEmpty(q))
		dequeue(q);
}

void resetMarks() {			/*Tekrar d�n���m bulunmak istendi�inde graf d���mlerini eski haline getirir.*/
	node* tmp = head;
	while(tmp != NULL) {
		tmp->marked = 0;
		tmp->parent = NULL;
		tmp->child = NULL;
		tmp = tmp->next;
	}
}

int checkCharDifference(char* w1, char* w2) {	/*Verilen iki e�it uzunluklu stringin birbiriyle uyu�mayan karakter say�s�n� d�nd�r�r.*/
	int count = 0;
	int size = 0;
	while(w1[size] != NULL){
		if(w1[size] != w2[size]) {
			count++;
		}
		size++;
	}
	return count;
}

int checkGraph(int** m, char* w1, char* w2) {
	return m[findNode2(w1)->nodeNo][findNode2(w2)->nodeNo];
}

int main() {
	
	FILE* fp;											/*Dosya de�i�keni*/
	char* kelime = (char*) malloc(sizeof(char) * 10);	/*Dosyadan okudu�um kelimeleri ald���m ge�ici de�i�ken.*/
	int** matris;										/*Graf d���mleri aras�ndaki ba�lant�lar� tutacak matris.*/
	int N;												/*Matrisin boyutu*/
	int i = 0;											/*�ndis de�i�keni*/
	int j = 0;											/*�ndis de�i�keni*/
	
	fp = fopen("kelime.txt", "r");
    if(fp == NULL) {
    	perror("Error opening file");
		return(-1);
   	}
	
	
	
	
	/*D���mleri dosyadan okuyup linkli liste halinde tutmaya ba�lad���m yer.*/
	head = (node*) malloc(sizeof(node));
	head->word = (char*) malloc(sizeof(char) * MAX_CHAR);
	fgets(kelime, MAX_CHAR, fp);
	while(kelime[j] != NULL)				/*Bu d�ng� okunan kelimenin uzunlu�unu bulur.*/
		j++;
	j--;
	kelime[j] = '\0';						/*'\n' yerine null koydum. �leride kullan�c� giri�i yaparken i�imi rahatlatt�.*/
	strcpy(head->word, kelime);
	head->nodeNo = i;
	head->next = NULL;
	i++;									/*Buraya kadar ilk d���m� head �zerinde olu�turdum.*/
	
	while(fgets(kelime, 10, fp) != NULL) {	/*Bu d�ng� ile geri kalan d���mleri de olu�turdum.*/
		kelime[j] = '\0';
		addNode(kelime, i);
		i++;
	}
	N = i;									/*��emler sonucunda kelime say�s� i �zerinde olu�mu�tu, onu N'e aktard�m.*/
	/*D���mleri dosyadan okuyup linkli liste halinde tutmay� bitirdi�im yer.*/
	

	
	/*Matrisi allocate etti�im yerin ba�lang�c�.*/
	matris = (int**) malloc(sizeof(int*) * N);
	for(j = 0; j < N; j++) {
		matris[j] = (int*) malloc(sizeof(int) * N);
	}
	/*Matrisi allocate etti�im yerin biti�i.*/
	
	
	/*Matrisin i�ini doldurdu�um yerin ba�lang�c�.*/
	node* tempNode1 = head;			/*D���mleri yerle�tirirken kulland���m ge�ici de�i�kenler.*/
	node* tempNode2 = head;
	
	for(i = 0; i < N; i++) {		/*D�� d�ng� [0, N), i� d�ng� [i, N) kadar d�n�yor. Bo� kalan k�s�mlar dolu olanlar�n simetri�i oldu�undan gereksiz yere d�nmedim.*/
		kelime = tempNode1->word;
		for(j = i; j < N; j++) {
			if(checkCharDifference(kelime, tempNode2->word) == 1) {	/*Kelimelerin farkl� karakter say�s� 1 ise ba�lant� var, de�il ise ba�lant� yok.*/
				matris[i][j] = 1;
				matris[j][i] = 1;
			} else {
				matris[i][j] = 0;
				matris[j][i] = 0;
			}
			tempNode2 = tempNode2->next;
		}
		tempNode1 = tempNode1->next;	/*tempNode1, head'den son d���me kadar her d���m� gezip matrisin sat�rlar�n� olu�turuyor.*/
		tempNode2 = findNode(i+1);		/*tempNode2, i+1 (Bir sonraki ad�mda i� d�ng�de kullan�calak olan i de�eri.) */
	}									/*matris numaras�na sahip d���mden ba�lay�p sradaki d���mleri gezip matrisin s�tunlar�n� olu�turuyor.*/
	/*Matrisin i�ini doldurdu�um yerin biti�i.
	
	/*A�a��daki switch case'in case 1 durumunda kullan�lan de�i�ken tan�mlamalar� ba�lang�c�.*/
	char* kelime1 = (char*) malloc(sizeof(char) * 10);	/*Kullan�c�dan al�nan ilk kelime.*/
	char* kelime2 = (char*) malloc(sizeof(char) * 10);	/*Kullan�c�dan al�nan ikinci kelime.*/
	queue* q = (queue*) malloc(sizeof(queue));			/*queue yap�s� i�in de�i�ken.*/
	char* tmpWord = (char*) malloc(sizeof(char) * 10);	/*��lemlerde kulland���m ge�ici de�i�kenler*/
	node* tmpNode = NULL;
	/*A�a��daki switch case'in case 1 durumunda kullan�lan de�i�ken tan�mlamalar� biti�i.*/
	
	int secim;
	do {	/*Uygulaman�n devam etmesini sa�layan d�ng�. 0 girilirse d�ng� biter.*/
		do {
			printf("\n\nSeciniz:\n1.Donusum bul.\n2.Baglanti var mi?\n0.Cikis\nSeciminiz: ");
			scanf("%d", &secim);
		} while(secim < 0 || secim > 2);
		
		if(secim != 0) {
			switch(secim) {
				case 1:	/*Donusum bul.*/
					/*BFS ba�lang�c�.*/
					printf("\n");
					do {
						printf("Kelime1: ");
						scanf("%s", kelime1);
					} while(findNode2(kelime1) == NULL);	/*Kelime dosyada mevcut olmad��� s�rece istemeye devam eder.*/
					do {
						printf("Kelime2: ");
						scanf("%s", kelime2);
					} while(findNode2(kelime2) == NULL);	/*Kelime dosyada mevcut olmad��� s�rece istemeye devam eder.*/
					
					init(q);									/*q de�i�keni �zerine bir Queue olu�turur.*/
					enqueue(q, findNode2(kelime1), NULL);		/*�lk kelimeye sahip graf d���m�n� bulur ve queue'ya atar.*/
					findNode2(kelime1)->marked = 1;				/*D���m queue'ya girdi�i i�in d���m� i�aretler. (��lenmemi� d���mler ile i�lenmeye ba�lamam�� d���mleri ay�rt etmek i�in gerekli.)*/
					tmpWord = findNode2(kelime1)->word;			/*Ge�ici de�i�kene kelimeyi al�r.*/
					while(!isEmpty(q) && strcmp(tmpWord, kelime2) != 0) {	/*Aranan kelime bulunamad��� veya queue bo�almad��� s�rece d�ng� devam eder.*/
						tmpNode = dequeue(q);								/*En �ndeki d���m queue'dan ��kart�l�r.*/
						tmpWord = tmpNode->word;
						/*printf("\nSu anda queue'dan cekilen kelime: %s\n", tmpWord);*/
						if(strcmp(tmpWord, kelime2) != 0) {					/*E�er istenen kelime bulunmad�ysa ��kart�lan d���m�n gezilmemi� kom�ular� queue'ya eklenir ve i�aretlenir.*/
							for(j = 0; j < N; j++) {
								if(matris[tmpNode->nodeNo][j] == 1 && findNode(j)->marked == 0) {
									/*printf("\n	Su anda queue'ya atilan kelime: %s\n", findNode(j)->word);*/
									enqueue(q, findNode(j), tmpNode);
									findNode(j)->marked = 1;
								} 
							}
						}
					}														/*E�er istenen kelime bulunduysa d�ng� biter. �stenen kelimeye ait d���m, tmpNode �zerinde olu�ur.*/
					
					if(strcmp(tmpWord, kelime2) == 0) {	
						i = 0;
						while(tmpNode->parent != NULL) {		/*Yolu parentlar� tutarak kaybetmemi�tim. D���mlerin childlar�n� doldurarak ileri y�nde de takip edebilmeyi sa�l�yorum.*/
							tmpNode->parent->child = tmpNode;
							tmpNode = tmpNode->parent;
							i++;
						}
						i++;
						
						printf("\nDonusum bulundu! Adim sayisi %d:\n", i);
						while(tmpNode != NULL) {				/*Yap�lan ge�i�leri s�ras�yla yazd�r�yorum.*/
							printf("%s , ", tmpNode->word);
							tmpNode = tmpNode->child;
						}
					} else {
						printf("\nDonusum mevcut degil.\n");
					}
					emptyQueue(q);
					resetMarks();
					/*BFS biti�i.*/
					break;
				case 2:	/*Baglanti var mi?*/
					/*Ba�lant� kontrol� ba�lang�c�.*/
					printf("\n");
					do {
						printf("Kelime1: ");
						scanf("%s", kelime1);
					} while(findNode2(kelime1) == NULL);	/*Kelime dosyada mevcut olmad��� s�rece istemeye devam eder.*/
					do {
						printf("Kelime2: ");
						scanf("%s", kelime2);
					} while(findNode2(kelime2) == NULL);	/*Kelime dosyada mevcut olmad��� s�rece istemeye devam eder.*/
					if(checkGraph(matris, kelime1, kelime2) == 1) {		/*Matris kontrol ediliyor. E�er 1 ise ba�lant� vard�r.*/
						printf("\nBaglanti mevcut!");
					} else {
						printf("\nBaglanti mevcut degil!");
					}
					/*Ba�lant� kontrol� biti�i.*/
					break;
			}
		}
	} while(secim != 0);
	
	
	
	
	freeNodeList();
	freeMatrix(matris, N);
	fclose(fp);
	printf("\n");
	system("PAUSE");
	return 0;
}
