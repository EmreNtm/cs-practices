#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000		/*queue'nun kapasitesi. */
#define MAX_CHAR 10		/*char* allocate ederken kullandýðým bir sabit.*/

struct node {				/*Kelimeleri tutan struct yapýsý*/
	char* word;				/*Okunan kelime*/
	int nodeNo;				/*Düðümün matristeki indisi*/
	int marked;				/*Default olarak 0'dýr, queue' ya girdiðinde 1 yapýlýr.*/
	struct node* parent;	/*BFS yaparken bir düðüme ulaþýldýðýnda o düðüme nereden geldiðimizi tutar.*/
	struct node* child;		/*BFS yaptýktan sonra bir yol bulunmuþsa yolu takip ederken kullandýðým deðiþken.*/
	struct node* next;		/*Dosyadan okunan tüm düðümleri bir linkli listede tuttum. Linkli liste için gerekli next deðiþkeni.*/
}*head = NULL;				/*Düðümleri tutan linkli listenin baþý.*/

typedef struct node node;

void addNode(char* w, int no) {		/*Düðüm linkli listesine yeni düðüm ekler.*/
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

node* findNode(int no) {	/*Numarasý verilen düðümü bulup döndürür.*/
	node* tmp = head;
	while(tmp != NULL && tmp->nodeNo != no) {
		tmp = tmp->next;
	}
	return tmp;
}

node* findNode2(char* w) {	/*Kelimesi verilen düðümü bulup döndürür.*/
	node* tmp = head;
	while(tmp != NULL && strcmp(tmp->word, w) != 0) {
		tmp = tmp->next;
	}
	return tmp;
}

void displayList() {		/*Bug çözerken kullandýðým linkli listeyi gösteren fonksiyon.*/
	node* tmp = head;
	while(tmp->next != NULL) {
		printf("%s\n", tmp->word);
		tmp = tmp->next;
	}
	printf("%s\n", tmp->word);
}

void freeNodeList() {	/*Düðüm linkli listesini serbest býrakýr.*/
	node* tmp;
	while(head->next != NULL) {
		tmp = head->next;
		free(head);
		head = tmp;
	}
	free(head);
}

void freeMatrix(int** m, int N) {	/*Verilen matrisi serbest býrakýr.*/
	int i;
    for (i = 0; i < N; i++) {
        free (m[i]);
    }
    free (m);
}


struct qNode {				/*Queue yapýsýnda kullandýðým düðüm.*/
	node* veri;				/*Queue düðümünün verisi graf düðümü*/
	struct qNode* next;		/*Queue yapýsýnda bu düðümden sonra gelen düðümü tutar.*/
};

typedef struct qNode qNode;

struct queue {		/*Queue yapýsý*/
	int count;		/*Queue'daki eleman sayýsý*/
	qNode* front;	/*En ön sýradaki queue düðümü*/
	qNode* rear;	/*En arka sýradaki queue düðümü*/
};

typedef struct queue queue;

void init(queue* q){	/*Boþ bir queue oluþturur.*/
	q->count = 0;
	q->front = NULL;
	q->rear = NULL;
}

int isEmpty(queue* q){	/*Queue boþ deðil ise 0 döndürür.*/
	return q->count == 0;
}

void enqueue(queue* q, node* v, node* p){	/*Queue'ya düðüm ekler.*/
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

node* dequeue(queue* q){	/*Queue'dan en öndeki düðümü çýkarýr.*/
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

void emptyQueue(q) {		/*Tekrar dönüþüm bulunmak istendiðinde queue'yi eski haline getirmek için boþaltýr.*/
	while(!isEmpty(q))
		dequeue(q);
}

void resetMarks() {			/*Tekrar dönüþüm bulunmak istendiðinde graf düðümlerini eski haline getirir.*/
	node* tmp = head;
	while(tmp != NULL) {
		tmp->marked = 0;
		tmp->parent = NULL;
		tmp->child = NULL;
		tmp = tmp->next;
	}
}

int checkCharDifference(char* w1, char* w2) {	/*Verilen iki eþit uzunluklu stringin birbiriyle uyuþmayan karakter sayýsýný döndürür.*/
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
	
	FILE* fp;											/*Dosya deðiþkeni*/
	char* kelime = (char*) malloc(sizeof(char) * 10);	/*Dosyadan okuduðum kelimeleri aldýðým geçici deðiþken.*/
	int** matris;										/*Graf düðümleri arasýndaki baðlantýlarý tutacak matris.*/
	int N;												/*Matrisin boyutu*/
	int i = 0;											/*Ýndis deðiþkeni*/
	int j = 0;											/*Ýndis deðiþkeni*/
	
	fp = fopen("kelime.txt", "r");
    if(fp == NULL) {
    	perror("Error opening file");
		return(-1);
   	}
	
	
	
	
	/*Düðümleri dosyadan okuyup linkli liste halinde tutmaya baþladýðým yer.*/
	head = (node*) malloc(sizeof(node));
	head->word = (char*) malloc(sizeof(char) * MAX_CHAR);
	fgets(kelime, MAX_CHAR, fp);
	while(kelime[j] != NULL)				/*Bu döngü okunan kelimenin uzunluðunu bulur.*/
		j++;
	j--;
	kelime[j] = '\0';						/*'\n' yerine null koydum. Ýleride kullanýcý giriþi yaparken iþimi rahatlattý.*/
	strcpy(head->word, kelime);
	head->nodeNo = i;
	head->next = NULL;
	i++;									/*Buraya kadar ilk düðümü head üzerinde oluþturdum.*/
	
	while(fgets(kelime, 10, fp) != NULL) {	/*Bu döngü ile geri kalan düðümleri de oluþturdum.*/
		kelime[j] = '\0';
		addNode(kelime, i);
		i++;
	}
	N = i;									/*Ýþemler sonucunda kelime sayýsý i üzerinde oluþmuþtu, onu N'e aktardým.*/
	/*Düðümleri dosyadan okuyup linkli liste halinde tutmayý bitirdiðim yer.*/
	

	
	/*Matrisi allocate ettiðim yerin baþlangýcý.*/
	matris = (int**) malloc(sizeof(int*) * N);
	for(j = 0; j < N; j++) {
		matris[j] = (int*) malloc(sizeof(int) * N);
	}
	/*Matrisi allocate ettiðim yerin bitiþi.*/
	
	
	/*Matrisin içini doldurduðum yerin baþlangýcý.*/
	node* tempNode1 = head;			/*Düðümleri yerleþtirirken kullandýðým geçici deðiþkenler.*/
	node* tempNode2 = head;
	
	for(i = 0; i < N; i++) {		/*Dýþ döngü [0, N), iç döngü [i, N) kadar dönüyor. Boþ kalan kýsýmlar dolu olanlarýn simetriði olduðundan gereksiz yere dönmedim.*/
		kelime = tempNode1->word;
		for(j = i; j < N; j++) {
			if(checkCharDifference(kelime, tempNode2->word) == 1) {	/*Kelimelerin farklý karakter sayýsý 1 ise baðlantý var, deðil ise baðlantý yok.*/
				matris[i][j] = 1;
				matris[j][i] = 1;
			} else {
				matris[i][j] = 0;
				matris[j][i] = 0;
			}
			tempNode2 = tempNode2->next;
		}
		tempNode1 = tempNode1->next;	/*tempNode1, head'den son düðüme kadar her düðümü gezip matrisin satýrlarýný oluþturuyor.*/
		tempNode2 = findNode(i+1);		/*tempNode2, i+1 (Bir sonraki adýmda iç döngüde kullanýcalak olan i deðeri.) */
	}									/*matris numarasýna sahip düðümden baþlayýp sradaki düðümleri gezip matrisin sütunlarýný oluþturuyor.*/
	/*Matrisin içini doldurduðum yerin bitiþi.
	
	/*Aþaðýdaki switch case'in case 1 durumunda kullanýlan deðiþken tanýmlamalarý baþlangýcý.*/
	char* kelime1 = (char*) malloc(sizeof(char) * 10);	/*Kullanýcýdan alýnan ilk kelime.*/
	char* kelime2 = (char*) malloc(sizeof(char) * 10);	/*Kullanýcýdan alýnan ikinci kelime.*/
	queue* q = (queue*) malloc(sizeof(queue));			/*queue yapýsý için deðiþken.*/
	char* tmpWord = (char*) malloc(sizeof(char) * 10);	/*Ýþlemlerde kullandýðým geçici deðiþkenler*/
	node* tmpNode = NULL;
	/*Aþaðýdaki switch case'in case 1 durumunda kullanýlan deðiþken tanýmlamalarý bitiþi.*/
	
	int secim;
	do {	/*Uygulamanýn devam etmesini saðlayan döngü. 0 girilirse döngü biter.*/
		do {
			printf("\n\nSeciniz:\n1.Donusum bul.\n2.Baglanti var mi?\n0.Cikis\nSeciminiz: ");
			scanf("%d", &secim);
		} while(secim < 0 || secim > 2);
		
		if(secim != 0) {
			switch(secim) {
				case 1:	/*Donusum bul.*/
					/*BFS baþlangýcý.*/
					printf("\n");
					do {
						printf("Kelime1: ");
						scanf("%s", kelime1);
					} while(findNode2(kelime1) == NULL);	/*Kelime dosyada mevcut olmadýðý sürece istemeye devam eder.*/
					do {
						printf("Kelime2: ");
						scanf("%s", kelime2);
					} while(findNode2(kelime2) == NULL);	/*Kelime dosyada mevcut olmadýðý sürece istemeye devam eder.*/
					
					init(q);									/*q deðiþkeni üzerine bir Queue oluþturur.*/
					enqueue(q, findNode2(kelime1), NULL);		/*Ýlk kelimeye sahip graf düðümünü bulur ve queue'ya atar.*/
					findNode2(kelime1)->marked = 1;				/*Düðüm queue'ya girdiði için düðümü iþaretler. (Ýþlenmemiþ düðümler ile iþlenmeye baþlamamýþ düðümleri ayýrt etmek için gerekli.)*/
					tmpWord = findNode2(kelime1)->word;			/*Geçici deðiþkene kelimeyi alýr.*/
					while(!isEmpty(q) && strcmp(tmpWord, kelime2) != 0) {	/*Aranan kelime bulunamadýðý veya queue boþalmadýðý sürece döngü devam eder.*/
						tmpNode = dequeue(q);								/*En öndeki düðüm queue'dan çýkartýlýr.*/
						tmpWord = tmpNode->word;
						/*printf("\nSu anda queue'dan cekilen kelime: %s\n", tmpWord);*/
						if(strcmp(tmpWord, kelime2) != 0) {					/*Eðer istenen kelime bulunmadýysa çýkartýlan düðümün gezilmemiþ komþularý queue'ya eklenir ve iþaretlenir.*/
							for(j = 0; j < N; j++) {
								if(matris[tmpNode->nodeNo][j] == 1 && findNode(j)->marked == 0) {
									/*printf("\n	Su anda queue'ya atilan kelime: %s\n", findNode(j)->word);*/
									enqueue(q, findNode(j), tmpNode);
									findNode(j)->marked = 1;
								} 
							}
						}
					}														/*Eðer istenen kelime bulunduysa döngü biter. Ýstenen kelimeye ait düðüm, tmpNode üzerinde oluþur.*/
					
					if(strcmp(tmpWord, kelime2) == 0) {	
						i = 0;
						while(tmpNode->parent != NULL) {		/*Yolu parentlarý tutarak kaybetmemiþtim. Düðümlerin childlarýný doldurarak ileri yönde de takip edebilmeyi saðlýyorum.*/
							tmpNode->parent->child = tmpNode;
							tmpNode = tmpNode->parent;
							i++;
						}
						i++;
						
						printf("\nDonusum bulundu! Adim sayisi %d:\n", i);
						while(tmpNode != NULL) {				/*Yapýlan geçiþleri sýrasýyla yazdýrýyorum.*/
							printf("%s , ", tmpNode->word);
							tmpNode = tmpNode->child;
						}
					} else {
						printf("\nDonusum mevcut degil.\n");
					}
					emptyQueue(q);
					resetMarks();
					/*BFS bitiþi.*/
					break;
				case 2:	/*Baglanti var mi?*/
					/*Baðlantý kontrolü baþlangýcý.*/
					printf("\n");
					do {
						printf("Kelime1: ");
						scanf("%s", kelime1);
					} while(findNode2(kelime1) == NULL);	/*Kelime dosyada mevcut olmadýðý sürece istemeye devam eder.*/
					do {
						printf("Kelime2: ");
						scanf("%s", kelime2);
					} while(findNode2(kelime2) == NULL);	/*Kelime dosyada mevcut olmadýðý sürece istemeye devam eder.*/
					if(checkGraph(matris, kelime1, kelime2) == 1) {		/*Matris kontrol ediliyor. Eðer 1 ise baðlantý vardýr.*/
						printf("\nBaglanti mevcut!");
					} else {
						printf("\nBaglanti mevcut degil!");
					}
					/*Baðlantý kontrolü bitiþi.*/
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
