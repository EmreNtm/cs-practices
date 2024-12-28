#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//NOT: Ekran temizlemek i�in system("CLS") kulland�m. Kodu Windows i�letim sisteminde yazd�m!

struct PERSON {
	int ID;
	int* friends;
	char* name;
	struct PERSON* left;
	struct PERSON* right;
}*root;

int s = 0;

typedef struct PERSON PERSON;

//Yeni bir PERSON olu�turur.
PERSON* createTree(int ID){ 
	PERSON* p = (PERSON*) malloc(sizeof(PERSON));
	if(root == NULL){
		root = p;
	}
	p->ID = ID;
	p->friends = (int*) malloc(sizeof(int));
	p->name = (char*) malloc(sizeof(char));
	p->name[0] = '\0'; //Elle kullan�c� eklenirse olu�acak hatalar� �nlemek i�in gerekli.
	p->left = NULL;
	p->right = NULL;
	s++; //Yeni ki�i eklendi�inden size 1 artar.
	return p;
}

//G�nderilen PERSON'un ID ve ismini yazar.
void visit(PERSON* p){
	printf("No: %d, Name: %s\n", p->ID, p->name);
}

//G�nderilen a�ac�n en k���k de�erini bulup geri d�nd�r�r.
PERSON* minValueNode(PERSON* p){
	PERSON* current = p;
	while(current->left != NULL){
		current = current->left;
	}
	return current;
}

//G�nderilen a�ac� inOrder olarak yazd�r�r.
void printInOrder(PERSON* p){
	if(p == NULL)
		return;
	printInOrder(p->left);
	visit(p);
	printInOrder(p->right);
}

//ID'si verilen ki�iyi arar.(Fonksiyon �a��r�l�rken root g�nderilir.)
PERSON* findPerson(PERSON* p, int ID){
	if(p == NULL || p->ID == ID)
		return p;
	if(ID < p->ID){
		return findPerson(p->left, ID);
	} else if(ID > p->ID){
		return findPerson(p->right, ID);
	}
}

//ID'si verilen ki�inin alt a�ac�n� yazd�r�r.
void printNext(int ID){
	PERSON* p = findPerson(root, ID);
	if(p == NULL)
		return;
	printInOrder(p->left); //Kendisini yazd�rmamak i�in sol ve sa� a�a�lar� ayr� ayr� yazd�r�r.
	printInOrder(p->right);
}

//
void printGreater(PERSON* p, int ID){
	if(p == NULL)
		return;
	if(p->ID <= ID){
		printGreater(p->right, ID);  //p ID'den k���k e�it ise sol taraf� atlay�p direk sa�a ge�.
	} else { //p ID'den b�y�k ise p'yi ve p'nin sa� kolunu yazd�r. Sonra p'nin soluna ilerle.
		visit(p);
		printInOrder(p->right);
		printGreater(p->left, ID);
	}
}

//ID'si verilen ki�i i�in bir PERSON olu�turup a�aca ekler. (Friends ve Name �zellikleri readInput fonksiyonunda y�klenir.)
//�a��r�l�rken p'ye root g�nderilir.
PERSON* insertNewUser(PERSON* p, int ID){
	if (p == NULL)
		return createTree(ID);
	if (ID < p->ID) {
		p->left = insertNewUser(p->left, ID);
	} else if(ID > p->ID){
		p->right = insertNewUser(p->right, ID);
	}
	return p;
}

//ID'si verilen ki�iyi a�a�tan siler.
PERSON* deleteUser(PERSON* p, int ID){
	if(p == NULL)
		return p;
	if(ID < p->ID){
		p->left = deleteUser(p->left, ID);
	} else if(ID > p->ID){
		p->right = deleteUser(p->right, ID);
	} else {
		if(p->left == NULL){ //Sa� kolun dolu olma durumu.
			PERSON* tmp = p->right;
			s--; //Ki�i say�s� 1 azald���ndan size'� 1 azalt�r.
			free(p);
			return tmp;
		} else if(p->right == NULL){ //Sol kolun dolu olma durumu.
			PERSON* tmp = p->left;
			s--; //Ki�i say�s� 1 azald���ndan size'� 1 azalt�r.
			free(p);
			return tmp;
		} else { //2 Kolun da dolu olma durumu. (Sa� alt a�ac�n en k���k ID'li PERSON'u silinen ki�inin yerine konur.)
			PERSON* tmp = minValueNode(p->right);
			p->ID = tmp->ID;
			p->right = deleteUser(p->right, tmp->ID);
		}
	}
	return p;
}

