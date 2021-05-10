#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED

struct Node{
    int Rank;
    char ChannelName[30];
    char Uploads[20];
    char Subscribers[20];
    char Views[20];
    struct Node* Right;
    struct Node* Left;
    int Height;
};

struct Node* MakeEmpty(struct Node* T){
    if(T != NULL){
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

int getHeight(struct Node* T) {
    if(T == NULL)
        return 0;
    return T->Height;
}

int getMax(int a, int b) {
    return (a > b)? a : b;
}

struct Node* getMin(struct Node* T)  {
    if(T == NULL)
        return NULL;
    else if(T->Left == NULL)
        return T;
    else
        return getMin(T->Left);
}

int CompareName(char key[30], char key2[30]){
    return strcmp(key2,key);
}

struct Node* Find(char ChannelName[30],struct Node* T){
    if(T == NULL)
        return NULL;
    else if(CompareName(ChannelName,T->ChannelName)==-1)
        return Find(ChannelName,T->Left);
    else if(CompareName(ChannelName,T->ChannelName)==1)
        return Find(ChannelName,T->Right);
    else
        return T;
};

int IsRepeated(struct Node* T,char ChannelName[30]){
    struct Node* N = Find(ChannelName,T);
    if(N != NULL && strcasecmp(N->ChannelName,ChannelName)==0)
        return 1;
    else
        return 0;
}

unsigned long convert(char str[20]){
    char result[100] = "";
    char* token;
    token = strtok(str,",");
    if(token == NULL)
        return 0;
    while(token != NULL) {
        strcat(result, token);
        token = strtok(NULL,",");
    }
     unsigned long n = atoi(result);

     return n;
}

void Reconvert(char* Subs){
    char *ptr, *optr;
    int i, length, commas;
    for ( ptr = Subs; *ptr; ptr++ );
    length = ptr - Subs;
    commas = ( length - 1 ) / 3;
    optr = Subs + length + commas;

    *optr-- = *ptr--;
    for(i=1;ptr>=Subs;i++){
        *optr-- = *ptr--;
        if((i%3)==0)
            *optr-- = ',';
    }
}

struct Node* Update(struct Node* T,char Uploads[20],char Subscribers[20],char Views[20]){
    unsigned long uploads = convert(T->Uploads);
    unsigned long subscribers = convert(T->Subscribers);
    unsigned long views = convert(T->Views);
    unsigned long New_Uploads = convert(Uploads);
    unsigned long New_Subscribers = convert(Subscribers);
    unsigned long New_Views = convert(Views);

    New_Uploads = New_Uploads + uploads;
    New_Subscribers = New_Subscribers + subscribers;
    New_Views = New_Views + views;

    sprintf(T->Uploads, "%lu", New_Uploads);
    sprintf(T->Subscribers, "%lu", New_Subscribers);
    sprintf(T->Views, "%lu\n", New_Views);

    Reconvert(T->Uploads);
    Reconvert(T->Subscribers);
    Reconvert(T->Views);
    return T;
}

struct Node* newNode(int Rank,char ChannelName[30],char Uploads[20],char Subscribers[20],char Views[20]){
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->Rank = Rank;
    strcpy(node->ChannelName,ChannelName);
    strcpy(node->Uploads,Uploads);
    strcpy(node->Subscribers,Subscribers);
    strcpy(node->Views,Views);
    node->Right = NULL;
    node->Left = NULL;
    node->Height = 1;
    return node;
}

struct Node* RotateRight(struct Node* T){
    struct Node* l = T->Left;
    struct Node* r = l->Right;

    l->Right = T;
    T->Left = r;

    T->Height = getMax(getHeight(T->Left),getHeight(T->Right))+1;
    l->Height = getMax(getHeight(l->Left),getHeight(l->Right))+1;
    return l;
}

struct Node* RotateLeft(struct Node* T){
    struct Node* r = T->Right;
    struct Node* l = r->Left;

    r->Left = T;
    T->Right = l;

    T->Height = getMax(getHeight(T->Left),getHeight(T->Right))+1;
    r->Height = getMax(getHeight(r->Left),getHeight(r->Right))+1;
    return r;
}

int getBalance(struct Node* N){
    if (N == NULL)
        return 0;
    return getHeight(N->Left) - getHeight(N->Right);
}

struct Node* Insert(struct Node* T,int Rank,char ChannelName[30],char Uploads[20],char Subscribers[20],char Views[20]){
    if(T == NULL)
        return newNode(Rank,ChannelName,Uploads,Subscribers,Views);

    else if(IsRepeated(T,ChannelName)){
        printf("Channel Already Exists!Data Updated!\n");
        struct Node* R = Find(ChannelName,T);
        R = Update(R,Uploads,Subscribers,Views);
        return R;
    }

    if(CompareName(ChannelName,T->ChannelName) == -1)
        T->Left = Insert(T->Left,Rank,ChannelName,Uploads,Subscribers,Views);
    else if(CompareName(ChannelName,T->ChannelName) == 1)
        T->Right = Insert(T->Right,Rank,ChannelName,Uploads,Subscribers,Views);
    else
        return T;

    T->Height = getMax(getHeight(T->Left),getHeight(T->Right))+1;
    int Balance = getBalance(T);
    //LL
    if(Balance > 1 && CompareName(ChannelName,T->Left->ChannelName)==-1)
        return RotateRight(T);
    //RR
    if(Balance < -1 && CompareName(ChannelName,T->Right->ChannelName))
        return RotateLeft(T);
    //LR
    if(Balance > 1 && CompareName(ChannelName,T->Left->ChannelName)==1){
        T->Left =  RotateLeft(T->Left);
        return RotateRight(T);
    }
    //RL
    if(Balance < -1 && CompareName(ChannelName,T->Right->ChannelName)==1){
        T->Right = RotateRight(T->Right);
        return RotateLeft(T);
    }
    return T;
}

struct Node* DeleteNode(struct Node* T,char ChannelName[30]){
    if(T == NULL)
        return T;
    if(CompareName(ChannelName,T->ChannelName)==-1)
        T->Left = DeleteNode(T->Left,ChannelName);
    else if(CompareName(ChannelName,T->ChannelName)==1)
        T->Right = DeleteNode(T->Right,ChannelName);
    else{
        if(T->Left == NULL || T->Right == NULL){
            struct Node* temp = T->Left ? T->Left : T->Right;

            if(temp == NULL){
                temp = T;
                T = NULL;
            }
            else
                *T = *temp;
            free(temp);
        }
        else{
            struct Node* temp = getMin(T->Right);
            T->Rank = temp ->Rank;
            strcpy(T->ChannelName,temp->ChannelName);
            strcpy(T->Uploads,temp->Uploads);
            strcpy(T->Subscribers,temp->Subscribers);
            strcpy(T->Views,temp->Views);
            T->Height = temp->Height;

            T->Right = DeleteNode(T->Right,temp->ChannelName);
        }
    }
    if(T == NULL)
        return T;

     T->Height = getMax(getHeight(T->Left),getHeight(T->Right))+1;

     int Balance = getBalance(T);

    //LL
    if (Balance > 1 && getBalance(T->Left) >= 0)
        return RotateRight(T);

    //LR
    if (Balance > 1 && getBalance(T->Left) < 0){
        T->Left = RotateLeft(T->Left);
        return RotateRight(T);
    }

    //RR
    if (Balance < -1 && getBalance(T->Right) <= 0)
        return RotateLeft(T);

    //RL
    if (Balance < -1 && getBalance(T->Right) > 0){
        T->Right = RotateRight(T->Right);
        return RotateLeft(T);
    }
    return T;
}

void PrintData(struct Node* node){
    if(node != NULL)
        printf("%d  %d  %s  %s  %s  %s\n",node->Height,node->Rank,node->ChannelName,node->Uploads,node->Subscribers,node->Views);
    else
        printf("Not Found!\n");
}

void PreOrder(struct Node* T){
    if(T != NULL){
        printf("%d  %2d  %s  %s  %s  %s",T->Height,T->Rank,T->ChannelName,T->Uploads,T->Subscribers,T->Views);
        PreOrder(T->Left);
        PreOrder(T->Right);
    }
}

struct Node* LoadData(struct Node * T){
    FILE *in;
    int Rank;
    char ChannelName[30];
    char Uploads[20];
    char Subscribers[20];
    char Views[20];
    char line[1000];
    in = fopen("Youtube+Channels.txt","r");
    while(fgets(line,1000,in) != 0){
        if(feof(in))
        break;
    else
            Rank = atoi(strtok(line,"\t"));
            strcpy(ChannelName,strtok(NULL,"\t"));
            strcpy(Uploads,strtok(NULL,"\t"));
            strcpy(Subscribers,strtok(NULL,"\t"));
            strcpy(Views,strtok(NULL,"\t"));
            T = Insert(T,Rank,ChannelName,Uploads,Subscribers,Views);
        }
        fclose(in);
        return T;
}

void SaveData(FILE *out,struct Node* T){
    if(T != NULL){
        fprintf(out,"%2d	%s	%s	%s	%s",T->Rank,T->ChannelName,T->Uploads,T->Subscribers,T->Views);
        SaveData(out,T->Left);
        SaveData(out,T->Right);
    }
}

typedef struct Hash *HashTable;
struct Hash{
    int Rank;
    char ChannelName[30];
    char Uploads[20];
    char Subscribers[20];
    char Views[20];
    int Flag;
};
int NumOfData = 0;

int IsPrime(int x){
    int i=2,flag=1;
    while(i<x){
        if((x%i)==0)
        return 0;
        i++;
    }
    return flag;
}

unsigned int HashFunction(char* key,int TableSize){
    unsigned int HashValue=0;
    while(*key != '\0')
        HashValue=(HashValue<<5)+ *key++;
    return (HashValue%TableSize);
}

void UpdateHash(HashTable hashtable[],int key,char Uploads[20],char Subscribers[20],char Views[20]){
    unsigned long uploads = convert(hashtable[key]->Uploads);
    unsigned long subscribers = convert(hashtable[key]->Subscribers);
    unsigned long views = convert(hashtable[key]->Views);
    unsigned long New_Uploads = convert(Uploads);
    unsigned long New_Subscribers = convert(Subscribers);
    unsigned long New_Views = convert(Views);

    New_Uploads = New_Uploads + uploads;
    New_Subscribers = New_Subscribers + subscribers;
    New_Views = New_Views + views;

    sprintf(hashtable[key]->Uploads, "%lu", New_Uploads);
    sprintf(hashtable[key]->Subscribers, "%lu", New_Subscribers);
    sprintf(hashtable[key]->Views, "%lu\n", New_Views);

    Reconvert(hashtable[key]->Uploads);
    Reconvert(hashtable[key]->Subscribers);
    Reconvert(hashtable[key]->Views);
}

int HashSearch(HashTable hashtable[],int HashSize,char ChannelName[30]){
    int key=0;
    int i=1;
    key = HashFunction(ChannelName,HashSize);
    while((strcmp(hashtable[key]->ChannelName,ChannelName)!=0)&&(i<=HashSize)){
        key=(key+(i*i))%HashSize;
        i++;
    }
    if(i>HashSize || hashtable[key]->Flag == 2)
        return -1;
    else
        return key;
}

void HashInsert(HashTable hashtable[],int HashSize ,int Rank,char ChannelName[30],char Uploads[20],char Subscribers[20],char Views[20]){
    int key=0;
    int i=1;

    int hashedkey = HashSearch(hashtable,HashSize,ChannelName);
    if(hashedkey != -1){
        UpdateHash(hashtable,hashedkey,Uploads,Subscribers,Views);
    }
    else{
    key = HashFunction(ChannelName,HashSize);
    while(hashtable[key]->Flag == 1){
        key=(key+(i*i))%HashSize;
        i++;
    }
        hashtable[key]->Rank = Rank;
        strcpy(hashtable[key]->ChannelName,ChannelName);
        strcpy(hashtable[key]->Uploads,Uploads);
        strcpy(hashtable[key]->Subscribers,Subscribers);
        strcpy(hashtable[key]->Views,Views);
        hashtable[key]->Flag = 1;
    }
}

int getNewHashSize(int n){
    int newHashSize = 2*n + 1;
    while(!IsPrime(newHashSize))
        newHashSize++;
    return newHashSize;
}

void Rehash(HashTable hashtable[], int hashsize){
    int oldhashsize = hashsize;
    int newhashsize = getNewHashSize(hashsize);
    hashsize = newhashsize;
    HashTable newhashtable[hashsize];
    for(int i=0;i<oldhashsize;i++){
        newhashtable[i] = malloc(sizeof(struct Hash));
        newhashtable[i]->Flag = 0;
        HashInsert(newhashtable,hashsize,hashtable[i]->Rank,hashtable[i]->ChannelName,hashtable[i]->Uploads,hashtable[i]->Subscribers,hashtable[i]->Views);
    }
    free(hashtable);
    hashtable = newhashtable;
}

void ReadHashFile(HashTable hashtable[],int HashSize){
    int i;
    FILE *inp;
    for(i=0;i<HashSize;i++){
        hashtable[i]=malloc(sizeof(struct Hash));
        hashtable[i]->Flag = 0;
    }
    int Rank;
    char ChannelName[30];
    char Uploads[20];
    char Subscribers[20];
    char Views[20];
    char line[1000];
    inp = fopen("Youtube+Channels.txt","r");
    while(fgets(line,1000,inp) != 0){
        if(feof(inp))
        break;
    else
            Rank = atoi(strtok(line,"\t"));
            strcpy(ChannelName,strtok(NULL,"\t"));
            strcpy(Uploads,strtok(NULL,"\t"));
            strcpy(Subscribers,strtok(NULL,"\t"));
            strcpy(Views,strtok(NULL,"\t"));
            NumOfData++;
            /*if(NumOfData > (HashSize/2))
                Rehash(hashtable,HashSize);
            else*/
            HashInsert(hashtable,HashSize,Rank,ChannelName,Uploads,Subscribers,Views);
    }
        fclose(inp);
}

void HashDelete(HashTable hashtable[],int HashSize,char ChannelName[]){
    int key=0;
    int i=1;
    key = HashFunction(ChannelName,HashSize);
    while(strcmp(hashtable[key]->ChannelName,ChannelName)!=0 && i<=HashSize){
        key=(key+i)%HashSize;
        i++;
    }
    if(i<HashSize || hashtable[key]->Flag == 2)
        hashtable[key]->Flag = 2;
}

void printHash(HashTable hashtable[],int HashSize){
    for(int i=0;i<HashSize;i++){
        if(hashtable[i]->Flag == 1)
            printf("At index: %d   %2d  %s  %s  %s  %s",i,hashtable[i]->Rank,hashtable[i]->ChannelName,hashtable[i]->Uploads,hashtable[i]->Subscribers,hashtable[i]->Views);
        else if(hashtable[i]->Flag == 0)
            printf("At index: %d        \n",i);
        }
}

void SaveHashData(FILE *outp, HashTable hashtable[], int HashSize){
    for(int i=0;i<HashSize;i++)
    if(hashtable[i]->Flag == 1)
        fprintf(outp,"%2d	%s	%s	%s	%s",hashtable[i]->Rank,hashtable[i]->ChannelName,hashtable[i]->Uploads,hashtable[i]->Subscribers,hashtable[i]->Views);
}

#endif // AVLTREE_H_INCLUDED
