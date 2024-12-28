#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//NOT: Ekran temizlemek için system("CLS") kullandým. Kodu Windows iþletim sisteminde yazdým!

struct PERSON {
	int ID;
	int* friends;
	char* name;
	struct PERSON* left;
	struct PERSON* right;
}*root;

int s = 0;

typedef struct PERSON PERSON;

//Yeni bir PERSON oluþturur.
PERSON* createTree(int ID){ 
	PERSON* p = (PERSON*) malloc(sizeof(PERSON));
	if(root == NULL){
		root = p;
	}
	p->ID = ID;
	p->friends = (int*) malloc(sizeof(int));
	p->name = (char*) malloc(sizeof(char));
	p->name[0] = '\0'; //Elle kullanýcý eklenirse oluþacak hatalarý önlemek için gerekli.
	p->left = NULL;
	p->right = NULL;
	s++; //Yeni kiþi eklendiðinden size 1 artar.
	return p;
}

//Gönderilen PERSON'un ID ve ismini yazar.
void visit(PERSON* p){
	printf("No: %d, Name: %s\n", p->ID, p->name);
}

//Gönderilen aðacýn en küçük deðerini bulup geri döndürür.
PERSON* minValueNode(PERSON* p){
	PERSON* current = p;
	while(current->left != NULL){
		current = current->left;
	}
	return current;
}

//Gönderilen aðacý inOrder olarak yazdýrýr.
void printInOrder(PERSON* p){
	if(p == NULL)
		return;
	printInOrder(p->left);
	visit(p);
	printInOrder(p->right);
}

//ID'si verilen kiþiyi arar.(Fonksiyon çaðýrýlýrken root gönderilir.)
PERSON* findPerson(PERSON* p, int ID){
	if(p == NULL || p->ID == ID)
		return p;
	if(ID < p->ID){
		return findPerson(p->left, ID);
	} else if(ID > p->ID){
		return findPerson(p->right, ID);
	}
}

//ID'si verilen kiþinin alt aðacýný yazdýrýr.
void printNext(int ID){
	PERSON* p = findPerson(root, ID);
	if(p == NULL)
		return;
	printInOrder(p->left); //Kendisini yazdýrmamak için sol ve sað aðaçlarý ayrý ayrý yazdýrýr.
	printInOrder(p->right);
}

//
void printGreater(PERSON* p, int ID){
	if(p == NULL)
		return;
	if(p->ID <= ID){
		printGreater(p->right, ID);  //p ID'den küçük eþit ise sol tarafý atlayýp direk saða geç.
	} else { //p ID'den büyük ise p'yi ve p'nin sað kolunu yazdýr. Sonra p'nin soluna ilerle.
		visit(p);
		printInOrder(p->right);
		printGreater(p->left, ID);
	}
}

//ID'si verilen kiþi için bir PERSON oluþturup aðaca ekler. (Friends ve Name özellikleri readInput fonksiyonunda yüklenir.)
//Çaðýrýlýrken p'ye root gönderilir.
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

//ID'si verilen kiþiyi aðaçtan siler.
PERSON* deleteUser(PERSON* p, int ID){
	if(p == NULL)
		return p;
	if(ID < p->ID){
		p->left = deleteUser(p->left, ID);
	} else if(ID > p->ID){
		p->right = deleteUser(p->right, ID);
	} else {
		if(p->left == NULL){ //Sað kolun dolu olma durumu.
			PERSON* tmp = p->right;
			s--; //Kiþi sayýsý 1 azaldýðýndan size'ý 1 azaltýr.
			free(p);
			return tmp;
		} else if(p->right == NULL){ //Sol kolun dolu olma durumu.
			PERSON* tmp = p->left;
			s--; //Kiþi sayýsý 1 azaldýðýndan size'ý 1 azaltýr.
			free(p);
			return tmp;
		} else { //2 Kolun da dolu olma durumu. (Sað alt aðacýn en küçük ID'li PERSON'u silinen kiþinin yerine konur.)
			PERSON* tmp = minValueNode(p->right);
			p->ID = tmp->ID;
			p->right = deleteUser(p->right, tmp->ID);
		}
	}
	return p;
}

//ID'si verilen kiþi varsa yazdýrýlýr, yoksa hiçbir þey yapýlmaz.
void contains(PERSON* p, int ID){
	if(p == NULL)
		return;
	if(ID < p->ID){ //Aranan ID gönderilen PERSON'un ID'sinden küçükse sol alt aðaçta aramaya devam eder.
		contains(p->left, ID);
	} else if(ID > p->ID){ //Aranan ID gönderilen PERSON'un ID'sinden büyükse sað alt aðaçta aramaya devam eder.
		contains(p->right, ID);
	} else { //Aranan ID bulundu.
		printf("Name: %s\n", p->name);
	}
	return;
}

