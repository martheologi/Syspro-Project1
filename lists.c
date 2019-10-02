#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"

//arxikopoiei th lista twn bitcoins
bitcoinListNode* initializeBitcoinList(){
    bitcoinListNode* head;

    head = malloc(sizeof(bitcoinListNode));
    head->next = NULL;
    head->bitcoinID = -1;
    head->value = 0;

    return head;
}

//pros8etei node sth lista twn bitcoins
void BitcoinListAppend(bitcoinListNode* ptr, int bitcoinId, int value){
    bitcoinListNode* newNode;
    //bitcoinListNode* temp = ptr;
    newNode = malloc(sizeof(bitcoinListNode));
    newNode->next = NULL;
    newNode->bitcoinID = bitcoinId;
    newNode->value = value;

    while(ptr->next!=NULL){
        ptr = ptr->next;
    }

    ptr->next = newNode;

    return;
}

void PrintBitcoinList(Wallet* wallet, int counter){
    bitcoinListNode* ptr;
    for(int i=0; i<counter; i++){
        printf("%s\t", wallet[i].userID);
        ptr = wallet[i].bitCoinList;
        while(ptr->next != NULL){
            printf("%d ($%d)\t", ptr->bitcoinID, ptr->value);
            ptr = ptr->next;
        }
        printf("%d ($%d)\t", ptr->bitcoinID, ptr->value);
        printf("%d\n", wallet[i].sum);
    }
}

void FreeBitcoinList(bitcoinListNode* ptr){
    bitcoinListNode* current = ptr;
    bitcoinListNode* temp_next;

    while(current != NULL){
        temp_next = current;
        current = current->next;
        free(temp_next);
    }
}

transactionListNode* initializeTransactionList(char* userID, char* transactionID, int transactionValue, Date date, Time mytime){
    transactionListNode* head;

    head = malloc(sizeof(transactionListNode));
    head->next = NULL;
    head->userID = malloc(strlen(userID)+1);
    strcpy(head->userID, userID);
    head->transactionValue = transactionValue;
    head->transactionID = malloc(strlen(transactionID)+1);
    strcpy(head->transactionID, transactionID);
    head->date = date;
    head->mytime = mytime;

    return head;
}

void TransactionListAppend(transactionListNode* ptr, char* userID, char* transactionID, int transactionValue, Date date, Time mytime){
    transactionListNode* newNode;
    transactionListNode* temp = ptr;

    newNode = malloc(sizeof(transactionListNode));
    newNode->next = NULL;
    newNode->transactionID = malloc(strlen(transactionID)+1);
    strcpy(newNode->transactionID, transactionID);
    newNode->userID = malloc(strlen(userID)+1);
    strcpy(newNode->userID, userID);
    newNode->transactionValue = transactionValue;
    newNode->date = date;
    newNode->mytime = mytime;

    while(temp->next!=NULL){
        temp = temp->next;
    }

    temp->next = newNode;

    return;
}

void FreeTransactionList(transactionListNode* ptr){
    transactionListNode* current = ptr;
    transactionListNode* temp_next;

    while(current != NULL){
        temp_next = current;
        free(temp_next->userID);
        free(temp_next->transactionID);
        current = current->next;
        free(temp_next);
    }
}

//arxikopoiei th lista me tis rizes twn dentrwn tou ka8e bitcoin
RootListNode* initializeRootList(){
    RootListNode* head;

    head = malloc(sizeof(RootListNode));
    head->next = NULL;
    head->root = NULL;

    return head;
}

//pros8etei komvo sth lista me tis rizes elegxontas oti den uparxei idio bitcoin
void RootListAppend(RootListNode* ptr, TreeRoot* root){
    RootListNode* newNode;
    newNode = malloc(sizeof(RootListNode));
    newNode->next = NULL;
    newNode->root = root;

    while(ptr->next!=NULL){
        if(ptr->root->bitcoinID == root->bitcoinID){
            printf("%d - %d\n", ptr->root->bitcoinID, root->bitcoinID);
            printf("Invalid BitcoinID\n");
            exit(0);
        }
        ptr = ptr->next;
    }

    ptr->next = newNode;

    return;
}

//vriskei th riza tou dentrou me bitcoinID
TreeRoot* FindRoot(RootListNode* rootList, int bitcoinID){
    RootListNode* current = rootList;

    while(current->next!=NULL && current->root->bitcoinID!=bitcoinID){
        current = current->next;
    }
    if(current!=NULL){
        return current->root;
    }
    else{
        printf("Wrong bitcoinID\n");
        exit(0);
    }
}

//enhmerwnei th lista me tis rizes vazontas auth pou exei parei neo komvo
void UpdateRootList(RootListNode** rootList, TreeRoot* root){
    RootListNode* tmp = *rootList;
    while(tmp->next!=NULL){
        if(tmp->root->bitcoinID == root->bitcoinID){
            tmp->root = root;
            break;
        }
        tmp = tmp->next;
    }
    return;
}

void PrintRootList(RootListNode* ptr){
    while(ptr->next!=NULL){
        printf("%d\t", ptr->root->bitcoinID);
        ptr = ptr->next;
    }
    printf("\n");
}

void FreeRootList(RootListNode* ptr){
    RootListNode* current = ptr;
    RootListNode* temp_next;

    while(current != NULL){
        temp_next = current;
        current = current->next;
        FreeTree(temp_next->root->treenode);
        free(temp_next->root);
        free(temp_next);
    }
}
