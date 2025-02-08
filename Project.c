#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50   // Bir kelimedeki max harf sayisi

// Kelime dugumu olusturur
struct wordNode{
    char data[MAX];
    int control;      // Kelime onceden gezilip gezilmedigini kontrol ediyor. Gezilmisse 1, gezilmemisse 0 olacak.
    struct wordNode* next;
};

// Queue icin dugum olusturur
struct queueNode{
	struct Stack* stack;
	struct queueNode* next;
};

// Stack icin linkli liste icin dugum
struct Node{
    char data[MAX];
    struct Node* next;
};

// Stack yapisi
struct Stack{
    struct Node* top;
};

// Queue yapisi
struct Queue{
    struct queueNode* front;
    struct queueNode* rear;
};

// Prototipler
int sourceControl(struct wordNode** head, char* word);
void copyStack(struct Stack* stack1, struct Stack* stack2);
struct wordNode* newWordNode(char* data);
void addList(struct wordNode** head, char* word);
struct Node* newNode(char* data);
struct Stack* createStack();
struct Queue* createQueue();
int isEmptyStack(struct Stack* stack);
int isEmptyQueue(struct Queue* queue);
void printList(struct wordNode* head);
void push(struct Stack* stack, char* data);
void enqueue(struct Queue* queue, struct Stack* stack);
char* pop(struct Stack* stack);
struct Stack* dequeue(struct Queue* queue);
void printStack(struct Stack* stack);
void clearStack(struct Stack* stack);
void clearQueue(struct Queue* queue);
void clearWordList(struct wordNode* head);
void readFile(struct wordNode** head, int wordLength);
void compare(struct wordNode** head, int wordLength, struct Queue* queue,char* source,char* target);


int main(){
    struct wordNode* head = NULL;
    struct Queue* queue = createQueue();
    char source[MAX], target[MAX];
    int wordLength;       
    do{
    	do{
    		printf("Kaynak kelimenizi yaziniz: \n");
		    scanf("%s", source);
		    wordLength = strlen(source);             //Kanak kelimenin harf sayisini ogreniyoruz.
		    readFile(&head, wordLength);             //Sozlugu linked list yapimiza aktariyoruz.
		    printf("Hedef kelimenizi yaziniz: \n");
		    scanf("%s", target);
		    if(sourceControl(&head,source)==1){
		    	printf("Kaynak kelime sozlukte yok. Tekrardan kaynak kelime giriniz.\n");
			}
		}while(sourceControl(&head,source)==1);
	    if(strlen(source)!=strlen(target)){
	    	printf("Girdiginiz iki kelimenin harf sayisi esit degil. Tekrardan kelimeleri giriniz.\n");
		}
	}while(strlen(source)!=strlen(target));	
	struct Stack* stack1 = createStack();      //Gecici stack olusturuyoruz. 
	push(stack1, source);               //Ilk kelime stacke atiyoruz.
	enqueue(queue, stack1);             //Stack baslangic icin siraya ekliyoruz.
	compare(&head, wordLength, queue, source, target);       
	clearWordList(head);         //Sozluk temizleniyor.
	clearQueue(queue);           //Queue yapimiz icinde stacklerle beraber temizleniyor.
    return 0;
}

//  Kaynak kelime sozlukte yoksa bir daha kelime istiyoruz.
int sourceControl(struct wordNode** head, char* word){
	struct wordNode* current=*head;
    while (current!=NULL) {
        if(strcmp(current->data,word)==0){
        	return 0;
		}
        current=current->next;
    }
    return 1;
}

// Bir stack baska bir stacke kopyalayan fonksiyon
void copyStack(struct Stack* stack1, struct Stack* stack2) {
	char tmp[MAX];
    struct Stack* temp = createStack();
    while(!isEmptyStack(stack1)){
    	strcpy(tmp,pop(stack1));
    	push(temp,tmp);
	}
	while(!isEmptyStack(temp)){
    	strcpy(tmp,pop(temp));
    	push(stack1,tmp);
    	push(stack2,tmp);
	}
}

