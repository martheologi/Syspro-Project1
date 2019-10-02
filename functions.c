#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"

void UserExistion(Wallet* wallet, char* userID, int lastUser){
    for(int i=0; i<lastUser; i++){
        if(strcmp(wallet[i].userID, userID)==0){
            printf("User already exists, exit application\n");
            exit(0);
        }
    }
    return;
}

//h hash function pros8etei ton ASCII ari8mo tou ka8e grammatos tou walletID
//kai epistrefei to upoloipo apo th diairesh me to mege8os tou hash table
int hash_function(char* walletID, int ht_size){
    int result = 0;
    int length = strlen(walletID);

    for(int i=0; i<length; i++){
        result += walletID[i] - '0';
    }
    return result%ht_size;
}

HashTableBucket** initializeHT(int ht_size){
    HashTableBucket** ht = malloc(ht_size*sizeof(HashTableBucket*));

    for(int i=0; i<ht_size; i++){
        ht[i] = malloc(sizeof(HashTableBucket));
        ht[i]->recs = 0;
        ht[i]->next = NULL;
        ht[i]->bucket = NULL;
    }

    return ht;
}

int findUserInBucket(void* bucket, int walletPos, int recs){
    int position = -1;
    int walletID;
    HT_record record;

    for(int i=0; i<recs; i++){
        memcpy(&record, bucket + i*sizeof(HT_record), sizeof(HT_record));
        if(record.walletID == walletPos){
            return i;
        }
    }
    return position;
}

HashTableBucket** insertRecord(HashTableBucket** ht, int walletPos, int hash_result, int bucketSize, char* userID, char* transactionID, int transactionValue, Date date, Time mytime){
    void* bucket;
    HT_record record;
    int recs_per_bucket = bucketSize/sizeof(HT_record);
    int position;

    //an den uparxei kanena record se auth th thesh tou hash table
    if(ht[hash_result]->recs == 0){
        ht[hash_result]->bucket = malloc(bucketSize);//initializeBucket(bucketSize);
        record.walletID = walletPos;
        record.transactionList = initializeTransactionList(userID, transactionID, transactionValue, date, mytime);
        memcpy(ht[hash_result]->bucket, &record, sizeof(HT_record));
        ht[hash_result]->recs=1;
    }
    //an uparxei record
    else{
        //elegxw an exei xxwro sto bucket
        if(ht[hash_result]->recs < recs_per_bucket){
            //an exei psaxnw an uparxei to walletID hdh sto bucket
            position = findUserInBucket(ht[hash_result]->bucket, walletPos, ht[hash_result]->recs);
            if(position == -1){
                //an den uparxei vazw to record sthn epomenh adeia 8esh
                record.walletID = walletPos;
                record.transactionList = initializeTransactionList(userID, transactionID, transactionValue, date, mytime);
                memcpy(ht[hash_result]->bucket + ht[hash_result]->recs*sizeof(HT_record), &record, sizeof(HT_record));
                ht[hash_result]->recs++;
            }
            else{
                //an uparxei enhmerwnw th lista me ta transaction tou
                memcpy(&record, ht[hash_result]->bucket + position*sizeof(HT_record), sizeof(HT_record));
                TransactionListAppend(record.transactionList, userID, transactionID, transactionValue, date, mytime);
                memcpy(ht[hash_result]->bucket + position*sizeof(HT_record), &record, sizeof(HT_record));
            }
        }
        else{
            //an den exei xwro psaxnw na dw an uparxei to walletID alliws ftiaxnw kainourio bucket kai to sundew me to gemato
            HashTableBucket* tmp = ht[hash_result];
            position = findUserInBucket(tmp->bucket, walletPos, tmp->recs);
            if(position==-1){
                while(tmp->next!=NULL){
                    position = findUserInBucket(tmp->bucket, walletPos, tmp->recs);
                    if(position == -1)
                        tmp = tmp->next;
                    else
                        break;
                }
            }
            //an uparxei enhmerwnw th lista me ta transactions tou
            if(position != -1){
                memcpy(&record, tmp->bucket + position*sizeof(HT_record), sizeof(HT_record));
                TransactionListAppend(record.transactionList, userID, transactionID, transactionValue, date, mytime);
                memcpy(tmp->bucket + position*sizeof(HT_record), &record, sizeof(HT_record));

            }
            //an den uparxei
            else{
                //ftiaxnw kainourio bucket an einai kai to teleutaio gemato
                if(tmp->recs == recs_per_bucket){
                    record.walletID = walletPos;
                    record.transactionList = initializeTransactionList(userID, transactionID, transactionValue, date, mytime);
                    HashTableBucket* newBucket = malloc(sizeof(HashTableBucket));
                    newBucket->next = NULL;
                    newBucket->recs = 1;
                    newBucket->bucket = malloc(bucketSize);

                    memcpy(newBucket->bucket, &record, sizeof(HT_record));
                    tmp->next = newBucket;

                }
                else{
                    //an exei xwro sto teleutaio vazw to record sthn epomenh adeia 8esh

                    record.walletID = walletPos;
                    record.transactionList = initializeTransactionList(userID, transactionID, transactionValue, date, mytime);
                    memcpy(tmp->bucket + tmp->recs*sizeof(HT_record), &record, sizeof(HT_record));
                    tmp->recs++;
                }
            }
        }
    }

    return ht;
}