//Aðacýn boyutunu döndürür.
int size(){
	return s;
}

//ID'si verilen kiþinin arkadaþlarýný yazdýrýr.
void friends(int ID){
	PERSON* p = findPerson(root, ID); //ID'si verilen kiþiyi bulur.
	if(p == NULL || p->friends == NULL)
		return;
	int i = 0;
	while(findPerson(root, p->friends[i])){ //Arkadaþ listesindeki ID'ler için kiþileri bulur ve isimlerini yazdýrýr.
		printf("Name: %s\n", findPerson(root, p->friends[i])->name);
		i++;
	}
	
}

//Verilen dosyadan aðacýn oluþturulmasý
void readInput(FILE* fp){
	int len = 100; //Satýr satýr okunacak maksimum karakter.
	char* s = (char*) malloc(sizeof(char) * len);
	
	int i;
	int j;
	int k;
	int tempID; //String'den int e çevirirken kullandýðým geçici deðiþken.
	
	PERSON* p;
	
	getc(fp);
	getc(fp); // Input dosyasýnýn baþýnda 3 farklý karakter geliyor. Onlarý geçmek için kullandým.
	getc(fp);
	while(!feof(fp)){
		fgets(s, 100, fp);
		if(s[0] == '\n')
			continue; //Dosyanýn en altýndaki fazla new line karakterlerini okumamak için yazdým.
		p = (PERSON*) malloc(sizeof(PERSON));
		p->name = (char*) malloc(sizeof(char)); //Dosyadan okuduðum deðerleri tuttuðum geçici PERSON.
		p->friends = (int*) malloc(sizeof(int));
		i = 0;
		j = 0;
		k = 0;
		
		//ID'yi alan kýsým.
		tempID = s[0] - '0';
		i++;
		if(s[i] != ','){
			tempID *= 10;
			tempID += s[i] - '0';
			i++;
		}
		p->ID = tempID;
		i++; //virgülü geçmek için.
		//___________________________
		
		//Ýsmi alan kýsým
		while(s[i] != ',' && s[i] != '\n'){
			p->name = (char*) realloc(p->name, sizeof(char) * (j+2));
			p->name[j] = s[i];
			i++;
			j++;
		}
		p->name[j] = '\0';
		//_______________________
		
		if(s[i] == ','){ //isimden sonra ',' varsa friends listesi boþ deðildir.
			i++;
			while(s[i] != '\n'){ //Alýnan line bitene kadar arkadaþlarý okur.
				if(s[i] != '-'){ //Sayý var demek.
					tempID = s[i] - '0';
					i++;
					if(s[i] == '-'){ // '-' geldi yani sayý bitti. Geçici PERSON'daki friends'e eklendi.
						p->friends = (int*) realloc(p->friends, sizeof(int) * (k+1));
						p->friends[k] = tempID;
						k++;
					} else if(s[i] <= '9' && s[i] >= '0'){ //Sayý 2 basamaklý olduðu için çevirmeye devam ediyor.
						tempID *= 10;
						tempID += s[i] - '0';
					}
				} else { //'-' geldi yani sayý bitti. Geçici PERSON'daki friends'e eklendi.
					p->friends = (int*) realloc(p->friends, sizeof(int) * (k+1));
					p->friends[k] = tempID;
					k++;
				}
				i++;
			}
			p->friends = (int*) realloc(p->friends, sizeof(int) * (k+1)); //Line bittikten sonra eksik kalan arkadaþý da ekliyor.
			p->friends[k] = tempID;
			k++;
			
			insertNewUser(root, p->ID);
			PERSON* tmp = findPerson(root, p->ID);
			tmp->name = p->name; //Geçici PERSON'daki bilgileri asýl PERSON'a yazma.
			tmp->friends = p->friends;
			
		} else { //isimden sonra ',' yok, friends listesi boþ.
			insertNewUser(root, p->ID);
			PERSON* tmp = findPerson(root, p->ID);
			tmp->name = p->name; //Geçici PERSON'daki ismi asýl PERSON'a yazma.
		}
		free(p); //Geçici PERSON'u içini boþaltmak için free yapýyor.
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
	readInput(fp); //Aðaç oluþturuldu.
	
	int ID; //girilecek ID için deðiþken.
	int fNo; //seçim deðiþkeni.
	
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
