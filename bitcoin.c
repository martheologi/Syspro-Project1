#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "functions.h"

int main(int argc, char **argv){
  	FILE* balancesFile = fopen(argv[1], "r");
    FILE* transactionsFile = fopen(argv[2], "r");

    int bitcoinValue = atoi(argv[3]);
    int sender_ht_size = atoi(argv[4]);
    int receiver_ht_size = atoi(argv[5]);
    int bucketSize = atoi(argv[6]);

  	if(balancesFile == NULL){
    		fprintf(stderr, "Cannot open file: %s\n", argv[1]);
    		return 1;
    }

    size_t textsize;
  	char* text = NULL;
  	int counter = 0;
  	char* token = NULL;
    TreeRoot* root;
    RootListNode* rootList;

  	//metraw tis grammes tou arxeiou gia na desmeusw katallhlh mnhmh
  	while(getline(&text, &textsize, balancesFile) != -1){
  		  counter++;
  	}

  	rewind(balancesFile);

    Wallet* wallet = malloc(counter*sizeof(Wallet));
    rootList = initializeRootList();

    int i=0;
    int bitcoinID;

//ftiaxnw to wallet apo to balancesFile
    while(getline(&text, &textsize, balancesFile) != -1){
        token = strtok(text, " ");

        //pernaw to userId sth domh
        wallet[i].userID = malloc(strlen(token)+1);
        strcpy(wallet[i].userID, token);
        UserExistion(wallet, wallet[i].userID, i);

        //pernaw ta bitcoins sth domh
        wallet[i].bitCoinList = initializeBitcoinList(); //arxikopoiw th lista me ta bitcoins

        token = strtok(NULL, " ");

        //dinw sthn kefalh ths listas to prwto bitcoin kai arxikopoiw to sum
        if(token != NULL){
            bitcoinID = atoi(token);
            wallet[i].bitCoinList->bitcoinID = bitcoinID;
            wallet[i].bitCoinList->value = bitcoinValue;
            wallet[i].sum = bitcoinValue;

            //arxikopoiw thn kefalh ths listas me tis rizes an eimai sthn prwth meriptwsh
            if(i==0){
                rootList->root = initializeRoot(bitcoinID, i, bitcoinValue);
            }
            else{
              //alliws pros8etw riza sth lista
                root = initializeRoot(bitcoinID, i, bitcoinValue);
                RootListAppend(rootList, root);
            }
            token = strtok(NULL, " ");
        }
        //an den exei kanena bitcoin tote h lista tou einai arxikopoihmenh me -1
        //sxolio giati mou trwei seg se megala arxeia
    /*    else{
            //sthn periptwsh pou den exei bitcoins kovw to \n apo to userID kai dinw sto sum tou $0
            token = strtok(wallet[i].userID, "\n");
            wallet[i].userID = malloc(strlen(token)+1);
            strcpy(wallet[i].userID, token);
            wallet[i].sum = 0;
            i++;
            continue;
        }*/

        while( token != NULL ) {
            //pernaw to token sth lista twn bitcoins
            bitcoinID = atoi(token);
            BitcoinListAppend(wallet[i].bitCoinList, bitcoinID, bitcoinValue);
            wallet[i].sum += bitcoinValue;

            root =  initializeRoot(bitcoinID, i, bitcoinValue);
            RootListAppend(rootList, root);

            token = strtok(NULL, " ");
        }
        i++;
    }
    //PrintBitcoinList(wallet, counter);

//kanw ta transactions
    if(transactionsFile == NULL){
    		fprintf(stderr, "Cannot open file: %s\n", argv[2]);
    		return 1;
    }

    //desmeuw kai arxikopoiw ta duo hash tables
    HashTableBucket** senderHT = initializeHT(sender_ht_size);
    HashTableBucket** receiverHT = initializeHT(receiver_ht_size);

    char* trID;
    char* sender = NULL;
    char* receiver = NULL;
    Date date;
    Time mytime;
    int maxID = 0;
    int sender_hash_result;
    int receiver_hash_result;
    int sender_walletPos;
    int receiver_walletPos;
    int trValue;
    void* bucket = NULL;
    HT_record record;
    int bytes;

    while(getline(&text, &textsize, transactionsFile) != -1){

        TakeTransactionsInfo(text, &trID, &sender, &receiver, &trValue, &date, &mytime);

        if(atoi(trID)>maxID){
            maxID = atoi(trID);
        }

        ExecuteTransactions(senderHT, receiverHT, sender, receiver, sender_ht_size, receiver_ht_size, counter, wallet, rootList, bucketSize, trID, trValue, date, mytime);

        free(trID);
        free(sender);
        free(receiver);
    }

    //PrintBitcoinList(wallet, counter);

    size_t inputsize;
   	char* input = NULL;
   	char* command = NULL;
    char* trans = NULL;
    char* mytrID;

		printf("Transactions are done. Type a command : ");

		while(1){
    		getline(&input, &inputsize, stdin);
    		input[strlen(input)-1 ] = '\0';			//bgazoume to \n apo to string

       	command = strtok(input, " ");

        if(strcmp(command,"/requestTransaction") == 0){
            trans = strtok(NULL, "\n");
            checkTransactionsInfo(trans, &trID, &sender, &receiver, &trValue, &date, &mytime, &maxID);

            if(atoi(trID)>maxID){
                maxID = atoi(trID);
            }
            ExecuteTransactions(senderHT, receiverHT, sender, receiver, sender_ht_size, receiver_ht_size, counter, wallet, rootList, bucketSize, trID, trValue, date, mytime);

            free(trID);
            free(sender);
            free(receiver);
        }
        else if(strcmp(command,"/requestTransactions") == 0){
            printf("%s\n", input);
            char* first = strtok(NULL, " ");
            token = strtok(NULL, " ");

            if(token == NULL){

                FILE* inputFile = fopen(first, "r");
                if(inputFile==NULL){
                    printf("error opening file\n");
                    printf("\nType a command : ");
                    continue;
                }
                while(getline(&text, &textsize, inputFile) != -1){
                    token = strtok(text, ";");
                    checkTransactionsInfo(text, &trID, &sender, &receiver, &trValue, &date, &mytime, &maxID);

                    if(atoi(trID)>maxID){
                        maxID = atoi(trID);
                    }
                    ExecuteTransactions(senderHT, receiverHT, sender, receiver, sender_ht_size, receiver_ht_size, counter, wallet, rootList, bucketSize, trID, trValue, date, mytime);

                    free(trID);
                    free(sender);
                    free(receiver);
                }
                fclose(inputFile);
            }
        }
        else if(strcmp(command,"/findEarnings") == 0){
            receiver = strtok(NULL, " ");
            receiver_walletPos = findWalletPosition(wallet, receiver, counter);

            if(receiver_walletPos==-1) {
                printf("user %s does not exist\n", receiver);
                printf("\nType a command : ");
                continue;
            }

            printf("For user %s:\n", receiver);
            char* timedate = strtok(NULL, "\n");

            findEarnigsOrPayments(receiverHT, receiver, receiver_walletPos, receiver_ht_size, timedate);
        }
        else if(strcmp(command,"/findPayments") == 0){
            sender = strtok(NULL, " ");
            sender_walletPos = findWalletPosition(wallet, sender, counter);

            if(sender_walletPos==-1) {
                printf("user %s does not exist\n", sender);
                printf("\nType a command : ");
                continue;
            }

            printf("For user %s:\n", sender);
            char* timedate = strtok(NULL, "\n");

            findEarnigsOrPayments(senderHT, sender, sender_walletPos, sender_ht_size, timedate);
        }
        else if(strcmp(command,"/walletStatus") == 0){
            char* userID = strtok(NULL, "\n");
            int userPos = findWalletPosition(wallet, userID, counter);
            printf("User with walletID %s has $%d in wallet\n", userID, wallet[userPos].sum);
        }
        else if(strcmp(command,"/bitCoinStatus") == 0){
            bitcoinID = atoi(strtok(NULL, "\n"));
            bitcoinStatus(rootList, wallet, bitcoinID);
        }
        else if(strcmp(command,"/traceCoin") == 0){
            bitcoinID = atoi(strtok(NULL, "\n"));
            traceCoin(rootList, bitcoinID);
        }
        else if(strcmp(command,"/exit") == 0){
            //////FREEEEES
            free(text);
            free(input);

            FreeHashTable(senderHT, sender_ht_size);
            free(senderHT);
            FreeHashTable(receiverHT, receiver_ht_size);
            free(receiverHT);

            for(i=0; i<counter; i++){
                FreeBitcoinList(wallet[i].bitCoinList);
                free(wallet[i].userID);
            }
            free(wallet);

            FreeRootList(rootList);

            fclose(balancesFile);
            fclose(transactionsFile);

            return 1;
        }
        else{
  				    printf("There is no command %s\n",command );
  			}
  			printf("\nType a command : ");
    }
    fflush(stdout);

    return 0;
}
