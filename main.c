#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include "AVLTree.h"
int hashsize = 200;
int numOfData = 0;

struct Node* ReadData(struct Node* T){
    int Rank;
    char ChannelName[30];
    char Uploads[20];
    char Subscribers[20];
    char Views[20];
    char line[100];
    printf("Enter Rank, Channel Name, Uploads, Subscribers and Views:\n");
    printf("NOTE: put Tab between data!!\n");
    gets(line);
    Rank = atoi(strtok(line,"\t"));
    strcpy(ChannelName,strtok(NULL,"\t"));
    strcpy(Uploads,strtok(NULL,"\t"));
    strcpy(Subscribers,strtok(NULL,"\t"));
    strcpy(Views,strtok(NULL,"\t"));
    strcat(Views,"\n");
    printf("%d  %s  %s  %s  %s\n",Rank,ChannelName,Uploads,Subscribers,Views);
    struct Node* Y = Insert(T,Rank,ChannelName,Uploads,Subscribers,Views);
    return Y;
}

void ReadHashData(HashTable hashtable[], int HashSize){
    int Rank;
    char ChannelName[30];
    char Uploads[20];
    char Subscribers[20];
    char Views[20];
    char line[100];
    printf("Enter Rank, Channel Name, Uploads, Subscribers and Views:\n");
    printf("NOTE: put Tab between data!!\n");
    gets(line);
    Rank = atoi(strtok(line,"\t"));
    strcpy(ChannelName,strtok(NULL,"\t"));
    strcpy(Uploads,strtok(NULL,"\t"));
    strcpy(Subscribers,strtok(NULL,"\t"));
    strcpy(Views,strtok(NULL,"\t"));
    printf("%d  %s  %s  %s  %s\n",Rank,ChannelName,Uploads,Subscribers,Views);
    strcat(Views,"\n");
    /*if(numOfData > (HashSize/2))
        Rehash(hashtable,HashSize);
    else*/
        HashInsert(hashtable,HashSize,Rank,ChannelName,Uploads,Subscribers,Views);
}

void printMainMenu(){
    printf("----------------------------------------\n");
    printf("1- Open AVL Tree\n");
    printf("2- Open Hash\n");
    printf("3- Exit\n");
    printf("----------------------------------------\n");
    printf("Choose what to do:");
}

void printSubMenu1(){
    printf("----------------------------------------\n");
    printf("1- Print Channels\n");
    printf("2- Search\n");
    printf("3- Insert Channel\n");
    printf("4- Delete Channel\n");
    printf("5- Delete Tree\n");
    printf("6- Save Channels\n");
    printf("7- Calculate Tree Height\n");
    printf("8- Back to Main Menu\n");
    printf("----------------------------------------\n");
    printf("Choose what to do:");
}

void LoadSubMenu1(struct Node* T){
    FILE *out;
    int x;
    char ChannelNameToFind[30];
    char ChannelNameToDelete[30];
    printSubMenu1();
    scanf("%d", &x);
    getchar();
    printf("----------------------------------------\n");
    printf("\n");
    while(1){
        switch(x){
            case 1:
                if(T == NULL)
                    printf("No Data in the Tree\n");
                else
                    PreOrder(T);
                break;
            case 2:
                printf("Enter the Name of the Channel to find: ");
                gets(ChannelNameToFind);
                printf("\n");
                struct Node* y = Find(ChannelNameToFind,T);
                if(y == NULL)
                    printf("Channel is Not Found\n");
                else
                    PrintData(y);
                break;
            case 3:
                ReadData(T);
                break;
            case 4:
                printf("Enter the Name of the Channel to Delete: ");
                gets(ChannelNameToDelete);
                printf("\n");
                struct Node* d = DeleteNode(T,ChannelNameToDelete);
                if(d == NULL)
                    printf("Deletion Failed!!\n");
                else
                    printf("%s Deleted\n",ChannelNameToDelete);
                break;
            case 5:
                T = MakeEmpty(T);
                printf("Tree Deleted!\n");
                break;
            case 6:
                out = fopen("Youtube+Channels.txt","w");
                SaveData(out,T);
                fclose(out);
                break;
            case 7:
                printf("Tree Height = %d\n",getHeight(T));
                break;
            case 8:
                T = MakeEmpty(T);
                return;
                break;
            default:
                printf("\n\nInvalid Input! Try again! \n\n");
                break;
        }
        printSubMenu1();
        scanf("%d",&x);
        getchar();
        printf("----------------------------------------\n");
        printf("\n");
    }
}

void printSubMenu2(){
    printf("----------------------------------------\n");
    printf("1- Print Hashed Table\n");
    printf("2- Print Table Size\n");
    printf("3- Insert New Record\n");
    printf("4- Find Record\n");
    printf("5- Delete Record\n");
    printf("6- Save Records\n");
    printf("7- Back to Main Menu\n");
    printf("----------------------------------------\n");
    printf("Choose what to do:");
}

void LoadSubMenu2(HashTable hashtable[]){
    FILE *outp;
    int x;
    char ChannelNameToFind[30];
    char ChannelNameToDelete[30];
    printSubMenu2();
    scanf("%d", &x);
    getchar();
    printf("----------------------------------------\n");
    printf("\n");
    while(1){
        switch(x){
        case 1:
            printHash(hashtable,hashsize);
            break;
        case 2:
            printf("\nHash Table Size = %d\n\n",hashsize);
            break;
        case 3:
            ReadHashData(hashtable,hashsize);
            break;
        case 4:
            printf("Enter the name of the Channel to Find: ");
            gets(ChannelNameToFind);
            printf("\n");
            int key = HashSearch(hashtable,hashsize,ChannelNameToFind);
            printf("Found: %d  %s  %s  %s  %s\n",hashtable[key]->Rank,hashtable[key]->ChannelName,hashtable[key]->Uploads
                   ,hashtable[key]->Subscribers,hashtable[key]->Views);
            break;
        case 5:
            printf("Enter the name of the Channel to Delete: ");
            gets(ChannelNameToDelete);
            printf("\n");
            HashDelete(hashtable,hashsize,ChannelNameToDelete);
            break;
        case 6:
            outp = fopen("Youtube+Channels.txt","w");
            SaveHashData(outp,hashtable,hashsize);
            fclose(outp);
            break;
        case 7:
            return;
            break;
        default:
            printf("\n\nInvalid Input! Try again: \n\n");
            break;
        }
        printSubMenu2();
        scanf("%d",&x);
        getchar();
        printf("----------------------------------------\n");
        printf("\n");
    }
}

int main(){
    int x;
    printMainMenu();
    struct Node* T = MakeEmpty(NULL);
    HashTable hashtable[hashsize];
    scanf("%d", &x);
    getchar();
    printf("----------------------------------------\n");
    printf("\n");
    while(1){
        switch(x){
            case 1:
                T = LoadData(T);
                LoadSubMenu1(T);
                break;
            case 2:
                ReadHashFile(hashtable,hashsize);
                LoadSubMenu2(hashtable);
                break;
            case 3:
                return 0;
                break;
            default:
                printf("\n\nInvalid Input! Try again: \n\n");
                break;
        }
        printMainMenu();
        scanf("%d",&x);
        getchar();
        printf("----------------------------------------\n");
        printf("\n");
    }
}
