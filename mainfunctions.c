#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "structs.h"
#include "functions.h"

void TakeTransactionsInfo(char* transaction, char** trID, char** sender, char** receiver, int* trValue, Date* date, Time* mytime){
    char* token = NULL;

    //pairnw to transactionID
    token = strtok(transaction, " ");
    *trID = malloc(strlen(token)+1);
    strcpy(*trID, token);

    //pairnw to userID tou sender kai to hasharw
    token = strtok(NULL, " ");
    *sender = malloc(strlen(token)+1);
    strcpy(*sender, token);

    //pairnw ton receiver
    token = strtok(NULL, " ");
    *receiver = malloc(strlen(token)+1);
    strcpy(*receiver, token);

    //pairnw to transaction Value
    token = strtok(NULL, " ");
    *trValue = atoi(token);

    //pairnw to Date
    token = strtok(NULL, " ");
    sscanf(token, "%d-%d-%d", &date->day, &date->month, &date->year);

    //pairnw to mytime
    token = strtok(NULL, "\n");
    sscanf(token, "%d:%d", &mytime->hour, &mytime->mins);

    return;
}

void checkTransactionsInfo(char* transaction, char** trID, char** sender, char** receiver, int* trValue, Date* date, Time* mytime, int* maxID){
    char* token = NULL;

    //dinw monadiko transactionID
    *trID = malloc(10);
    *maxID = *maxID + 1;
    sprintf(*trID, "%d", *maxID);

    //pairnw to userID tou sender kai to hasharw
    token = strtok(transaction, " ");
    *sender = malloc(strlen(token)+1);
    strcpy(*sender, token);

    //pairnw ton receiver
    token = strtok(NULL, " ");
    *receiver = malloc(strlen(token)+1);
    strcpy(*receiver, token);

    //pairnw to transaction Value
    token = strtok(NULL, " ");
    *trValue = atoi(token);

    token = strtok(NULL, " ");

    if(token == NULL){
        //an den mou dinoun hmeromhnia dinw thn trexousa wra kai hmeromhnia
        time_t t;
        time(&t);
        struct tm *info;
        char* buffer = malloc(strlen(ctime(&t)+1));
        info = localtime(&t);
        strftime(buffer,strlen(ctime(&t)+1),"%x - %I:%M%p", info);
        sscanf(buffer, "%d/%d/%d - %d:%d", &date->day, &date->month, &date->year, &mytime->hour, &mytime->mins);
        free(buffer);
    }
    else{
        //pairnw to Date
        sscanf(token, "%d-%d-%d", &date->day, &date->month, &date->year);

        //pairnw to mytime
        token = strtok(NULL, " ");
        sscanf(token, "%d:%d", &mytime->hour, &mytime->mins);
    }

    return;
}