void FreeHashTable(HashTableBucket** ht, int ht_size){
    HT_record record;
    for(int i=0; i<ht_size; i++){
        if(ht[i]->bucket!=NULL){
            while(ht[i]->next != NULL){
                for(int j=0; j<ht[i]->recs; j++){
                    memcpy(&record, ht[i]->bucket + j*sizeof(HT_record), sizeof(HT_record));
                    FreeTransactionList(record.transactionList);
                }
                free(ht[i]->bucket);
                ht[i] = ht[i]->next;
            }
            for(int j=0; j<ht[i]->recs; j++){
                memcpy(&record, ht[i]->bucket + j*sizeof(HT_record), sizeof(HT_record));
                FreeTransactionList(record.transactionList);
            }
            free(ht[i]->bucket);
        }
        free(ht[i]);
    }
    return;
}

int findWalletPosition(Wallet* wallet, char* userID, int counter){
    int position = -1;
    for(int i=0; i<counter; i++){
        if(strcmp(wallet[i].userID, userID) == 0){
            return i;
        }
    }
    return position;
}

Wallet* WalletUpdate(Wallet* wallet, int sender_walletPos, int receiver_walletPos, int value, RootListNode** rootList){
    Wallet* mywallet = wallet;
    bitcoinListNode* listPtr;
    int myvalue = value;
    TreeRoot* root;

    //an o sender exei na steilei to poso
    if(mywallet[sender_walletPos].sum > value){
        listPtr = mywallet[sender_walletPos].bitCoinList;
        //oso exei bitcoin me ligotera xrhmata apo auta pou prepei na steilei
        while(listPtr->value < value){
            value -= listPtr->value;  //meiwnw to value oso to poso pou exei to bitcoin
            if(mywallet[receiver_walletPos].bitCoinList->bitcoinID == -1){//an o receiver den exei kanena bitcoin enhmerwnei thn lista tou
                mywallet[receiver_walletPos].bitCoinList->value = value;
                mywallet[receiver_walletPos].bitCoinList->bitcoinID = listPtr->bitcoinID;
            }
            else{//alliws pros8etei to bitcoin sth lista tou receiver
                BitcoinListAppend(mywallet[receiver_walletPos].bitCoinList, listPtr->bitcoinID, listPtr->value);/////!!!!!!!!!!!
            }

            //enhmerwnw to dentro tou bitcoin
            root = FindRoot(*rootList, listPtr->bitcoinID);
            root->treenode = BitcoinTreeAppend(root->treenode, sender_walletPos, receiver_walletPos, listPtr->value);
            UpdateRootList(rootList, root);

            listPtr->value = 0;
            listPtr = listPtr->next;
        }
        //enhmerwnw ton sender
        listPtr->value -= value;
        mywallet[sender_walletPos].sum -= myvalue;

        //enhmerwnw to dentro tou bitcoin
        root = FindRoot(*rootList, listPtr->bitcoinID);
        root->treenode = BitcoinTreeAppend(root->treenode, sender_walletPos, receiver_walletPos, value);
        UpdateRootList(rootList, root);
    }
    else{
        printf("Invalid transaction\n");
        return mywallet;
    }

    //enhmerwnw ton receiver
    if(mywallet[receiver_walletPos].bitCoinList->bitcoinID == -1){
        mywallet[receiver_walletPos].bitCoinList->value = value;
        mywallet[receiver_walletPos].bitCoinList->bitcoinID = listPtr->bitcoinID;
        mywallet[receiver_walletPos].sum += myvalue;
    }
    else{
        BitcoinListAppend(mywallet[receiver_walletPos].bitCoinList, listPtr->bitcoinID, value);
        mywallet[receiver_walletPos].sum += myvalue;
    }
    return mywallet;
}

int DateValidation(Date startDate, Date endDate, Date nodeDate){
    if(nodeDate.year>=startDate.year && nodeDate.year<=endDate.year){
        if(nodeDate.month>=startDate.month && nodeDate.month<=endDate.month){
            if(nodeDate.day>=startDate.day && nodeDate.day<=endDate.day){
                return 1;
            }
        }
    }
    return 0;
}

int TimeValidation(Time startTime, Time endTime, Time nodeTime){
    if(nodeTime.hour>=startTime.hour && nodeTime.hour<=endTime.hour){
        if(nodeTime.hour>=startTime.hour && nodeTime.hour<=endTime.hour){
            return 1;
        }
    }
    return 0;
}