// Yeni bir kelime dugumu olusturur
struct wordNode* newWordNode(char* data){
    struct wordNode* node = (struct wordNode*)malloc(sizeof(struct wordNode));
    if (!node) {
        printf("Bellek yetersiz\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->data, data);
    node->control = 0;
    node->next = NULL;
    return node;
}

// Kelime linkli listesine eleman ekler(Sozluk icin)
void addList(struct wordNode** head, char* word){
    struct wordNode* newNode = newWordNode(word);
    // Eðer liste bossa, yeni dugumu basa ekle
    if (*head == NULL) {
        *head = newNode;
        (*head)->control=0;
        return;
    }
    struct wordNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
        temp->control=0;
    }
    temp->next = newNode;
}

// Yeni bir stack dugum oluþturur
struct Node* newNode(char* data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if (!node){
        printf("Bellekte yer yok\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->data, data);
    node->next = NULL;
    return node;
}

// Yeni bir queue dugumu olusturma fonksiyonu
struct queueNode* createQueueNode(struct Stack* stack) {
    struct queueNode* newNode = (struct queueNode*)malloc(sizeof(struct queueNode));
    newNode->stack = stack;
    newNode->next = NULL;
    return newNode;
}

// Stack olusturur
struct Stack* createStack(){
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    if (!stack){
        printf("Bellekte yer yok\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    return stack;
}

// Yeni bir queue olusturma fonksiyonu
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Stack doluluk durumuna bakar
int isEmptyStack(struct Stack* stack){
    return (stack->top == NULL);
}

// Queue bos mu dolu mu diye bakar
int isEmptyQueue(struct Queue* queue){
    return (queue->front == NULL);
}

// Kelime linkli listesini yazdirma
void printList(struct wordNode* head){
    struct wordNode* current=head;
    printf("Kelime listesi: ");
    while (current!=NULL) {
        printf("%s kontrol:%d\n",current->data,current->control);
        current=current->next;
    }
    printf("\n");
}

// Stack'e eleman atar
void push(struct Stack* stack, char* data){
    struct Node* node = newNode(data);
    node->next = stack->top;
    stack->top = node;
}

// Queue eleman ekleme (enqueue) fonksiyonu
void enqueue(struct Queue* queue, struct Stack* stack) {
    struct queueNode* newNode = createQueueNode(stack);
    // Eger kuyruk bossa, yeni dugumu hem front hem de rear olarak ayarla
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // Aksi halde, yeni dugumu mevcut son dugumun sonrasina ekle ve rear'i guncelle
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Kuyruktan eleman cikarma (dequeue) fonksiyonu
struct Stack* dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue bos! Eleman cikartilamaz.\n");
        return NULL;
    } else {
        // Eger kuyruk sadece bir dugumden olusuyorsa, front ve rear'ý NULL yaparak queue bosalt
        if (queue->front == queue->rear) {
            struct queueNode* temp = queue->front;
            struct Stack* stack = temp->stack;
            queue->front = NULL;
            queue->rear = NULL;
            free(temp);
            return stack;
        } else {
            // Aksi halde, front'u guncelle ve eski front dugumunu kuyruktan cikar
            struct queueNode* temp = queue->front;
            struct Stack* stack = temp->stack;
            queue->front = queue->front->next;
            free(temp);
            return stack;
        }
    }
}

// Stack eleman cikarir
char* pop(struct Stack* stack){
    if (isEmptyStack(stack)){
        printf("Yigin bos\n");
        exit(EXIT_FAILURE);
    }
    struct Node* temp = stack->top;
    char* data = (char*)malloc(MAX * sizeof(char));
    if (!data){
        printf("Bellekte yer yok\n");
        exit(EXIT_FAILURE);
    }
    strcpy(data, temp->data);
    stack->top = temp->next;
    free(temp);
    return data;
}



// Stack asagidan yukariya dogru yazdirir.
void printStack(struct Stack* stack) {
    if (isEmptyStack(stack)) {
        printf("Yigin bos\n");
        return;
    }
    printf("Kuyruktan cektiginiz son yigin icindeki asagidan yukariya dogru elemanlar: \n");   
    struct Stack* tempStack = createStack(); // Gecici yigin olusturuldu.
    while (!isEmptyStack(stack)) {
        push(tempStack, pop(stack)); // Yigini ters cevirerek gecici yigina aktardik.
    }
    while (!isEmptyStack(tempStack)) {
        printf("%s -> ", tempStack->top->data);
        push(stack,pop(tempStack));
    }
    printf("\n");
    free(tempStack); 
}


// Stack'teki tum elemanlari temizler
void clearStack(struct Stack* stack){
    while (stack->top != NULL){
        struct Node* temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
    free(stack);
}

// Queue'daki tum elemanlari temizler
void clearQueue(struct Queue* queue){
    while (queue->front != NULL){
        struct queueNode* temp = queue->front;
        queue->front = queue->front->next;
        clearStack(temp->stack); // Stack icindeki elemanlari temizle
        free(temp);
    }
    free(queue);
}

// Sozluk linkli listesini temizler
void clearWordList(struct wordNode* head) {
    struct wordNode* current = head;
    while (current != NULL) {
        struct wordNode* temp = current;
        current = current->next;
        free(temp);
    }
}


// Dosyadan okuma ve sozluk olusturma fonksiyonu
void readFile(struct wordNode** head, int wordLength) {
    char line[MAX];
    FILE* file = fopen("dictionary.txt", "r");
    if (file == NULL) {
        printf("Dosya acilamadi\n");
        exit(EXIT_FAILURE);
    } 
    struct wordNode* current = *head;
    while (fgets(line, sizeof(line), file)){
        line[strcspn(line, "\n")] = '\0'; // Satir sonundaki yeni satir karakterini kaldirir
        if (strlen(line) == wordLength){ // Eger kelimenin uzunlugu hedef uzunluga esitse
        	addList(head,line);  //Sozluk icin olusturdugumuz linkli listeye atar. (Tum sozlugu atmiyorum. Zaman ve bellekten tasarruf icin
        }                        //source ile ayni uzunluktaki kelimeleri listeye atiyorum.
    }
    fclose(file);
}

// Tum islemlerin yapildigi fonksiyon aslinda bu. Kuyruktan stack cekiyorum ve top ile target kelimemi kiyasliyorum.
void compare(struct wordNode** head, int wordLength, struct Queue* queue, char* source, char* target) {
    if (*head == NULL) {
        printf("Hata: head NULL\n");
        return;
    }
    struct wordNode* current = *head;    //Sozluge ulasiyorum.
    struct Stack* stack2;                //Dequeue yaptigim stack kiyaslamak icin gecici stack olusturuyorum.
    do {
        if (!isEmptyQueue(queue)) {       //Kuyruk bos degilse 
            stack2 = dequeue(queue);      //Kuyruktan yigin cekiyorum ve gecici yiginima esitliyorum.
            printStack(stack2);           //Elimde olan yigini yazdiriyorum.
            while (current != NULL) {     //Sozluk geziyoruz.
                int i = 0, diffCount = 0;         //i donguyu dondermek icin, diffCount ise farkli harf sayisini sayiyor.
                while ((diffCount <= 1) && (i < wordLength)) {         //Kelimedeki harf sayisi ya da farkli harf sayisi 1'i gecene kadar dongude geziyorum.
                    if (current->data[i] != stack2->top->data[i]) {     //Elimdeki stackte ve sozlukteki kelimelerin harflerini tek tek kiyasliyorum.
                        diffCount++;                                    //Farkli ise diffCount 1 artiyor.
                    }
                    i++;                                                //Dongu donuyor.
                }
                if (diffCount == 1) {                                   //Eger farkli harf sayisi 1 ise
                    if (current->control == 0) {                        //Kelime onceden baska stacke atilmis mi diye kontrol ediyorum. Eger etmesem sonsuz donguye girer.
                        current->control = 1;                           //Kelime onceden baska stackte degilse artik atacagim icin control 1 yapiyorum.
                        struct Stack* stack = createStack();            //Siraya eklemek icin yeni stack olusturuyorum.
                        copyStack(stack2, stack);                       //Onceki stacki kopyaliyorum.
                        push(stack, current->data);                     //Ek olarak yeni ekleyecegim kelimeyi burada ekliyorum.
                        enqueue(queue, stack);                          //Ve yeni kelime eklenmis stacki siraya ekliyorum.
                    }
                }
                current = current->next;                                //Sozlukte diger kelimelere baksin diye ilerliyorum.
            }
            current=*head;                          //Baska stack en ustteki kelime kiyaslamasi icin sozlugun en basina donmesini sagliyorum.
        }
    } while ((!isEmptyQueue(queue)) && (strcmp(stack2->top->data, target) != 0));       //Sira bosalana kadar ya da hedef kelime bulunana kadar geziyorum.
    if ((strcmp(stack2->top->data, target) == 0)) {                      //Hedef kelime bulunursa elimdeki stacki asagidan yukari dogru yazdiriyorum.
        printf("En kisa yol bulundu.\n");
        printStack(stack2);
    } else {
        printf("Iki kelime arasi yol yok.\n");                          //Bulunamadiysa iki kelime arasi yol olmadigini anliyorum.
    }
}