//ID'si verilen ki�i varsa yazd�r�l�r, yoksa hi�bir �ey yap�lmaz.
void contains(PERSON* p, int ID){
	if(p == NULL)
		return;
	if(ID < p->ID){ //Aranan ID g�nderilen PERSON'un ID'sinden k���kse sol alt a�a�ta aramaya devam eder.
		contains(p->left, ID);
	} else if(ID > p->ID){ //Aranan ID g�nderilen PERSON'un ID'sinden b�y�kse sa� alt a�a�ta aramaya devam eder.
		contains(p->right, ID);
	} else { //Aranan ID bulundu.
		printf("Name: %s\n", p->name);
	}
	return;
}

//A�ac�n boyutunu d�nd�r�r.
int size(){
	return s;
}

//ID'si verilen ki�inin arkada�lar�n� yazd�r�r.
void friends(int ID){
	PERSON* p = findPerson(root, ID); //ID'si verilen ki�iyi bulur.
	if(p == NULL || p->friends == NULL)
		return;
	int i = 0;
	while(findPerson(root, p->friends[i])){ //Arkada� listesindeki ID'ler i�in ki�ileri bulur ve isimlerini yazd�r�r.
		printf("Name: %s\n", findPerson(root, p->friends[i])->name);
		i++;
	}
	
}

//Verilen dosyadan a�ac�n olu�turulmas�
void readInput(FILE* fp){
	int len = 100; //Sat�r sat�r okunacak maksimum karakter.
	char* s = (char*) malloc(sizeof(char) * len);
	
	int i;
	int j;
	int k;
	int tempID; //String'den int e �evirirken kulland���m ge�ici de�i�ken.
	
	PERSON* p;
	
	getc(fp);
	getc(fp); // Input dosyas�n�n ba��nda 3 farkl� karakter geliyor. Onlar� ge�mek i�in kulland�m.
	getc(fp);
	while(!feof(fp)){
		fgets(s, 100, fp);
		if(s[0] == '\n')
			continue; //Dosyan�n en alt�ndaki fazla new line karakterlerini okumamak i�in yazd�m.
		p = (PERSON*) malloc(sizeof(PERSON));
		p->name = (char*) malloc(sizeof(char)); //Dosyadan okudu�um de�erleri tuttu�um ge�ici PERSON.
		p->friends = (int*) malloc(sizeof(int));
		i = 0;
		j = 0;
		k = 0;
		
		//ID'yi alan k�s�m.
		tempID = s[0] - '0';
		i++;
		if(s[i] != ','){
			tempID *= 10;
			tempID += s[i] - '0';
			i++;
		}
		p->ID = tempID;
		i++; //virg�l� ge�mek i�in.
		//___________________________
		
		//�smi alan k�s�m
		while(s[i] != ',' && s[i] != '\n'){
			p->name = (char*) realloc(p->name, sizeof(char) * (j+2));
			p->name[j] = s[i];
			i++;
			j++;
		}
		p->name[j] = '\0';
		//_______________________
		
		if(s[i] == ','){ //isimden sonra ',' varsa friends listesi bo� de�ildir.
			i++;
			while(s[i] != '\n'){ //Al�nan line bitene kadar arkada�lar� okur.
				if(s[i] != '-'){ //Say� var demek.
					tempID = s[i] - '0';
					i++;
					if(s[i] == '-'){ // '-' geldi yani say� bitti. Ge�ici PERSON'daki friends'e eklendi.
						p->friends = (int*) realloc(p->friends, sizeof(int) * (k+1));
						p->friends[k] = tempID;
						k++;
					} else if(s[i] <= '9' && s[i] >= '0'){ //Say� 2 basamakl� oldu�u i�in �evirmeye devam ediyor.
						tempID *= 10;
						tempID += s[i] - '0';
					}
				} else { //'-' geldi yani say� bitti. Ge�ici PERSON'daki friends'e eklendi.
					p->friends = (int*) realloc(p->friends, sizeof(int) * (k+1));
					p->friends[k] = tempID;
					k++;
				}
				i++;
			}
			p->friends = (int*) realloc(p->friends, sizeof(int) * (k+1)); //Line bittikten sonra eksik kalan arkada�� da ekliyor.
			p->friends[k] = tempID;
			k++;
			
			insertNewUser(root, p->ID);
			PERSON* tmp = findPerson(root, p->ID);
			tmp->name = p->name; //Ge�ici PERSON'daki bilgileri as�l PERSON'a yazma.
			tmp->friends = p->friends;
			
		} else { //isimden sonra ',' yok, friends listesi bo�.
			insertNewUser(root, p->ID);
			PERSON* tmp = findPerson(root, p->ID);
			tmp->name = p->name; //Ge�ici PERSON'daki ismi as�l PERSON'a yazma.
		}
		free(p); //Ge�ici PERSON'u i�ini bo�altmak i�in free yap�yor.
	}
}