void findEarnigsOrPayments(HashTableBucket** ht, char* user, int walletPos, int ht_size, char* timedate){
    HT_record record;
    int position;
    int amount = 0;
    transactionListNode* transNode;
    Time startTime, endTime;
    Date startDate, endDate;
    int timeFlag = 0, dateFlag = 0;
    int valT = -1, valD = -1;
    char* token = NULL;

    token = strtok(timedate, " ");
    //pairnw to date kai time an mou ta dinei o xrhsths
    if(token != NULL){
        if(token[2]==':'){
            timeFlag = 1;
            sscanf(token, "%d:%d", &startTime.hour, &startTime.mins);
            token = strtok(NULL, " ");
            //o xrhsths mou dinei mono time
            if(token[2] == ':'){
                sscanf(token, "%d:%d", &endTime.hour, &endTime.mins);
            }
            //o xrhsths mou dinei kai date
            else{
                dateFlag = 1;
                sscanf(token, "%d-%d-%d", &startDate.day, &startDate.month, &startDate.year);

                token = strtok(NULL, " ");
                sscanf(token, "%d:%d", &endTime.hour, &endTime.mins);

                token = strtok(NULL, "\n");
                sscanf(token, "%d-%d-%d", &endDate.day, &endDate.month, &endDate.year);
            }
        }
        //o xrhsths mou dinei mono date
        else{
            dateFlag = 1;
            sscanf(token, "%d-%d-%d", &startDate.day, &startDate.month, &startDate.year);

            token = strtok(NULL, "\n");
            sscanf(token, "%d-%d-%d", &endDate.day, &endDate.month, &endDate.year);
        }
    }
    int hash = hash_function(user, ht_size);
    HashTableBucket* tmp = ht[hash];
    //position = findUserInBucket(tmp->bucket, walletPos, tmp->recs);
    //if(position==-1){
    while(tmp!=NULL){
        position = findUserInBucket(tmp->bucket, walletPos, tmp->recs);
        if(position == -1){
            tmp = tmp->next;
        }
        else
            break;
    }
    //}

    if(position == -1){
        printf("user %s has no amount\n", user);
        return;
    }
    memcpy(&record, tmp->bucket + position*sizeof(HT_record), sizeof(HT_record));

    transNode = record.transactionList;

    while(transNode != NULL){
        if(timeFlag==1)
            valT = TimeValidation(startTime, endTime, transNode->mytime);
        if(dateFlag==1)
            valD = DateValidation(startDate, endDate, transNode->date);
        if((valT==1 && valD==1) || (timeFlag==0 && dateFlag==0) || (dateFlag==1 && timeFlag==0 && valD==1) || (timeFlag==1 && dateFlag==0 && valT==1)){
            amount += transNode->transactionValue;
            printf("transactionID: %s with: %s value: $%d\n", transNode->transactionID, transNode->userID, transNode->transactionValue);
        }
        transNode = transNode->next;
    }
    printf("Total amount $%d\n", amount);
    return;
}

void bitcoinStatus(RootListNode* rootList, Wallet* wallet, int bitcoinID){
    TreeRoot* root = FindRoot(rootList, bitcoinID);
    bitcoinListNode* listPtr = wallet[root->treenode->walletID].bitCoinList;
    while(listPtr!=NULL){
        if(listPtr->bitcoinID==bitcoinID)
            break;
        listPtr = listPtr->next;
    }
    printf("%d %d $(%d)", root->bitcoinID, root->treenode->value, listPtr->value);
}

void traceCoin(RootListNode* rootList, int bitcoinID){
    TreeRoot* root = FindRoot(rootList, bitcoinID);
    TreeNode* treenode = root->treenode;
    printTree(treenode);
}

void ExecuteTransactions(HashTableBucket** senderHT, HashTableBucket** receiverHT, char* sender, char* receiver, int sender_ht_size, int receiver_ht_size, int counter, Wallet* wallet, RootListNode* rootList, int bucketSize, char* trID, int trValue, Date date, Time mytime){
    int sender_hash_result = hash_function(sender, sender_ht_size);
    int receiver_hash_result = hash_function(receiver, receiver_ht_size);

    //pairnw th 8esh pou vrisketai o xrhsths sto wallet
    int sender_walletPos = findWalletPosition(wallet, sender, counter);
    int receiver_walletPos = findWalletPosition(wallet, receiver, counter);

    if(sender_walletPos==-1 || receiver_walletPos==-1 || sender_walletPos==receiver_walletPos){
        printf("Invalid transaction\n");
        return;
    }
    //elegxw an uparxoun arketa xrhmata sto wallet
    //printf("%d-%d\n", wallet[sender_walletPos].sum, trValue);
    if(wallet[sender_walletPos].sum<trValue){
        printf("Invalid transaction, not enough money\n");
        return;
    }

    senderHT = insertRecord(senderHT, sender_walletPos, sender_hash_result, bucketSize, receiver, trID, trValue, date, mytime);
    receiverHT = insertRecord(receiverHT, receiver_walletPos, receiver_hash_result, bucketSize, sender, trID, trValue, date, mytime);

    wallet = WalletUpdate(wallet, sender_walletPos, receiver_walletPos, trValue, &rootList);

    return;
}