//insertNewUser() +.
//deleteUser() +.
//contains() +.
//friends() +.
//size() +.
//printNext() +.
//printGreater() +.
//printInOrder() +.

int main(){
	root = NULL;
	FILE* fp;
	fp = fopen("Input.txt", "r");
	readInput(fp); //A�a� olu�turuldu.
	
	int ID; //girilecek ID i�in de�i�ken.
	int fNo; //se�im de�i�keni.
	
	do{
		printf("************************\n");
		printf("1: insertNewUser\n");
		printf("2: deleteUser\n");
		printf("3: contains\n");
		printf("4: friends\n");
		printf("5: size\n");
		printf("6: printNext\n");
		printf("7: printGreater\n");
		printf("8: printInOrder\n");
		printf("-1: Exit\n");
		printf("************************\n");
		printf("Function Number: ");
		scanf("%d", &fNo);
		system("CLS");
		
		switch (fNo) {
			case 1:
				printf("************************\n");
				printf("insertUser ()\n");
				printf("************************\n");
				printf("ID: ");
				scanf("%d", &ID);
				insertNewUser(root, ID);
				system("CLS");
				printf("************************\n");
				printf("%d inserted.\n", ID);
				break;
			case 2:
				printf("************************\n");
				printf("deleteUser\n");
				printf("************************\n");
				printf("ID: ");
				scanf("%d", &ID);
				deleteUser(root, ID);
				system("CLS");
				printf("************************\n");
				printf("%d deleted.\n", ID);
				break;
			case 3:
				printf("************************\n");
				printf("contains\n");
				printf("************************\n");
				printf("ID: ");
				scanf("%d", &ID);
				system("CLS");
				printf("************************\n");
				contains(root, ID);
				break;
			case 4:
				printf("************************\n");
				printf("friends\n");
				printf("************************\n");
				printf("ID: ");
				scanf("%d", &ID);
				system("CLS");
				printf("************************\n");
				friends(ID);
				break;
			case 5:
				system("CLS");
				printf("************************\n");
				printf("Size: %d\n", size());
				break;
			case 6:
				printf("************************\n");
				printf("printNext\n");
				printf("************************\n");
				printf("ID: ");
				scanf("%d", &ID);
				system("CLS");
				printf("************************\n");
				printNext(ID);
				break;
			case 7:
				printf("************************\n");
				printf("printGreater\n");
				printf("************************\n");
				printf("ID: ");
				scanf("%d", &ID);
				system("CLS");
				printf("************************\n");
				printGreater(root, ID);
				break;
			case 8:
				system("CLS");
				printf("************************\n");
				printInOrder(root);
				break;
		}	
	} while(fNo != -1);
	
	fclose(fp);
	system("PAUSE");
	return 0;
}
